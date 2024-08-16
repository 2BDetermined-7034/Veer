#!/usr/bin/env python3

# MODIFY THIS LINE TO SET WPILIB VERSION
version="2024.3.2"

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




import os
import json

# Only gets local copies of vendordep files for depdir wpilib/2024/maven
def get_vendor_deps():
	if platform.system() == "Windows" or platform.system().find("MSYS") != -1:
		depdir = "C:/Users/Public/wpilib/2024/maven/"
	else:
		depdir = "" # TODO Add GNU/Linux support
	vendordeps_json_dir = os.fsencode(os.getcwd() + "/vendordeps")
	for file in os.listdir(vendordeps_json_dir):
		filename = os.fsdecode(file)
		if not filename.endswith(".json"):
			continue
		
		f = open(os.getcwd() + "/vendordeps/" + filename)
		data = json.load(f)
		for entry in data['javaDependencies']:
			if 'isJar' in entry and entry['isJar'] == False:
				print("Spec says this isn't a .jar file, moving on")
			groupId = str(entry['groupId'])
			artifactId = str(entry['artifactId'])
			version = str(entry['version'])
			# print("%s %s %s" % (groupId, artifactId, version))	

			status = subprocess.call(
				"cp " + depdir + "/" + groupId.replace(".", "/") + "/" + artifactId + "/" + version + "/" \
					+ artifactId + "-" + version + ".jar" # This line is the jar filename
				" ./lib/" + groupId + "-" + artifactId + "-" + version + ".jar"
				,
				shell=True
			)
			if status == 1:
				print("Could not import local vendordep %s (cp failed)" % (groupId))
			if status == 0:
				print("Successfully imported local vendordep %s %s" % (groupId, artifactId))
		for entry in data['jniDependencies']:
			break
		for entry in data['cppDependencies']:
			break


get_vendor_deps()