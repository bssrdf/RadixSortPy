#define PY_SSIZE_T_CLEAN
//#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <Python.h>


static int isFloat = 0;

void insertion_sort(uint32_t *array, int offset, int end) {
    int x, y;
    uint32_t temp;
    for (x=offset; x<end; ++x) {
        for (y=x; y>offset && array[y-1]>array[y]; y--) {
            temp = array[y];
            array[y] = array[y-1];
            array[y-1] = temp;
        }
    }
}

void insertion_sort64(uint64_t *array, int64_t offset, int64_t end) {
    int64_t x, y;
    uint64_t temp;
    for (x=offset; x<end; ++x) {
        for (y=x; y>offset && array[y-1]>array[y]; y--) {
            temp = array[y];
            array[y] = array[y-1];
            array[y-1] = temp;
        }
    }
}

void radix_sort(uint32_t *array, int offset, int end, int shift) {
    int x, y; 
    uint32_t value, temp;
    int last[256] = { 0 }, pointer[256];

    for (x=offset; x<end; ++x) {
        ++last[(array[x] >> shift) & 0xFF];
    }

    last[0] += offset;
    pointer[0] = offset;
    for (x=1; x<256; ++x) {
        pointer[x] = last[x-1];
        last[x] += last[x-1];
    }

    for (x=0; x<256; ++x) {
        while (pointer[x] != last[x]) {
            value = array[pointer[x]];
            y = (value >> shift) & 0xFF;
            while (x != y) {
                temp = array[pointer[y]];
                array[pointer[y]++] = value;
                value = temp;
                y = (value >> shift) & 0xFF;
            }
            array[pointer[x]++] = value;
        }
    }

    if (shift > 0) {
        shift -= 8;
        for (x=0; x<256; ++x) {
            temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
            if (temp > 64) {
                radix_sort(array, pointer[x] - temp, pointer[x], shift);
            } else if (temp > 1) {
                // std::sort(array + (pointer[x] - temp), array + pointer[x]);
                insertion_sort(array, pointer[x] - temp, pointer[x]);
            }
        }
    }
}


void radix_sort64(uint64_t *array, int64_t offset, int64_t end, int64_t shift) {
    int64_t x, y; 
    uint64_t value, temp;
    int64_t last[256] = { 0 }, pointer[256];

    for (x=offset; x<end; ++x) {
        ++last[(array[x] >> shift) & 0xFF];
    }

    last[0] += offset;
    pointer[0] = offset;
    for (x=1; x<256; ++x) {
        pointer[x] = last[x-1];
        last[x] += last[x-1];
    }

    for (x=0; x<256; ++x) {
        while (pointer[x] != last[x]) {
            value = array[pointer[x]];
            y = (value >> shift) & 0xFF;
            while (x != y) {
                temp = array[pointer[y]];
                array[pointer[y]++] = value;
                value = temp;
                y = (value >> shift) & 0xFF;
            }
            array[pointer[x]++] = value;
        }
    }

    if (shift > 0) {
        shift -= 8;
        for (x=0; x<256; ++x) {
            temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
            if (temp > 64) {
                radix_sort64(array, pointer[x] - temp, pointer[x], shift);
            } else if (temp > 1) {
                // std::sort(array + (pointer[x] - temp), array + pointer[x]);
                insertion_sort64(array, pointer[x] - temp, pointer[x]);
            }
        }
    }
}
/*
inline uint32_t FloatFlip(uint32_t f)
{
	uint32_t mask = -(int32_t)(f >> 31) | 0x80000000;
	return f ^ mask;
}

inline void FloatFlipX(uint32_t *f)
{
	uint32_t mask = -(int32_t)(*f >> 31) | 0x80000000;
	*f ^= mask;
}

// ================================================================================================
// flip a float back (invert FloatFlip)
//  signed was flipped from above, so:
//  if sign is 1 (negative), it flips the sign bit back
//  if sign is 0 (positive), it flips all bits back
// ================================================================================================
inline uint32_t IFloatFlip(uint32_t f)
{
	uint32_t mask = ((f >> 31) - 1) | 0x80000000;
	return f ^ mask;
}*/


