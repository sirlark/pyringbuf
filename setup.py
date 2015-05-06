from setuptools import setup, Extension

ringbuf = Extension('ringbuf', sources=["src/ringbuf.c"])

setup(
    name = "pyringbuf",
    version = "0.1b2",
    description = "A circular/ring buffer written in C",
    ext_modules = [ringbuf],
    url = "https://github.com/sirlark/pyringbuf",
    author = "James Dominy",
    author_email = "jgdominy+pyringbuf@gmail.com",
    license = "MIT",
    classifiers = [
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: C",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: Implementation :: CPython"
    ],
    keywords = "circular ring buffer",
    test_suite = "test.tests"
)
