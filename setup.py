from distutils.core import setup, Extension

ringbuf = Extension('ringbuf', sources=["src/ringbuf.c"])

setup(
    name = "Ring Buffer",
    version = "0.1",
    description = "A circular/ring buffer written in C",
    ext_modules = [ringbuf]
)
