// This is the internal Python 3.4 add_methods function from
// https://hg.python.org/cpython/file/3.4/Objects/typeobject.c
// that we use to add methods to existing types

static int
add_methods(PyTypeObject *type, PyMethodDef *meth)
{
    PyObject *dict = type->tp_dict;

    for (; meth->ml_name != NULL; meth++) {
        PyObject *descr;
        int err;
        if (PyDict_GetItemString(dict, meth->ml_name) &&
            !(meth->ml_flags & METH_COEXIST))
                continue;
        if (meth->ml_flags & METH_CLASS) {
            if (meth->ml_flags & METH_STATIC) {
                PyErr_SetString(PyExc_ValueError,
                     "method cannot be both class and static");
                return -1;
            }
            descr = PyDescr_NewClassMethod(type, meth);
        }
        else if (meth->ml_flags & METH_STATIC) {
          PyObject *cfunc = PyCFunction_NewEx(meth, (PyObject*)type, NULL);
            if (cfunc == NULL)
                return -1;
            descr = PyStaticMethod_New(cfunc);
            Py_DECREF(cfunc);
        }
        else {
            descr = PyDescr_NewMethod(type, meth);
        }
        if (descr == NULL)
            return -1;
        err = PyDict_SetItemString(dict, meth->ml_name, descr);
        Py_DECREF(descr);
        if (err < 0)
            return -1;
    }
    return 0;
}
