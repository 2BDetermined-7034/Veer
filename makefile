CC=arm-frc2024-linux-gnueabi-g++.exe
DEBUG=1
CFLAGS=-O2 -std=c++20
FIND=C:/msys64/usr/bin/find.exe

JAVA_SOURCES=$(shell $(FIND) ./java/ -name *.java)
JAVA_OUTPUTS=$(JAVA_SOURCES:./java/src/%.java=./java/lib/%.class)
JAR_FILES   =$(JAVA_OUTPUTS:./java/lib/%.class=-C ./java/lib %.class)

default: ./lib/Veer.jar

./lib/Veer.jar: $(JAVA_OUTPUTS)
	jar cf $@ $(JAR_FILES)

./java/lib/%.class: ./java/src/%.java
	javac -d ./java/lib/ $^

.PHONY: clean
clean:
	rm ./lib/Veer.jar
	rm -rf ./java/lib/**