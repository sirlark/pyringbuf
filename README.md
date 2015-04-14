# pyringbuf
A python extension implementing a circular/ring buffer of characters as a C extension. I overwrites silently.

##Usage:

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
