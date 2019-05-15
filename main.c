/*
   Author: Ilan Schnell
*/

#include "Python.h"


/* command line options */
int summary;

#define USAGE  printf("usage: %s [-sh] [file] ...\n", argv[0])


/* add `str` to `dict`, and increase its value by 1 (or set its value
   to 1 initially) */
void add_word(PyObject *dict, char *str)
{
    PyObject *value;
    long cnt;

    value = PyDict_GetItemString(dict, str);
    cnt = (value == NULL) ? 0 : PyLong_AsLong(value);
    cnt++;
    PyDict_SetItemString(dict, str, PyLong_FromLong(cnt));
}


/* process the file pointer, and add all words (separated by whitespace)
   to the dictionary */
int process_fp(PyObject *dict, FILE *fp)
{
    char s[256];
    int c, i = 0;

    while (1) {
        c = fgetc(fp);
        if (c == 9 || c == 10 || c == 13 || c == 32 || c == EOF) {
            s[i] = '\0';
            if (i)
                add_word(dict, s);
            i = 0;
            if (c == EOF)
                break;
        } else {
            s[i++] = c;
        }
        if (i > 255) {
            printf("Buffer too small.\n");
            return -1;
        }
    }
    return 0;
}


int process_file(PyObject *dict, char *filename)
{
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Can't open `%s` for reading.\n", filename);
        return -1;
    }
    if (process_fp(dict, fp) < 0) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}


void display(PyObject *dict)
{
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    long i, total = 0;

    while (PyDict_Next(dict, &pos, &key, &value)) {
        i = PyLong_AsLong(value);
        if (!summary)
            printf("%s: %ld\n", PyUnicode_AsUTF8(key), i);
        total += i;
    }
    if (summary) {
        printf("Unique words: %ld\n", PyDict_Size(dict));
        printf("Total words: %ld\n", total);
    }
}


void help(void)
{
    printf("\n\
Calculates the frequency of words in file(s) or stdin.\n\
Options:\n\
  -s   only display summary instead of word count table\n\
  -h   display this help and exit\n");
}


/* changes the global variables: table, names, full */
void process_options(int argc, char *argv[])
{
    int op;

    summary = 0;
    while ((op = getopt(argc, argv, "hs")) != -1)
        switch(op) {
        case 'h':
            USAGE;
            help();
            exit(0);
        case 's':
            summary = 1;
            break;
        default:
            USAGE;
            exit(1);
        }
}


int main(int argc, char *argv[])
{
    PyObject *dict;

    process_options(argc, argv);

    Py_Initialize();
    dict = PyDict_New();
    if (dict == NULL)
        goto error;

    if (argc == optind) {
        if (process_fp(dict, stdin) < 0)
            goto error;
    }
    else {
        while (optind < argc)
            if (process_file(dict, argv[optind++]) < 0)
                goto error;
    }

    display(dict);

    Py_DECREF(dict);
    Py_Finalize();
    return 0;

 error:
    Py_XDECREF(dict);
    Py_Finalize();
    return 1;
}
