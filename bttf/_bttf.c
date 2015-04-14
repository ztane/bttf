#include <Python.h>
#include "bytesformat.h"
// #include "typehacks.h"

#define GETSTATE(m) ((struct bttf_module_state*)PyModule_GetState(m))
#define BTTF_DOC "Brings some of the magic from the future to the past and from the past back to the future."


#define BTTF_FEATURES(X) \
    X(bytes_mod)

//    X(dict_methods)

typedef struct bttf_module_state {
#define X(i) int i ## _installed;
BTTF_FEATURES(X)
#undef X
} bttf_module_state;

static PyModuleDef bttf_module;


/*********************** bytesformat hack ************************/

static PyNumberMethods bytes_as_number = {
    0,              /*nb_add*/
    0,              /*nb_subtract*/
    0,              /*nb_multiply*/
    bytes_mod,      /*nb_remainder*/
};

static PyObject *
bytes_mod_install(PyObject *self, PyObject *args) {
    if (! PyBytes_Type.tp_as_number) {
        PyBytes_Type.tp_as_number = &bytes_as_number;
        PyType_Modified(&PyBytes_Type);
    }
    Py_RETURN_NONE;
}

/*****************************************************************/


/******************** dictionary methods hack ********************/

/*static PyObject *
PyDict_iterkeys(PyObject *self, PyObject *args) {
    return PyObject_GetIter(self);
}


static PyMethodDef PyDictExtra[] = {
    { "iterkeys", PyDict_iterkeys, METH_NOARGS,
        "D.iterkeys() -> an iterator over the keys of D" },
    {NULL}
};

static PyObject *
dict_methods_install(PyObject *self, PyObject *args) {
    add_methods(&PyDict_Type, PyDictExtra);
    PyType_Modified(&PyDict_Type);
    Py_RETURN_NONE;
}
*/

/*****************************************************************/


static PyObject *
install(PyObject *self, PyObject *arg) {
    PyCFunction installer = NULL;
    int *status_flag = NULL;
    bttf_module_state *state = GETSTATE(self);

    if (! PyUnicode_Check(arg)) {
        PyErr_SetString(PyExc_TypeError, "install: argument must be a `str`");
        return NULL;
    }

#define X(i)                                              \
    if (PyUnicode_CompareWithASCIIString(arg, #i) == 0) { \
        status_flag = &(state->i ## _installed);          \
        installer = i ## _install;                        \
    }
BTTF_FEATURES(X)
#undef X

    if (! installer) {
        return PyErr_Format(PyExc_ValueError, "install: unknown feature %R", arg);
    }

    if (! *status_flag) {
        *status_flag = 1;
        return installer(self, arg);
    }
    Py_RETURN_NONE;
}


static PyMethodDef bttf_module_methods[] = {
    {"install", install, METH_O,
     "Install features"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};


static PyModuleDef bttf_module = {
    PyModuleDef_HEAD_INIT,
    "_bttf",
    BTTF_DOC,
    sizeof(struct bttf_module_state),
    bttf_module_methods,
    NULL,
    NULL,
    NULL,
    NULL
};


PyMODINIT_FUNC
PyInit__bttf(void)
{
    PyObject* m;

    m = PyModule_Create(&bttf_module);
    if (!m) {
        return NULL;
    }

    return m;
}
