#!/usr/bin/env python3

# MODIFY THIS LINE TO SET WPILIB VERSION
version="2024.3.2"

# ----------------

import platform
import subprocess
import os
import json
import requests

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




# Only gets local copies of vendordep files for depdir wpilib/2024/maven
def get_local_vendor_deps():
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


def get_online_vendor_deps():
	# Chose this directory because thats where the WPIlib plugin puts them by default
	vendordeps_json_dir = os.fsencode(os.getcwd() + "/vendordeps")
	for file in os.listdir(vendordeps_json_dir):
		filename = os.fsdecode(file)
		if not filename.endswith(".json"):
			continue
		
		f = open(os.getcwd() + "/vendordeps/" + filename)

		data = json.load(f)
		base = data['mavenUrls'][0]

		for entry in data['javaDependencies']:
			if 'isJar' in entry and entry['isJar'] == False:
				print("Spec says this isn't a .jar file, moving on")
				continue
			groupId = str(entry['groupId'])
			artifactId = str(entry['artifactId'])
			version = str(entry['version'])

			url = gav_to_url(base, groupId, artifactId, version, "jar")
			print("jar url = ", url)


			#status = subprocess.call(
				#"curl -s " + url 
				#"./lib/%s-%s.jar" % (artifactId, version)	
				#,
				#shell=True
			#)
			perform_download(url)
		
		for entry in data['jniDependencies']:
			if 'isJar' in entry and entry['isJar'] == False:
				print("Spec says %s this isn't a .jar file, moving on" % artifactId)
				continue
			groupId = str(entry['groupId'])
			artifactId = str(entry['artifactId'])
			version = str(entry['version'])

			url = gav_to_url(base, groupId, artifactId, version, "jar")
			print("jni url = ", url)
			perform_download(url)

			

def perform_download(url):
	'''
	Do the downlaod of the file
	:param url: url of the file to be downloaded
	:return:  downloaded file       
	'''
	
	filename = './lib/' + url.split("/")[-1] # Location of jar outputs
	#print("Filename = ", filename)

	try:
		response = requests.get(url, allow_redirects=True)
		open(filename, "wb").write(response.content)    # overwritting file in case it exists
	except BaseException as be:
		print(f"Something went wrong while downloading: {url} and the following exception was raised: {be}. Exiting!"
        	f"This operation is mandatory, exiting!")
		exit(1)
	return filename

def gav_to_url(base, g, a, v, ext):
        '''
        This method creates the JAR or POM file link from G:A:V coordinates
        :param g: groupID
        :param a: artefactID
        :param v: artefact version
        :param ext: set pom if you want to create a link for the pom file,
                    set jar if you want to create a link for the jar file
        :return: url pointing to the desired file: jar or pom
        '''
        
        gid = g.replace(".", "/")
        return base + gid + "/" + a + "/" + v + "/" + a + "-" + v + "." + ext

get_online_vendor_deps()