#!/usr/bin/python

import os
import re
import subprocess
import unittest

testDir  = os.path.dirname(os.path.abspath(__file__))
fangFile = os.path.dirname(testDir) + "/fang"

class TestFang(unittest.TestCase):

    def testHelp(self):
        output = subprocess.check_output([fangFile, "-h"])
        self.assertTrue(output.startswith("usage: fang "), "'" + output + " does not start with 'usage: fang '")

    def testFindInCurrentDirectory(self):
        os.chdir(testDir + "/data/sxe")
        paths = subprocess.check_output([fangFile]).strip().split("\n")
        pathSet = set(paths)
        self.assertIn("./libsxe", pathSet)

    def testFindInDirectory(self):
        os.chdir(testDir)
        paths = subprocess.check_output([fangFile, "data/sxe"]).strip().split("\n")
        pathSet = set(paths)
        self.assertIn("data/sxe/libsxe", pathSet)
        self.assertNotIn("test_fang.py", pathSet)

    def testFindWithExt(self):
        os.chdir(testDir + "/data/sxe")
        paths = subprocess.check_output([fangFile, "-e", ".h"]).strip().split("\n")

        for path in paths:
            self.assertTrue(path.endswith(".h"), "fang -e .h: '%s' doesn't end with '.h'" % path)

        pathSet = set(paths)
        self.assertIn("./mak/bin/t/libev/ev.h", pathSet)
        self.assertNotIn("./libsxe",            pathSet)

    def testFindWithFilePat(self):
        os.chdir(testDir)
        paths  = subprocess.check_output([fangFile, "-f", "lib.*"]).strip().split("\n")
        libPat = re.compile(r'lib.*')

        for path in paths:
            self.assertTrue(libPat.match(os.path.basename(path)),
                            "fang -f lib.*: '%s' doesn't match /lib.*/" % os.path.basename(path))

        pathSet = set(paths)
        self.assertIn("./data/sxe/mak/bin/t/libev", pathSet)
        self.assertNotIn("data/sxe/mak",            pathSet)

    def testFindAndGrepWithExt(self):
        os.chdir(testDir)
        paths = subprocess.check_output([fangFile, "-e", ".h", "data/sxe", "typedef"]).strip().split("\n")
        pathSet = set(paths)
        self.assertIn("data/sxe/mak/bin/t/libev/ev.h:48:typedef double ev_tstamp;", pathSet)

    def testFindAndGrepNoNumbers(self):
        os.chdir(testDir)
        paths = subprocess.check_output([fangFile, "-e", ".h", "-N", "data/sxe", "typedef"]).strip().split("\n")
        pathSet = set(paths)
        self.assertIn("data/sxe/mak/bin/t/libev/ev.h:typedef double ev_tstamp;", pathSet)



if __name__ == '__main__':
    unittest.main()
