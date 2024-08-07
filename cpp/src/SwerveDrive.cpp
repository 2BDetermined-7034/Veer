#include "../include/SwerveDrive.hpp"

SwerveDrive::SwerveDrive(JNIEnv* env, jobject obj, Eigen::Vector2d wheelDistance, double wheelRadius)
	: wheelDistance(wheelDistance), wheelRadius(wheelRadius) {

	jptr<double, jdoubleArray> transferRegion;
	transferRegion.alloc(env, obj, "transferRegion", 16);

	modules[0].tangent = Eigen::Matrix<double, 2, 1>(wheelDistance.y(), -wheelDistance.x());
}

void SwerveDrive::update(JNIEnv* env, jobject obj) {
	
}