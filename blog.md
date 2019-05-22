Using Python as a C library
===========================

<i>Ilan Schnell - May 2019</i>


In this article, we want to show something unusual: how to write a simple
C program that uses only the Python C library.

The Python/C API is very well documented and offers access to all of
Python's data types and their functionality, such as `list`, `dict`, etc. .
Python provides this C API in order to allow users to write
Python C extension modules, which basically behave like Python modules
but are writen in C rather than in Python.

These C extensions are very important when interfacing Python with existing
C libraries and when writing speed- or memory-critical libraries.
The Python standard library itself contains a large number of C extension
modules, which are an excellent resource for exploring how to write
C extensions.  When I wrote bitarray in 2008, I found studying
the `array` standard library extremely useful.

Now we want to write a pure C program that uses some of the functionalities
exposed in the Python/C API, such as Python's data structures and
algorithms, but is not a Python C extension.  Instead this C program has
its own `main()` function and only uses the Python dictionary implementation
to count word frequencies in a text file.
The task of reading a text file and recognizing words separated by whitespace
is not too hard to accompish in C.
Whenever a word is read, it is added to a dictionary, which maps words
according to their frequency count.  This function looks like this:

    void add_word(PyObject *dict, char *str)
    {
        PyObject *value;
        long cnt;

        value = PyDict_GetItemString(dict, str);
        cnt = (value == NULL) ? 0 : PyLong_AsLong(value);
        cnt++;
        PyDict_SetItemString(dict, str, PyLong_FromLong(cnt));
    }

We need to include `Python.h` and a link to `libpython` to make this work.
The entire program, and a build script that works on Linux and MacOS,
can be found <a href="https://github.com/ilanschnell/wfc">here</a>.

Obviously, we could have easily written a program for this particular task
in pure Python without having to worry about any C code at all.  But this is
not the point of this exercise.  The point is to show how the Python C
library can be used directly within a C program (without writing an entire
C extension).

I hope you've enjoyed this article, and maybe learned something new and useful.
