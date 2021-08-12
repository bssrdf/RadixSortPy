from distutils.core import setup, Extension

setup(name='RadixSort', version='0.1',  \
      ext_modules=[Extension('RadixSort', ['radixsortpy.c'])])