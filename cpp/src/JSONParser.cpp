#include "../include/JSONParser.hpp"

#include <vector>
#include <span>
#include <charconv>
#include <cctype>
#include <cstdio>
#include <cstdlib>

enum class TokenType : char {
	openBrace,
	closeBrace,
	openBracket,
	closeBracket,
	colon,
	comma,

	number,
	string,
	booleanTrue,
	booleanFalse,
	null
};

struct Token {
	const char* string;
	uint32_t    length;
	TokenType   type;
};

struct JSONParser::JSONObject::ParseInfo {
	std::vector<JSONParser::JSONObject> objects;
	std::vector<size_t> children;
	std::vector<char>   strings;
	Token*      currentToken;
	Token*      end;
};

JSONParser::JSONParser(std::string_view json) {	
	std::vector<Token> tokens;

	const char* p = json.begin();
	const char* end = json.end();
	while (p < end) {
		char c = *p;
		if (std::isspace(c)) {
			++p;
			continue;
		} else if (c == '"') {
			tokens.push_back({
				.string = ++p,
				.length = 4,
				.type = TokenType::string
			});

			while (p != json.end() && *p != '"') {
				++p;
			}

			tokens.back().length = p - tokens.back().string;

			++p;
		} else if (isdigit(c) || c == '-') {
			tokens.push_back({
				.string = p,
				.length = 0,
				.type = TokenType::number
			});

			do {
				++p;
			} while (p != json.end() && isdigit(*p));

			if (*p == '.') {
				do {
					++p;
				} while (p != json.end() && isdigit(*p));
			}

			tokens.back().length = p - tokens.back().string;
		/* Switch statement isn't utilized because it will significantly bloat execuable size. */
		} else if (c == '{') {
			tokens.push_back({
				.type = TokenType::openBrace
			});
			++p;
		} else if (c == '}') {
			tokens.push_back({
				.type = TokenType::closeBrace
			});
			++p;
		} else if (c == '[') {
			tokens.push_back({
				.type = TokenType::openBracket
			});
			++p;
		} else if (c == ']') {
			tokens.push_back({
				.type = TokenType::closeBracket
			});
			++p;
		} else if (c == ':') {
			tokens.push_back({
				.type = TokenType::colon
			});
			++p;
		} else if (c == ',') {
			tokens.push_back({
				.type = TokenType::comma
			});
			++p;
		} else {
			if (p > json.end() - 5) {
				++p;
				continue;
			}

			uint32_t test = *(uint32_t*)p;

			if (test == 'eurt') {
				tokens.push_back({
					.type = TokenType::booleanTrue,
				});
				p += 4;
			} else if (test == 'llun') {
				tokens.push_back({
					.type = TokenType::null,
				});
				p += 4;
			} else if (test == 'slaf') {
				if (p[4] == 'e') {
					tokens.push_back({
						.type = TokenType::booleanFalse,
					});
					++p;
				}
				p += 4;
			} else {
				++p;
			}
		}
	}

	if (tokens[0].type == TokenType::openBrace) {
		JSONObject::ParseInfo parseInfo = {
			.currentToken = tokens.data(),
			.end = tokens.data() + tokens.size(),
		};
		
		size_t offset = JSONObject::parse(parseInfo);

		size_t objectSize = parseInfo.objects.size() * sizeof(JSONObject);
		size_t childrenSize = parseInfo.children.size() * sizeof(size_t);
		size_t stringSize = parseInfo.strings.size() * sizeof(char);

		arena = (char*)malloc(objectSize + childrenSize + stringSize);

		std::span<JSONObject>   objects((JSONObject*)arena, parseInfo.objects.size());
		std::span<JSONObject*>  children((JSONObject**)(arena + objectSize), parseInfo.children.size());
		std::span<char>         strings((char*)(arena + objectSize + childrenSize), parseInfo.strings.size());

		std::copy(parseInfo.objects.begin(), parseInfo.objects.end(), objects.begin());
		std::copy(parseInfo.children.begin(), parseInfo.children.end(), (size_t*)children.begin().base());
		std::copy(parseInfo.strings.begin(), parseInfo.strings.end(), strings.begin());

		for (size_t i = 0; i < parseInfo.children.size(); ++i) {
			children[i] = objects.data() + (size_t)children[i];
		}

		for (size_t i = 0; i < parseInfo.objects.size(); ++i) {
			objects[i].name = strings.data() + (size_t)objects[i].name;

			if (objects[i].type == JSONObject::object || objects[i].type == JSONObject::array) {
				objects[i].treeValue.children = children.data() + objects[i].treeValue._childrenIndex;
			} else if (objects[i].type == JSONObject::string) {
				objects[i].stringValue = strings.data() + objects[i]._stringIndex;
			}
		}

		root = objects.data() + offset;

	} else {
		root = nullptr;
		arena = nullptr;
	}
}

JSONParser::~JSONParser() {
	if (arena) {
		free(arena);
		arena = nullptr;
	}
}

