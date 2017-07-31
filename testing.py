import subprocess
import unittest

def diff(command):
    command1 = ["./" + command[0]] + command[1:]
    command2 = ["./ref/linux64/" + command[0]] + command[1:]
    build = subprocess.Popen(command1, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ref = subprocess.Popen(command2, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out1, err1 = build.communicate()
    out2, err2 = ref.communicate()
    return out1, out2, err1, err2, build.returncode, ref.returncode

class TestStringMethods(unittest.TestCase):

    def test_compilation_error(self):
        out1, out2, err1, err2, c1, c2 = diff(["TextMiningCompiler", "invalid.txt", "foo.out"])
        self.assertEqual(out1 == out2)
        self.assertEqual(err1 == err2)
        self.assertEqual(c1 == c2)

    def test_compilation1(self):
        out1, out2, err1, err2, c1, c2 = diff(["TextMiningCompiler", "tests/test.txt", "test.out"])
        self.assertEqual(out1 == out2)
        self.assertEqual(err1 == err2)
        self.assertEqual(c1 == c2)

    def test_compilation2(self):
        out1, out2, err1, err2, c1, c2 = diff(["TextMiningCompiler", "tests/words.txt", "test.out"])
        self.assertEqual(out1 == out2)
        self.assertEqual(err1 == err2)
        self.assertEqual(c1 == c2)

"""
    def test_dist0_1(self):
    def test_dist0_2(self):
    def test_dist0_3(self):
    def test_dist0_4(self):
    def test_dist0_5(self):
    def test_dist0_6(self):

    def test_dist1_1(self):
    def test_dist1_2(self):
    def test_dist1_3(self):
    def test_dist1_4(self):
    def test_dist1_5(self):
    def test_dist1_6(self):
"""

if __name__ == '__main__':
    unittest.main()
