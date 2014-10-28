Robinsonian similarity
======================

This package provides an *O(n^2 log n)* time algorithm for recognizing
whether a matrix is Robinsonian. In case the input matrix is
Robinsonian, it returns the corresponding permutation.

Dependencies
------------

* CMake
* C++ compiler

Build instructions
------------------

    mkdir build
    cd build
    cmake ..
    make

Run instructions
----------------

```
% help
./robinson -h 

% generate Robinsonian matrix

./robinson -g 100 -L 5000

./robinson -g 100 -L 5000 > /tmp/matrix.txt

./robinson -s /tmp/matrix.txt

for i in {1..1000};do echo -n "$i ";./robinson -g 100 -L 5000 | ./robinson -s -;done
```
