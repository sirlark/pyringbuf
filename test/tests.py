import unittest
from ringbuf import RingBuffer

class TestRingBuffer(unittest.TestCase):
    def test_push_pop(self):
        R = RingBuffer(5)
        R.push("1")
        self.assertEqual(R.pop(), "1")

    def test_push_push_pop_pop(self):
        R = RingBuffer(5)
        R.push("1")
        R.push("2")
        self.assertEqual(R.pop()+R.pop(), "12")

    def test_write_read(self):
        R = RingBuffer(5)
        R.write("abcd")
        self.assertEqual(R.read(4), "abcd")

    def test_wrapping_push_pop(self):
        R = RingBuffer(3)
        R.push("1")
        R.push("2")
        R.push("3")
        self.assertEqual(R.pop(), "1")
        R.push("a")
        self.assertEqual(R.read(2), "23")
        self.assertEqual(R.pop(), "a")

    def test_wrapping_write_read(self):
        R = RingBuffer(5)
        R.write("abcd")
        self.assertEqual(R.read(1), "a")
        R.write("e1")
        self.assertEqual(R.read(3), "bcd")

    def test_wrapping_write_wrapping_read(self):
        R = RingBuffer(5)
        R.write("abcd")
        self.assertEqual(R.read(1), "a")
        R.write("e1")
        self.assertEqual(R.read(5), "bcde1")

    def test_wrapping_overwriting_push_pop(self):
        R = RingBuffer(3)
        R.push("1")
        R.push("2")
        R.push("3")
        R.push("a")
        self.assertEqual(R.pop(), "a")

    def test_push_pop_pop(self):
        R = RingBuffer(5)
        R.push("1")
        self.assertEqual(R.pop(), "1")
        self.assertEqual(R.pop(), "")

    def test_write_read_read(self):
        R = RingBuffer(5)
        R.write("abcd")
        self.assertEqual(R.read(4), "abcd")
        self.assertEqual(R.read(2), "")
        
    def test_write_read_read(self):
        R = RingBuffer(5)
        with self.assertRaises(ValueError):
            R.write("abcdef")


