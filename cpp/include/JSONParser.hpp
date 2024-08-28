#ifndef VEER_JSONPARSER_HPP
#define VEER_JSONPARSER_HPP

#include <vector>
#include <string_view>
#include <cstdint>
#include <cstring>

class JSONParser {
public:
	JSONParser() {
		root = nullptr;
		arena = nullptr;
	}

	JSONParser(std::string_view json);

	~JSONParser(); 

	class JSONObject {
	public:
		JSONObject() {}

		JSONObject* find(const char* name) {
			for (uint32_t i = 0; i < treeValue.count; ++i) {
				if (treeValue.children[i]) {
					if (!std::strcmp(name, treeValue.children[i]->name)) {
						return treeValue.children[i];
					}
				}
			}

			return nullptr;
		}

		JSONObject* get(uint32_t i) {
			return treeValue.children[i];
		}

		JSONObject* operator[](uint32_t i) {
			return treeValue.children[i];
		}

		uint32_t size() const {
			return treeValue.count;
		}

		enum Type : uint32_t {
			object,
			array,
			string,
			number,
			boolean,
			null
		};

		union {
			const char* name;
			size_t _nameIndex;
		};
		union {
			struct {
				union {
					JSONObject** children;
					size_t _childrenIndex;
				};
				uint32_t count;
			} treeValue;

			union {
				const char* stringValue;
				size_t _stringIndex;
			};
			double numberValue;
			bool booleanValue;
		};
		Type        type;

		protected:

		struct ParseInfo;
		static size_t parse(ParseInfo& parseInfo);

		friend class JSONParser;
	}* root;

protected:
	char* arena;
};

#endif