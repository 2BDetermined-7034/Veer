#include "../include/generated/tbd_veer_SwerveDrive.h"
#include "../include/SwerveDrive.hpp"
#include "../include/jptr.hpp"

#include <memory>

JNIEXPORT void JNICALL Java_tbd_veer_SwerveDrive_nativeInit(JNIEnv* env, jobject obj) {
	jptr<SwerveDrive> swerveHandle;
	swerveHandle.alloc(env, obj, "nativeMemory", 1);

	// Showcasing iterators
	for (auto& i: swerveHandle) {
		i.demoVariable = 123;
	}
}

JNIEXPORT jint JNICALL Java_tbd_veer_SwerveDrive_getNativeStatus(JNIEnv* env, jobject obj) {
	jptr<SwerveDrive> swerveHandle;
	swerveHandle.load(env, obj, "nativeMemory");

	return swerveHandle->demoVariable;
}