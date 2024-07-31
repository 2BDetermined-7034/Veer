#ifndef VEER_SWERVEDRIVE_HPP
#define VEER_SWERVEDRIVE_HPP

#include <jni.h>

class SwerveDrive {
public:
	SwerveDrive(JNIEnv* env, jobject obj);

	int myNumber;

	static jclass clazz;
	static jfieldID memoryID;
	static jfieldID handleID;
};

#endif