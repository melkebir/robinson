/*
 * matrix.cpp
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "matrix.h"

Matrix* Matrix::createRobinson(int n, int L)
{
  assert(n > 0);
  Matrix* pMatrix = new Matrix(n);
  
  // generate Robinsonian matrix
  std::random_device rd;
  std::mt19937 gen(rd());
  typedef std::uniform_int_distribution<int> uniform_dist;
  
  IntMatrix& A = pMatrix->_A;
  
  // initialize diagonal with L
  for (int i = 0; i < n; ++i)
  {
    A[i][i] = L;
  }
  
  // initialize first row
  for (int j = n - 1; j > 0; --j)
  {
    int UB = L - j * L / n;
    
    //    int val = uniform_dist(LB, UB)(gen);
    int val = uniform_dist(j < n - 1 ? A[0][j+1] : 0, UB)(gen);
    A[j][0] = A[0][j] = val;
  }
  
  for (int i = 1; i < n; ++i)
  {
    for (int j = n - 1; j > i; --j)
    {
      int UB = L - std::min(n - i - 1, j) * L / n;
      if (j == n - 1)
      {
        int val = uniform_dist(A[i-1][j], UB)(gen);
        A[j][i] = A[i][j] = val;
      }
      else
      {
        int LB = std::max(A[i][j+1], A[i-1][j]);
        int val = uniform_dist(LB, UB)(gen);
        A[j][i] = A[i][j] = val;
      }
    }
  }
  
  assert(pMatrix->isRobinson());
  return pMatrix;
}

Matrix* Matrix::createRobinsonian(int n, int L)
{
  Matrix* pMatrix = createRobinson(n, L);
  assert(pMatrix->isRobinson());
  
  pMatrix->shuffle();
  
  return pMatrix;
}

Matrix* Matrix::createNonRobinsonian(int n, int L)
{
  Matrix* pMatrix = createRobinson(n, L);
  pMatrix->_A[0][n - 1] = pMatrix->_A[n - 1][0] = L;
  
  pMatrix->shuffle();
  
  return pMatrix;
}

Matrix* Matrix::create(std::istream& in)
{
  int n = -1;
  in >> n;
  std::string line;
  // eat the new line character
  std::getline(in, line);
  
  if (n <= 0)
  {
    std::cerr << "Error: n should be at least 1" << std::endl;
    return NULL;
  }
  
  Matrix* pMatrix = new Matrix(n);
  

  for (int i = 0; i < n; ++i)
  {
    if (!in.good())
    {
      std::cerr << "Error: row " << i << " is missing" << std::endl;
      delete pMatrix;
      return NULL;
    }
    
    std::getline(in, line);
    std::stringstream ss(line);
    for (int j = 0; j < n; ++j)
    {
      int val = -1;
      ss >> val;

      if (val < 0)
      {
        std::cerr << "Error: nonnegative value expected at row " << i << " and column " << j << std::endl;
        delete pMatrix;
        return NULL;
      }
      
      pMatrix->_A[i][j] = val;
    }
  }
  
  if (!pMatrix->isSymmetric())
  {
    std::cerr << "Error: matrix is not symmetric" << std::endl;
    delete pMatrix;
    return NULL;
  }
  
  return pMatrix;
}

void Matrix::permute(const IntVector& perm)
{
  assert(perm.size() == _n);

  IntMatrix B(_n, IntVector(_n, 0));
  
  for (int i = 0; i < _n; ++i)
  {
    for (int j = 0; j < _n; ++j)
    {
      B[i][j] = _A[perm[i]][perm[j]];
    }
  }
  
  _A = std::move(B);
}

void Matrix::shuffle()
{
  IntVector perm(_n, 0);
  for (int i = 0; i < _n; ++i)
  {
    perm[i] = i;
  }
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::shuffle(perm.begin(), perm.end(), gen);
  
  permute(perm);
}

bool Matrix::isSymmetric() const
{
  for (int i = 0; i < _n; ++i)
  {
    for (int j = 0; j < i; ++j)
    {
      if (_A[i][j] != _A[j][i])
      {
        std::cerr << "A[" << i << "][" << j << "] = "
                  << _A[i][j] << " != " << "A[" << j << "][" << i
                  << "] = " << _A[j][i] << std::endl;
        return false;
      }
    }
  }
  
  return true;
}

bool Matrix::isRobinson() const
{
  if (!isSymmetric())
  {
    return false;
  }
  
  // check rows
  for (int i = 0; i < _n; ++i)
  {
    for (int j = i + 1; j < _n; ++j)
    {
      if (_A[i][j-1] < _A[i][j])
      {
        return false;
      }
    }
  }

  // check columns
  for (int j = 0; j < _n; ++j)
  {
    for (int i = j + 1; i < _n; ++i)
    {
      if (_A[i-1][j] < _A[i][j])
      {
        return false;
      }
    }
  }
  
  return true;
}

void Matrix::write(std::ostream& out) const
{
  out << _n << std::endl;
  for (int i = 0; i < _n; ++i)
  {
    bool first = true;
    
    for (int j = 0; j < _n; ++j)
    {
      if (!first)
      {
        out << " ";
      }
      else
      {
        first = false;
      }
      
      out << _A[i][j];
    }
    
    out << std::endl;
  }
}
