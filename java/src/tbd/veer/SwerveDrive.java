package tbd.veer;

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
			angle       = 8;
	}
	
	private byte[]      nativeHandle;
	private double[]    transferRegion;

	public SwerveDrive() {
		nativeInit();
	}

	public double getMotorInfo(int index) {
		return transferRegion[index];
	}

	private native void nativeInit();
}
