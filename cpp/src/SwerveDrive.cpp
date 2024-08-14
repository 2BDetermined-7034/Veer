#include "../include/SwerveDrive.hpp"

SwerveDrive::SwerveDrive(JNIEnv* env, jobject obj, Eigen::Vector2d wheelDistance, double wheelRadius)
	: wheelDistance(wheelDistance), wheelRadius(wheelRadius) {

	jptr<double, jdoubleArray> transferRegion;
	transferRegion.alloc(env, obj, "transferRegion", 25);

	modules[frontLeft].position =   Eigen::Vector2d(wheelDistance.x(),  wheelDistance.y());
	modules[frontRight].position =  Eigen::Vector2d(-wheelDistance.x(), wheelDistance.y());
	modules[backRight].position =   Eigen::Vector2d(-wheelDistance.x(),-wheelDistance.y());
	modules[backLeft].position =    Eigen::Vector2d( wheelDistance.x(),-wheelDistance.y());

	for (int i = 0; i < 4; ++i) {
		Eigen::Vector2d position = modules[i].position;
		modules[i].normal = position.normalized();
		modules[i].tangent = Eigen::Vector2d(position.y(), -position.x());
	}
}

void SwerveDrive::update(JNIEnv* env, jobject obj) {
	jptr<double, jdoubleArray> transferRegion;
	transferRegion.load(env, obj, "transferRegion");

	std::memcpy(&velocity, transferRegion.data() + odometryVelocity, sizeof(Eigen::Vector2d));
	std::memcpy(&angularVelocity, transferRegion.data() + odometryAngularVelocity, sizeof(double));

	for (int i = 0; i < 4; ++i) {
		Eigen::Vector2d moduleVelocity = velocity + modules[i].tangent * angularVelocity;

		transferRegion[i + driveMotor + speed] = moduleVelocity.norm();
		transferRegion[i + angleMotor + speed] = std::atan2(moduleVelocity.y(), moduleVelocity.x());
	}

	std::memcpy(&position, transferRegion.data() + odometryPosition, sizeof(Eigen::Vector3d));
	std::memcpy(&orientation, transferRegion.data() + odometryOrientation, sizeof(Eigen::Vector3d));
}