#ifndef VEER_JPTR_HPP
#define VEER_JPTR_HPP

#include "jArrayFunctions.hpp"
#include <iterator>

/*
 * class jptr<T> - Smart ownership over memory owned by the JVM.
 * This class does not satisfy CopyConstructible nor CopyAssignable.
 * However, it does satisfy MoveConstructible and MoveAssignable.
 *
 * References:
 * https://en.cppreference.com/w/cpp/named_req/CopyConstructible
 * https://en.cppreference.com/w/cpp/named_req/CopyAssignable
 *
 * https://en.cppreference.com/w/cpp/named_req/MoveConstructible
 * https://en.cppreference.com/w/cpp/named_req/MoveAssignable
 */
template <typename T, class ArrayType = jbyteArray>
class jptr {
public:
// Constructors, assignment operators, and deconstructor
	jptr() {
		jhandle = NULL;
		ptr = nullptr;
		count = 0;
	}

	jptr(const jptr&) = delete;
	jptr& operator=(const jptr&) = delete;

	jptr(jptr&& c) {
		this->~jptr();

		this->env = c.env;
		this->jhandle = c.jhandle;
		this->ptr = c.ptr;
		this->count = c.count;

		c.jhandle = NULL;
		c.ptr = nullptr;
		c.count = 0;
	}

	jptr& operator=(jptr&& c) {
		this->~jptr();

		this->env = c.env;
		this->jhandle = c.jhandle;
		this->ptr = c.ptr;
		this->count = c.count;

		c.jhandle = NULL;
		c.ptr = nullptr;
		c.count = 0;

		return *this;
	}

	~jptr() {
		if (jhandle) {
			jArrayFunctions<ArrayType>::releaseArrayElements(env, jhandle, (void*)ptr);
			jhandle = NULL;
			ptr = nullptr;
			count = 0;
		}
	}

// Creation functions
	void alloc(JNIEnv* env, jobject obj, const char* handleName, size_t count) {
		jclass clazz = env->GetObjectClass(obj);
		jfieldID handleID = jArrayFunctions<ArrayType>::getFieldID(env, clazz, handleName);

		ArrayType memory = jArrayFunctions<ArrayType>::newArray(env, count * sizeof(T));
		env->SetObjectField(obj, handleID, memory);

		this->env = env;
		this->jhandle = (ArrayType)env->GetObjectField(obj, handleID);
		this->ptr = (T*)jArrayFunctions<ArrayType>::getArrayElements(env, this->jhandle);
		this->count = count;
	}

	void load(JNIEnv* env, jobject obj, const char* handleName) {
		jclass clazz = env->GetObjectClass(obj);
		jfieldID handleID = jArrayFunctions<ArrayType>::getFieldID(env, clazz, handleName);

		this->env = env;
		this->jhandle = (ArrayType)env->GetObjectField(obj, handleID);
		this->ptr = (T*)jArrayFunctions<ArrayType>::getArrayElements(env, this->jhandle);
		this->count = jArrayFunctions<ArrayType>::getArraySize(env, this->jhandle) / sizeof(T);
	}

// Operator overloads
	inline T* data() const {
		return ptr;
	}

	inline size_t size() const {
		return count;
	}

	inline size_t size_bytes() const {
		return sizeof(T) * count;
	}

	template <typename I = size_t> requires std::is_integral_v<T>
	inline T& operator[](I i) const {
		return ptr[i];
	}

	inline T& operator*() const {
		return *ptr;
	}

	inline T* operator->() const {
		return ptr;
	}

	inline operator T*() const {
		return ptr;
	}

// Iterators
	inline T* begin() const {
		return ptr;
	}

	inline T* end() const {
		return ptr + count;
	}

private:
	JNIEnv*     env;
	ArrayType  jhandle;
	T*          ptr;
	size_t      count;
};

#endif