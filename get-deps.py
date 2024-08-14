#!/usr/bin/env python3

# MODIFY THIS LINE TO SET WPILIB VERSION
version="2024.2.1"

# ----------------

import platform
import subprocess

depdir = ""

if platform.system() == "Windows" or platform.system().find("MSYS") != -1:
	depdir = "C:/Users/Public/wpilib/2024/maven/edu/wpi/first"
else:
	depdir = "" # TODO Add GNU/Linux support

dependencies = []

with open("dependencies.txt", "r") as file:
	dependencies = file.readlines()

for i in dependencies:
	i = i.removesuffix("\n")
	status = subprocess.call(
		"cp " + depdir + "/" + i + "/" + i + "-java/" + version + "/" + i + "-java-" + version + ".jar"
		" ./lib/" + i + ".jar"
		,
		shell=True
	)
