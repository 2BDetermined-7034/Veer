package tbd.veer;

public class SwerveDrive {
	private byte[]  nativeMemory;
	private long    nativeHandle;

	public SwerveDrive() {
		nativeInit();
	}

	public native int getNativeStatus();

	private native void nativeInit();
}
