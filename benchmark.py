from random import (randint, random)
from time import clock
import RadixSort as rs


if __name__=='__main__':
    for value in 1000, 10000, 100000, 1000000, 10000000, 50000000:
    #for value in [10]:
       # ls = [randint (-value//2, value//2) for _ in range(value)]
        ls = [random()*value-value/2.0 for v in range(value)]
        mi = min(ls)
        ls2 = list(ls)
        #print(ls)
        start = clock()
        rs.sortList(ls)
        end = clock()
        #print(ls)
        print("rs %d: %0.2fs" % (value, end-start))
        tdiff = end-start
        start = clock()
        ls2.sort()
        end = clock()
        #print(ls2)
        print("qs %d: %0.2fs %0.2f%%" % (value, end-start, ((end-start)/tdiff*100)))
        last = mi
        for i in ls:
            assert last <= i
            last = i
        last = mi
        for i in ls2:
            assert last <= i
            last = i
        