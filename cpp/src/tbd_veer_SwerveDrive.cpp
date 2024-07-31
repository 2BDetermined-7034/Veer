#include "../include/generated/tbd_veer_SwerveDrive.h"
#include "../include/SwerveDrive.hpp"

JNIEXPORT void JNICALL Java_tbd_veer_SwerveDrive_nativeInit(JNIEnv* env, jobject obj) {
	jbyteArray memory;
	memory = env->NewByteArray(sizeof(SwerveDrive));

	jboolean isCopy = false;
	SwerveDrive* instance = (SwerveDrive*)env->GetByteArrayElements(memory, &isCopy);

	*instance = SwerveDrive(env, obj);

	instance->myNumber = 123;

	env->SetObjectField(obj, SwerveDrive::memoryID, memory);
	env->SetLongField(obj, SwerveDrive::handleID, (jlong)instance);
}

JNIEXPORT jint JNICALL Java_tbd_veer_SwerveDrive_getNativeStatus(JNIEnv* env, jobject obj) {
	jboolean isCopy = false;
	SwerveDrive* instance = (SwerveDrive*)env->GetLongField(obj, SwerveDrive::handleID);

	return instance->myNumber;
}