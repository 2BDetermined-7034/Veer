#ifndef VEER_JARRAYFUNCTIONS_HPP
#define VEER_JARRAYFUNCTIONS_HPP

#ifndef VEER_JPTR_HPP
#error "jArrayFunctions.hpp is an internal header, and should not be used directly"
#endif

#include <jni.h>

template <class ArrayType>
class jArrayFunctions {
	static jfieldID     getFieldID(JNIEnv* env, jclass clazz, const char* handleName);
	static ArrayType    newArray(JNIEnv* env, size_t length);
	static void*        getArrayElements(JNIEnv* env, ArrayType handle);
	static size_t       getArraySize(JNIEnv* env, ArrayType handle);
	static void         releaseArrayElements(JNIEnv* env, ArrayType handle);
};

template <>
class jArrayFunctions<jbyteArray> {
public:
	static jfieldID getFieldID(JNIEnv* env, jclass clazz, const char* handleName) {
		return env->GetFieldID(clazz, handleName, "[B");
	}

	static jbyteArray newArray(JNIEnv* env, size_t length) {
		return env->NewByteArray(length);
	}
	
	static void* getArrayElements(JNIEnv* env, jbyteArray handle) {
		return env->GetByteArrayElements(handle, nullptr);
	}

	static size_t getArraySize(JNIEnv* env, jbyteArray handle) {
		return env->GetArrayLength(handle);
	}

	static void releaseArrayElements(JNIEnv* env, jbyteArray handle, void* ptr) {
		env->ReleaseByteArrayElements(handle, (jbyte*)ptr, 0);
	}
};

template <>
class jArrayFunctions<jdoubleArray> {
public:
	static jfieldID getFieldID(JNIEnv* env, jclass clazz, const char* handleName) {
		return env->GetFieldID(clazz, handleName, "[D");
	}

	static jdoubleArray newArray(JNIEnv* env, size_t length) {
		return env->NewDoubleArray(length);
	}
	
	static void* getArrayElements(JNIEnv* env, jdoubleArray handle) {
		return env->GetDoubleArrayElements(handle, nullptr);
	}

	static size_t getArraySize(JNIEnv* env, jdoubleArray handle) {
		return env->GetArrayLength(handle) * sizeof(jdouble);
	}

	static void releaseArrayElements(JNIEnv* env, jdoubleArray handle, void* ptr) {
		env->ReleaseDoubleArrayElements(handle, (jdouble*)ptr, 0);
	}
};

#endif