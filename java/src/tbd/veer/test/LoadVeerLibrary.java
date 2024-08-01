package tbd.veer.test;

import tbd.veer.*;

public class LoadVeerLibrary {
	public static void main(String[] args) {
		try {
			Veer.init();
		} catch (UnsatisfiedLinkError e) {
			System.err.println(e.getMessage());

			System.exit(1);
		}

		System.exit(0);
	}
}
