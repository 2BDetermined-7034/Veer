package tbd.veer;

public class SwerveDrive {
	public byte[] nativeHandle;

	public SwerveDrive() {
		nativeInit();
	}

	private native void nativeInit();
}
