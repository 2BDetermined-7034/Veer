#include "../include/SwerveDrive.hpp"

// Static definitions
jclass      SwerveDrive::clazz;
jfieldID    SwerveDrive::memoryID;
jfieldID    SwerveDrive::handleID;

SwerveDrive::SwerveDrive(JNIEnv* env, jobject obj) {
	if (SwerveDrive::clazz == NULL) {
		clazz = env->GetObjectClass(obj);
		memoryID = env->GetFieldID(clazz, "nativeMemory", "[B");
		handleID = env->GetFieldID(clazz, "nativeHandle", "J");
	}
	
}