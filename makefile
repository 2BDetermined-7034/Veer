DEBUG=1

ifeq ($(OS), Windows_NT)
	ARM_CC=C:/Users/Public/wpilib/2024/roborio/bin/arm-frc2024-linux-gnueabi-g++.exe
	CC=g++.exe
	FIND=C:/msys64/usr/bin/find.exe
	CPP_SHARED_OUTPUT_x64=./lib/x64/Veer.dll
	LDFLAGS=-Wl,--subsystem,windows
else
	ARM_CC=arm-none-eabi-g++
	CC=g++
	FIND=/usr/bin/find
	CPP_SHARED_OUTPUT_x64=./lib/x64/libVeer.so
	LDFLAGS=
endif

ifeq ($(DEBUG), 1)
	CFLAGS=-fPIC -O2 -std=c++20 -DDEBUG "-I$(CURDIR)/cpp/include/jni/"
else
	CFLAGS=-fPIC -O2 -std=c++20 "-I$(CURDIR)/cpp/include/jni/""
endif

JAVA_SOURCES=$(shell $(FIND) ./java/src/ -name *.java)
JAVA_OUTPUTS=$(JAVA_SOURCES:./java/src/%.java=./java/lib/%.class)
JAR_FILES   =$(JAVA_OUTPUTS:./java/lib/%.class=-C ./java/lib/ %.class)
JAVA_DEPS   =$(shell cat dependencies.txt)
JAR_DEPS    ="$(shell echo $(JAVA_DEPS:%=./lib/%.jar) | tr ' ' ';')"

CPP_SOURCES    =$(shell $(FIND) ./cpp/src/ -name *.cpp)
CPP_OUTPUTS_arm=$(CPP_SOURCES:./cpp/src/%.cpp=./cpp/lib/arm/%.o)
CPP_OUTPUTS_x64=$(CPP_SOURCES:./cpp/src/%.cpp=./cpp/lib/x64/%.o)

CPP_SOURCE_DIRS    =$(shell $(FIND) ./cpp/src/ -type d)
CPP_OUTPUT_DIRS_arm=$(CPP_SOURCE_DIRS:./cpp/src/%=./cpp/lib/arm/%) ./lib/arm
CPP_OUTPUT_DIRS_x64=$(CPP_SOURCE_DIRS:./cpp/src/%=./cpp/lib/x64/%) ./lib/x64

default: ./lib/Veer.jar ./lib/arm/libVeer.so $(CPP_OUTPUT_DIRS_arm) $(CPP_OUTPUT_DIRS_x64) $(CPP_SHARED_OUTPUT_x64)

# Subdirectory constructor
$(CPP_OUTPUT_DIRS_arm):
	mkdir -p $@

$(CPP_OUTPUT_DIRS_x64):
	mkdir -p $@

# Java build
./lib/Veer.jar: $(JAVA_OUTPUTS)
	jar cf $@ $(JAR_FILES)

./java/lib/%.class: ./java/src/%.java
	javac -cp $(JAR_DEPS) -d ./java/lib/ -sourcepath ./java/src/ $< -h ./cpp/include/generated/

# Native arm build
./lib/arm/libVeer.so: $(CPP_OUTPUTS_arm)
	$(ARM_CC) -shared $^ -o $@

./cpp/lib/arm/%.o: ./cpp/src/%.cpp
	$(ARM_CC) -c $(CFLAGS) $^ -o $@

# Native x64 build
$(CPP_SHARED_OUTPUT_x64): $(CPP_OUTPUTS_x64)
	$(CC) -shared $(LDFLAGS) $^ -o $@

./cpp/lib/x64/%.o: ./cpp/src/%.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
# Remove java and cpp outputs
	rm -rf ./java/lib/**
	rm -rf ./cpp/lib/arm/**
	rm -rf ./cpp/lib/x64/**
	rm -rf ./cpp/include/generated/**

	rm -f ./lib/Veer.jar
	rm -f ./lib/arm/libVeer.so
# Remove both the GNU/Linux and Windows shared library variants without complaining
	rm -f ./lib/x64/libVeer.so
	rm -f ./lib/x64/Veer.dll

	rm -rf ./logs/