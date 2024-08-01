#include "../include/generated/tbd_veer_SwerveDrive.h"
#include "../include/SwerveDrive.hpp"
#include "../include/jptr.hpp"

#include <memory>

JNIEXPORT void JNICALL Java_tbd_veer_SwerveDrive_nativeInit(JNIEnv* env, jobject obj) {
	jptr<SwerveDrive> swerveHandle;
	swerveHandle.alloc(env, obj, "nativeHandle", 1);
}