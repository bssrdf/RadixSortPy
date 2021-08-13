from random import randint
from time import clock
import RadixSort as rs


if __name__=='__main__':
    for value in 1000, 10000, 100000, 1000000, 10000000:
        ls = [randint (1, value) for _ in range(value)]
        ls2 = list(ls)
        start = clock()
        rs.sortList(ls)
        end = clock()
        print("rs %d: %0.2fs" % (value, end-start))
        tdiff = end-start
        start = clock()
        ls2.sort()
        end = clock()
        print("qs %d: %0.2fs %0.2f%%" % (value, end-start, ((end-start)/tdiff*100)))
        last = -1
        for i in ls:
            assert last <= i
            last = i
        last = -1
        for i in ls2:
            assert last <= i
            last = i
        