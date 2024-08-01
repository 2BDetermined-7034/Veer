package tbd.veer;

public class SwerveDrive {
	public byte[]  nativeMemory;

	public SwerveDrive() {
		nativeInit();
	}

	public native int getNativeStatus();

	private native void nativeInit();
}
