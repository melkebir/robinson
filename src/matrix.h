/*
 * matrix.h
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <ostream>
#include <istream>
#include <random>

class Matrix
{
public:
  typedef std::vector<int> IntVector;
  typedef std::vector<IntVector> IntMatrix;
  
protected:
  Matrix(int n)
    : _n(n)
    , _A(n, IntVector(n, 0))
  {
  }
  
protected:
  const int _n;
  IntMatrix _A;
  
public:
  static Matrix* create(int n, int L);
  static Matrix* create(std::istream& in);
  
  void permute(const IntVector& perm);
  void shuffle();
  
  bool isSymmetric() const;
  bool isRobinsonian() const;
  
  void write(std::ostream& out) const;
  
  int operator()(int i, int j) const { return _A[i][j]; }
  
  int n() const { return _n; };
};

#endif // MATRIX_H
