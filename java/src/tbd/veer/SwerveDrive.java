package tbd.veer;

import edu.wpi.first.wpilibj.*;
import com.ctre.phoenix6.hardware.*;

public class SwerveDrive {
	public static final class TransferIndices {
		public static final int
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
			odometryOrientation = 23;
	}
	
	private byte[]      nativeHandle;
	private double[]    transferRegion;

	private TalonFX     frontLeftDrive;
	private TalonFX     frontLeftAngle;
	private TalonFX     frontRightDrive;
	private TalonFX     frontRightAngle;
	private TalonFX     backLeftDrive;
	private TalonFX     backLeftAngle;
	private TalonFX     backRightDrive;
	private TalonFX     backRightAngle;

	public SwerveDrive() {
		nativeInit();

		frontLeftDrive =    new TalonFX(1);
		frontLeftAngle =    new TalonFX(2);
		frontRightDrive =   new TalonFX(3);
		frontRightAngle =   new TalonFX(4);
		backLeftDrive =     new TalonFX(8);
		backLeftAngle =     new TalonFX(7);
		backRightDrive =    new TalonFX(5);
		backRightAngle =    new TalonFX(6);
	}

	public void update() {
		transferRegion[TransferIndices.odometryVelocity] =      1.0;
		transferRegion[TransferIndices.odometryVelocity + 1] =  0;
		transferRegion[TransferIndices.odometryVelocity + 2] =  0;

		transferRegion[TransferIndices.frontLeft + TransferIndices.driveMotor + TransferIndices.angle] = 0;
		nativeUpdate();
		frontLeftDrive.set(transferRegion[TransferIndices.frontLeft + TransferIndices.driveMotor + TransferIndices.speed]);
	}

	public double getMotorInfo(int index) {
		return transferRegion[index];
	}

	private native void nativeInit();
	private native void nativeUpdate();
}
