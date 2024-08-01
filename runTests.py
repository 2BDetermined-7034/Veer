#!/usr/bin/env python3

import os
import subprocess
import sys

tests = os.listdir("./java/lib/tbd/veer/test/")

testsPassed = 0

subprocess.call("mkdir -p ./logs", shell=True)

for file in tests:
    test = os.path.splitext(file)[0]

    with open("./logs/" + test + ".out", "w") as out, open("./logs/" + test + ".err", "w") as err:
        status = subprocess.call("java -cp ./lib/Veer.jar -Djava.library.path=./lib/x64/ tbd.veer.test." + test, shell=True, stdout=out, stderr=err)
        if (status == 0):
            print(test + ": \033[32mSUCCESS\033[0m")
            testsPassed += 1
        else:
            print(test + ": \033[31mFAILED\033[0m ({})".format(status))

        out.seek(0, os.SEEK_END)
        if (out.tell() == 0):
            os.remove("./logs/" + test + ".out")

        err.seek(0, os.SEEK_END)
        if (err.tell() == 0):
            os.remove("./logs/" + test + ".err")

print("Tests passed: " + str(testsPassed))
print("Tests failed: " + str(len(tests) - testsPassed))