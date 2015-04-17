=========
pyringbuf
=========

A python extension implementing a circular/ring buffer of characters as a C
extension. It overwrites silently.

Availability
------------

Currently, pyringbuf is available at https://pypi.python.org/pypi/pyringbuf,
or via ``pip install pyringbuf``. As this is a C extensions, there is a
compilation step, so your system will need to be able to compile extension
modules for python.

Usage
-----

::

    >>> from ringbuf import RingBuffer
    >>> R = RingBuffer(5)    #choose your buffer size
    >>> R.push("a")          #push a single character into the buffer
    >>> R.pop()              #pop a single character
    'a'
    >>> R.write("bcdef")     #fill buffer with many characters at once
    >>> R.read(4)            #read many characters at once
    'bcde'
    >>> R.read(1)
    'f'
    >>> R.read(1)            #returns an empty string if the buffer is empty 
    ''

Implementation
--------------

For the interested, this has been implemented as a pure C extension. The ring
buffer itself is implemented as a fixed length array of chars (8-bit signed),
dynamically allocated on the Python Heap on instatiation of a buffer. Read and
write indexes both increase monotonically, and addresses in the array are
determined using /index/ modulo /buffer size/. Writing more than the buffer
size at once throws an exception, otherwise if writing overtakes read the read
index, then the buffer is silently overwritten.
