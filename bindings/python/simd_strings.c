#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <simdstr/search.h>

#include <stdio.h>

static PyObject *
py_simd_strstr(PyObject *self, PyObject *args)
{
        const char* haystack;
        const char* needle;
        Py_ssize_t haystack_size;
        Py_ssize_t needle_size;

        if (!PyArg_ParseTuple(args, "s#s#", &haystack, &haystack_size, &needle, &needle_size))
        {
                return NULL;
        }

        const char* result = simd_generic_search_32 (haystack, haystack_size, needle, needle_size, -1, -1);

        Py_ssize_t match_position = (result == NULL) ? -1 : (result - haystack);

        return Py_BuildValue("n", match_position);
}

static PyMethodDef module_methods[] = {
        {"find", py_simd_strstr, METH_VARARGS, "Find position of first match of needle in haystack"},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef simdstr_module = {
        PyModuleDef_HEAD_INIT,
        "simd_strings",
        NULL,
        -1,
        module_methods
};

PyMODINIT_FUNC PyInit_simdstr(void)
{
        return PyModule_Create (&simdstr_module);
}