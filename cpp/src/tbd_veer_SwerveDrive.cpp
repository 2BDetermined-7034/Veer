#include "../include/generated/tbd_veer_SwerveDrive.h"
#include "../include/SwerveDrive.hpp"
#include "../include/jptr.hpp"

#include <memory>

JNIEXPORT void JNICALL Java_tbd_veer_SwerveDrive_nativeInit(JNIEnv* env, jobject obj) {
	jptr<SwerveDrive> swerveHandle;
	swerveHandle.alloc(env, obj, "nativeHandle", 1);

	swerveHandle[0] = SwerveDrive(env, obj, Eigen::Vector2d(1.0, 1.0), 0.05);
}

JNIEXPORT void JNICALL Java_tbd_veer_SwerveDrive_nativeUpdate(JNIEnv* env, jobject obj) {
	jptr<SwerveDrive> swerveHandle;
	swerveHandle.load(env, obj, "nativeHandle");

	swerveHandle->update(env, obj);
}