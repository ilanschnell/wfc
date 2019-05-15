Using Python as a C library
===========================

<i>Ilan Schnell - May 2019</i>


In this article, we want to show something unusual: Writing a simple
C program, which uses only the Python C library.

The Python/C API is very well documented and offers access to all of
Python's data types and thier functionality, such `list`, `dict`, etc. .
This C API is provided in order to allow users to write
Python C extension modules, which are basically behave like Python modules,
but are writen in C (instead of Python).
These C extensions are very important when interfacing Python with existing
C libraries, and when writing speed or memory critial libraries.
The Python standard library itself contains a large number of C extension
modules, which are an excellent resource for exploring how to write
C extensions yourself.  I wrote bitarray in 2008, and during the process
of writing this C extension, I found the `array` standard library extremely
useful.

Now, we want to write a pure C program which uses some of the functionality
exposed in the Python/C API, such as Python's data structures and
algorithms, but is not a Python C extension.  Instead this C program has
it's own `main()` function, and only uses the Python dictionary implementation
in order to count word frequencies in a text file.
The task of reading a text file and recognizing words separated by whitespace
is a not too hard to accompish in C.
Whenever a word is read, it is added to our dictionary, which maps words
to their frequency count.  This function looks like this:

    void add_word(PyObject *dict, char *str)
    {
        PyObject *value;
        long cnt;

        value = PyDict_GetItemString(dict, str);
        cnt = (value == NULL) ? 0 : PyLong_AsLong(value);
        cnt++;
        PyDict_SetItemString(dict, str, PyLong_FromLong(cnt));
    }

Obviously, we need to include `Python.h` and link to `libpython` to make
this work.  The entire program, and a build script which works on Linux
and MacOS can be found <a href="https://github.com/ilanschnell/wfc">here</a>.
