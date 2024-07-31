package tbd.veer;

public class Main {
	public static void main(String[] args) {
		System.load("C:\\Users\\wlhsf\\Documents\\2024\\Veer\\lib\\x64\\Veer.dll");
		SwerveDrive swerveDrive = new SwerveDrive();

		System.out.println(swerveDrive.getNativeStatus());
	}
}
