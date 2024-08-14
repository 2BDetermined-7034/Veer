package tbd.veer;

import edu.wpi.first.wpilibj.*;

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

	public SwerveDrive() {
		nativeInit();
	}

	public void update() {
		
	}

	public double getMotorInfo(int index) {
		return transferRegion[index];
	}

	private native void nativeInit();
	private native void nativeUpdate();
}
