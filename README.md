## A module to sort lists of 32-bit signed integers and 64-bit floating point numbers using radix sort algorithm

### Sorting 100000, 1000000 and 10000000 integers, Radix Sort outperforms python's built-in Tim-Sort by 5x to 10x
           
  N          |    100000     |  1000000   |  10000000
 ----        |    -----      |  ------    |  --------
rs           |    0.01s      |  0.08s     |   0.92s
ts (timsort) |    0.04s      |  0.63s     |   9.78s  
ts slower by |    534.18%    |  826.61%   |   1065.91%


### Sorting 1000000, 10000000 and 50000000 doubles, Radix Sort outperforms python's built-in Tim-Sort by 6x to 15x
           
  N          |    1000000     |  10000000   |  50000000
 ----        |    -----      |  ------    |  --------
rs           |    0.08s      |  0.71s     |   3.89s
ts (timsort) |    0.49s      |  8.55s     |   59.91s  
ts slower by |    638.89%    |  1199.22%   |  1540.28%

