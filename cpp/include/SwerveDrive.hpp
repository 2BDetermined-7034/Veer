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
		angle       = 8,

		odometryVelocity    = 17,
		odometryAngularVelocity = 19,
		odometryPosition    = 20,
		odometryOrientation = 23
	};

	void update(JNIEnv* env, jobject obj);

	Eigen::Vector2d velocity;
	double          angularVelocity;
	Eigen::Vector3d position;
	Eigen::Vector3d orientation;

	struct SwerveModule {
		double prevAngleAngle, prevDriveAngle;
		Eigen::Vector2d position;   // Location relative to the robot pivot point
		Eigen::Vector2d normal;	    // Normalized position
		Eigen::Vector2d tangent;    // Tangent to position (NOT NORMALIZED)
	} modules[4];

	Eigen::Vector2d wheelDistance;
	double wheelRadius;
};

#endif