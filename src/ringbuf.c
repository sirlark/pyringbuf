#include <Python.h>

typedef struct {
    PyObject_HEAD
    Py_ssize_t size;
    Py_ssize_t read_idx;
    Py_ssize_t write_idx;
    char *buffer;
} RingBuffer;

static PyObject *RingBuffer_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"size", NULL};
    RingBuffer *self;
    Py_ssize_t buf_size;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &buf_size)) return NULL;
	self = (RingBuffer *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->size = buf_size;
		self->buffer = (char *)PyMem_Malloc(self->size);
		if (self->buffer == NULL) {
			Py_DECREF(self);
			return PyErr_NoMemory();
		}
		self->read_idx = 0;
		self->write_idx = 0;
	}
	return (PyObject *)self;
}

static int RingBuffer_init(RingBuffer *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"size", NULL};
    Py_ssize_t buf_size;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &buf_size)) return -1;
	if ((self->buffer)&&(self->size != buf_size)) {
		PyMem_Free(self->buffer);
		self->buffer = NULL;
	}
	self->size = buf_size;
	if (!self->buffer) {
		self->buffer = malloc(self->size);
		if (self->buffer == NULL) {
			//TODO: Set an exception?
			return -1;
		}
	}
	self->read_idx = 0;
	self->write_idx = 0;
	return 0;
}

static void RingBuffer_dealloc(RingBuffer *self) {
    PyMem_Free(self->buffer);
	self->buffer = NULL;
    self->ob_type->tp_free((PyObject *)self);
}

static PyObject *RingBuffer_push(RingBuffer *self, PyObject *args) {
	char c;

	if (!PyArg_ParseTuple(args, "c", &c)) {
		PyErr_SetString(PyExc_ValueError, "c must be a string of length 1");
		return NULL;
	}
	//TODO: Check for full buffer
	self->buffer[self->write_idx%self->size] = c;
	self->write_idx++;
	Py_RETURN_NONE;
}

static PyObject *RingBuffer_pop(RingBuffer *self) {
	PyObject *tmp = NULL;
    if (self->read_idx >= self->write_idx) {
		//TODO: raise a Buffer empty error
		tmp = Py_BuildValue("s", "");
    } else {
		tmp = Py_BuildValue("c", self->buffer[self->read_idx%self->size]);
		self->read_idx++;
	}
	return tmp;
}

static PyObject *RingBuffer_write(RingBuffer *self, PyObject *args) {
	char *s;
    int s_len;
	Py_ssize_t write_idx;

	if (PyArg_ParseTuple(args, "s#", &s, &s_len)) {
		if (s_len > self->size) {
			PyErr_SetString(PyExc_ValueError, "s is larger than buffer size");
			return NULL;
		}
		//TODO: Check for full buffer
		write_idx = self->write_idx%self->size;
		if (write_idx + s_len > self->size) {
			strncpy(self->buffer+write_idx, s, self->size-write_idx);
			strncpy(self->buffer, s+(self->size-write_idx), s_len-(self->size-write_idx));
		} else {
			strncpy(self->buffer+write_idx, s, s_len);
		}
		self->write_idx += s_len;
		Py_RETURN_NONE;
	} else {
		PyErr_SetString(PyExc_ValueError, "s must be a string");
		return NULL;
	}
}

static PyObject *RingBuffer_read(RingBuffer *self, PyObject *args) {
	PyObject *tmp = NULL;
	char *s;
	int n;
	Py_ssize_t read_idx;
	if (PyArg_ParseTuple(args, "i", &n)) {
		if (self->read_idx+n > self->write_idx) {
			//TODO: raise a Buffer empty error
			tmp = Py_BuildValue("s", "");
		} else {
			read_idx = self->read_idx%self->size;
			s = malloc(n+1);
			if (read_idx + n > self->size) {
				strncpy(s, self->buffer+read_idx, self->size-read_idx);
				strncpy(s+(self->size-read_idx), self->buffer, n-(self->size-read_idx));
			} else {
				strncpy(s, self->buffer+read_idx, n);
			}
			s[n] = 0;
			self->read_idx += n;
			tmp = Py_BuildValue("s", s);
			free(s);
		}
		return tmp;
	} else {
		PyErr_SetString(PyExc_ValueError, "n must be a string");
		return NULL;
	}
}

static PyMethodDef RingBuffer_methods[] = {
    {"push", (PyCFunction)RingBuffer_push, METH_VARARGS, "b.push(c) -> None: Push a single character into the buffer"},
    {"pop", (PyCFunction)RingBuffer_pop, METH_NOARGS, "b.pop() -> str: Pop a single character from the buffer"},
    {"write", (PyCFunction)RingBuffer_write, METH_VARARGS, "b.write(s) -> None: Pushes multiple characters into the buffer at once"},
    {"read", (PyCFunction)RingBuffer_read, METH_VARARGS, "b.read(n) -> str: Extracts n characters from the buffer at once"},
    {NULL}  /* Sentinel */
};

static PyTypeObject RingBufferType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "ringbuf.RingBuffer",      /*tp_name*/
    sizeof(RingBuffer),        /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)RingBuffer_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "RingBuffer objects",      /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    RingBuffer_methods,        /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)RingBuffer_init, /* tp_init */
    0,                         /* tp_alloc */
    RingBuffer_new,            /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC initringbuf(void) {
    PyObject* m;

    if (PyType_Ready(&RingBufferType) < 0) return;

    m = Py_InitModule3("ringbuf", module_methods, "A circular/ring buffer written in C");

    if (m == NULL) return;

    Py_INCREF(&RingBufferType);
    PyModule_AddObject(m, "RingBuffer", (PyObject *)&RingBufferType);
    //(void) Py_InitModule("ringbuf", module_methods);
};
