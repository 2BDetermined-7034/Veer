#ifndef VEER_SWERVEDRIVE_HPP
#define VEER_SWERVEDRIVE_HPP

#include "jptr.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"

class SwerveDrive {
public:
	SwerveDrive(JNIEnv* env, jobject obj, Eigen::Vector2d wheelDistance, double wheelRadius);

	enum TransferIndices {
		frontLeft   = 0,
		frontRight  = 1,
		backLeft    = 2,
		backRight   = 3,

		driveMotor  = 0,
		angleMotor  = 4,

		speed       = 0,
		angle       = 8
	};

	void update(JNIEnv* env, jobject obj);

	Eigen::Vector3d position;
	Eigen::Vector3d velocity;
	Eigen::Vector3d orientation;
	Eigen::Vector3d angularVelocity;

	struct SwerveModule {
		double prevAngleAngle, prevDriveAngle;
		const Eigen::Vector2d tangent;
	} modules[4];

	const Eigen::Vector2d wheelDistance;
	const double wheelRadius;
};

#endif