## A module to sort lists of 32-bit signed integers using radix sort algorithm

### Sorting 100000, 1000000 and 10000000 integers, Radix Sort outperforms python's built-in Tim-Sort by 5x to 10x
           
  N          |    100000     |  1000000   |  10000000
 ----        |    -----      |  ------    |  --------
rs           |    0.01s      |  0.08s     |   0.92s
ts (timsort) |    0.04s      |  0.63s     |   9.78s  
ts slower by |    534.18%    |  826.61%   |   1065.91%

