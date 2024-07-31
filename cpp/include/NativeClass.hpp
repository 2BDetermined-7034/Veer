#ifndef VEER_NATIVECLASS_HPP
#define VEER_NATIVECLASS_HPP

#include <jni.h>

template <class T>
class NativeClass {
public:
	template <typename... U>
	T* getHandles(JNIEnv* env, jobject obj, jclass& clazz, jfieldID& memoryID, jfieldID& handleID, U... args) {
		clazz = env->GetObjectClass(obj);
		memoryID = env->GetFieldID(clazz, "nativeMemory", "[B");
		handleID = env->GetFieldID(clazz, "nativeHandle", "J");

		jbyteArray memory;
		memory = env->NewByteArray(sizeof(T));

		jboolean isCopy = false;
		T* instance = (T*)env->GetByteArrayElements(memory, &isCopy);

		*instance = T(args...);

		env->SetObjectField(obj, memoryID, memory);
		env->SetLongField(obj, handleID, (jlong)instance);
	}
};

#endif