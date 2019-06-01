#ifdef _WIN32
#include "wincolor.hpp"
#endif
#ifdef linux
#include "linuxcolor.hpp"
#endif
#include <Python.h>

static PyObject * Py_setcolor(PyObject *self, PyObject *args){
    int color_code;
    const char* color_name;
    PyObject* color = PyTuple_GetItem(args, 0);
    int is_long = PyObject_IsInstance(color, (PyObject*)&PyLong_Type);
    if(is_long)
    {
        PyArg_Parse(color, "i", &color_code);
        return PyBool_FromLong(setcolor(color_code));
    }
    else if(PyArg_Parse(color, "s", &color_name))
    {
        return PyBool_FromLong(set_color_with_words(color_name));
    }
    return PyBool_FromLong(0);
}

static PyObject * Py_get_current_color(PyObject *self, PyObject *args){
#ifdef _WIN32
    return (PyObject*) Py_BuildValue("i", get_current_color());
#endif
#ifdef linux
    return (PyObject*) Py_BuildValue("s", get_current_color());
#endif
}

static PyMethodDef jshl_Py_methods[] = {
    {"setcolor", Py_setcolor, METH_VARARGS},
    {"get_current_color", Py_get_current_color, METH_VARARGS},
    {nullptr, nullptr},
};

static struct PyModuleDef color_module =
{
    PyModuleDef_HEAD_INIT,
    "_color",
    "",
    -1,
    jshl_Py_methods
};

PyMODINIT_FUNC PyInit__color(void){
    return PyModule_Create(&color_module);
}