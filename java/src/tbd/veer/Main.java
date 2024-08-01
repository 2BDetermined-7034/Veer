package tbd.veer;

public class Main {
	public static void main(String[] args) {
		System.load("C:\\Users\\Magneto\\Documents\\FRC\\Veer\\lib\\x64\\Veer.dll");
		
		SwerveDrive swerveDrive = new SwerveDrive();

		System.out.println(swerveDrive.getNativeStatus());
	}
}