size_t JSONParser::JSONObject::parse(JSONParser::JSONObject::ParseInfo& parseInfo) {
	JSONObject result;

	switch (parseInfo.currentToken->type) {
	case TokenType::openBrace: {
		++parseInfo.currentToken;

		if (parseInfo.currentToken >= parseInfo.end) {
			return -1; // Error
		}

		result.type = JSONObject::object;
		result.treeValue.count = 0;

		if (parseInfo.currentToken->type == TokenType::closeBrace) {
			++parseInfo.currentToken;
			parseInfo.objects.push_back(result);
			return parseInfo.objects.size() - 1;
		}

		std::vector<size_t> children;

		while (parseInfo.currentToken->type == TokenType::string) {
			size_t stringOffset = parseInfo.strings.size();
			parseInfo.strings.resize(stringOffset + parseInfo.currentToken->length + 1);
			std::memcpy(parseInfo.strings.data() + stringOffset, parseInfo.currentToken->string, parseInfo.currentToken->length);
			parseInfo.strings.back() = '\0';

			// I didn't want to do two range checks so I only increased currentToken once
			parseInfo.currentToken += 2;
			if (parseInfo.currentToken >= parseInfo.end) {
				return -1; // Error
			}

			// Read the above comment
			if (parseInfo.currentToken[-1].type != TokenType::colon) {
				return -1; // Error
			}

			size_t child = parse(parseInfo);
			if (child != -1) {
				children.push_back(child);
				parseInfo.objects[child]._nameIndex = stringOffset;
			}

			if (parseInfo.currentToken >= parseInfo.end) {
				return -1; // Error
			}

			if (parseInfo.currentToken->type == TokenType::closeBrace) {
				break;
			}

			if (parseInfo.currentToken->type != TokenType::comma) {
				return -1; // Error
			}

			++parseInfo.currentToken;

			if (parseInfo.currentToken >= parseInfo.end) {
				return -1; // Error
			}
		}

		++parseInfo.currentToken;

		size_t childrenOffset = parseInfo.children.size();
		result.treeValue.count = children.size();
		result.treeValue._childrenIndex = childrenOffset;
		parseInfo.children.resize(childrenOffset + result.treeValue.count);
		std::copy(children.begin(), children.end(), parseInfo.children.begin() + childrenOffset);
		parseInfo.objects.push_back(result);
		return parseInfo.objects.size() - 1;
	}
	case TokenType::openBracket: {
		++parseInfo.currentToken;

		if (parseInfo.currentToken >= parseInfo.end) {
			return -1; // Error
		}

		result.type = JSONObject::array;
		result.treeValue.count = 0;

		if (parseInfo.currentToken->type == TokenType::closeBracket) {
			++parseInfo.currentToken;
			parseInfo.objects.push_back(result);
			return parseInfo.objects.size() - 1;
		}

		std::vector<size_t> children;

		while (1) {
			children.push_back(parse(parseInfo));

			if (parseInfo.currentToken >= parseInfo.end) {
				return -1; // Error
			}

			if (parseInfo.currentToken->type == TokenType::closeBracket) {
				break;
			}

			if (parseInfo.currentToken->type != TokenType::comma) {
				return -1; // Error
			}

			++parseInfo.currentToken;

			if (parseInfo.currentToken >= parseInfo.end) {
				return -1; // Error
			}
		}

		++parseInfo.currentToken;

		size_t childrenOffset = parseInfo.children.size();
		result.treeValue.count = children.size();
		result.treeValue._childrenIndex = childrenOffset;
		parseInfo.children.resize(childrenOffset + result.treeValue.count);
		std::copy(children.begin(), children.end(), parseInfo.children.begin() + childrenOffset);
		parseInfo.objects.push_back(result);
		return parseInfo.objects.size() - 1;
	}
	case TokenType::string: {
		size_t stringOffset = parseInfo.strings.size();
		parseInfo.strings.resize(stringOffset + parseInfo.currentToken->length + 1);
		std::memcpy(parseInfo.strings.data() + stringOffset, parseInfo.currentToken->string, parseInfo.currentToken->length);
		parseInfo.strings.back() = '\0';

		result.type = JSONObject::string;
		result._stringIndex = stringOffset; 
		break;
	}
	case TokenType::number:
		result.type = JSONObject::number;
		std::from_chars(
			parseInfo.currentToken->string,
			parseInfo.currentToken->string + parseInfo.currentToken->length,
			result.numberValue
		);
		break;
	case TokenType::booleanTrue:
	 	result.type = JSONObject::boolean;
		result.booleanValue = true;
		break;
	case TokenType::booleanFalse:
	 	result.type = JSONObject::boolean;
		result.booleanValue = false;
		break;
	case TokenType::null:
	 	result.type = JSONObject::null;
		break;
	}

	++parseInfo.currentToken;

	parseInfo.objects.push_back(result);
	return parseInfo.objects.size() - 1;
}