inline uint32_t FloatFlip(uint32_t f)
{
	uint32_t mask = -(int32_t)(f >> 31) | 0x80000000;
	return f ^ mask;
}

inline void FloatFlipX(uint32_t *f)
{
	uint32_t mask = -(int32_t)(*f >> 31) | 0x80000000;
	*f ^= mask;
}

// ================================================================================================
// flip a float back (invert FloatFlip)
//  signed was flipped from above, so:
//  if sign is 1 (negative), it flips the sign bit back
//  if sign is 0 (positive), it flips all bits back
// ================================================================================================
inline uint64_t IFloatFlip(uint64_t f)
{
	uint64_t mask = ((f >> 63) - 1) | 0x8000000000000000;
	return f ^ mask;
}

inline void IFloatFlipX(uint64_t *f)
{
	uint64_t mask = ((*f >> 63) - 1) | 0x8000000000000000;
	*f ^= mask;
}

//This is the function that is called from your python code
static PyObject* RadixSort_sortList(PyObject* self, PyObject* args){

  PyObject * listObj;

  //The input arguments come as a tuple, we parse the args to get the various variables
  //In this case it's only one list variable, which will now be referenced by listObj
  if (! PyArg_ParseTuple( args, "O", &listObj))
    return NULL;

  //length of the list
  long length = PyList_Size(listObj);

  //iterate over all the elements
  long i;
  uint32_t* array;
  uint64_t* array64;
  double *farray, *fsorted; 
  isFloat = PyFloat_Check(PyList_GetItem(listObj, 0));
  if(isFloat) {
      farray = (double *)malloc(sizeof(double) * length);
      fsorted = (double *)malloc(sizeof(double) * length);
      array64 = (uint64_t*) farray;
  }
  else {
      array = (uint32_t *)malloc(sizeof(uint32_t) * length);
  }
  for(i = 0; i < length; i++){
    //get an element out of the list - the element is also a python objects
    PyObject* temp = PyList_GetItem(listObj, i);
    //we know that object represents an integer - so convert it into C long
    
    if (isFloat) {        
        farray[i] = PyFloat_AsDouble(temp);
        uint64_t fi = array64[i];
        uint64_t mask = -(int64_t)(fi >> 63) | 0x8000000000000000;
        array64[i] ^= mask;    
    }
    else{ 
        int elem = (int) PyLong_AsLong(temp);
        array[i] = (uint32_t)(elem + INT_MAX);
    }
    
  }
  

  if (isFloat){
      radix_sort64(array64, 0, length, 56); 
  }
  else{
      radix_sort(array, 0, length, 24); 
  }

  if (isFloat) {
      for(i = 0; i < length; i++){    
        IFloatFlipX(&array64[i]);
      }      
      memcpy(fsorted, array64, sizeof(double) * length);
  }

  for(i = 0; i < length; i++){    
    //PyList_SetItem(listObj, i, PyLong_FromLong((long)array[i]));
    if (isFloat){
        PyList_SetItem(listObj, i, Py_BuildValue("d", fsorted[i]));
    }
    else{
        int tmp = (int)(array[i]-INT_MAX);
        PyList_SetItem(listObj, i, Py_BuildValue("i", tmp));
    }
  }

  if(isFloat){
     free(array64);
     free(fsorted);
  }
  else{
     free(array);
  }
  
  //value returned back to python code - another python object
  //build value here converts the C long to a python integer  
  Py_INCREF(Py_None);
  return Py_None;
}

//This is the docstring that corresponds to our 'add' function.
static char RadixSort_docs[] =
    "sortList(list): sort elements of the list\n";

/* This table contains the relavent info mapping -
  <function-name in python module>, <actual-function>,
  <type-of-args the function expects>, <docstring associated with the function>
*/
static PyMethodDef RadixSort_funcs[] = {
    
    {"sortList", RadixSort_sortList, METH_VARARGS, "sort elements of the list"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef RadixSortmodule = {
    PyModuleDef_HEAD_INIT,
    "RadixSort",   /* name of module */
    RadixSort_docs, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    RadixSort_funcs
};

PyMODINIT_FUNC PyInit_RadixSort(void){
    
    return PyModule_Create(&RadixSortmodule);
}

