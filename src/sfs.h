/*
 * sfs.h
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#ifndef SFS_H
#define SFS_H

#include "matrix.h"
#include <vector>

class SFS
{
public:
  typedef Matrix::IntVector IntVector;
  typedef Matrix::IntMatrix IntMatrix;
  typedef std::vector<bool> BoolVector;
  
public:
  SFS(const Matrix& A)
    : _A(A)
  {
  }
  
  IntVector solve() const;
  IntVector solveEnumPivot() const;
  
protected:
  IntVector sfs(const IntVector& tau_inv,
                const IntVector& tau) const;
  
  IntVector solve(const IntVector& tau_inv,
                  const IntVector& tau) const;

  struct Comparison
  {
  private:
    const IntVector& _currentOrder;
    const IntVector& _newLabel;
    const BoolVector& _currentSlice;
    
  public:
    Comparison(const IntVector& currentOrder,
               const IntVector& newLabel,
               const BoolVector& currentSlice)
      : _currentOrder(currentOrder)
      , _newLabel(newLabel)
      , _currentSlice(currentSlice)
    {
    }
    
    bool operator ()(int a, int b) const
    {
      if (_currentSlice[a] && !_currentSlice[b])
      {
        return true;
      }
      else if (!_currentSlice[a] && _currentSlice[b])
      {
        return false;
      }
      else if (_newLabel[a] > _newLabel[b])
      {
        return true;
      }
      else if (_newLabel[a] < _newLabel[b])
      {
        return false;
      }
      else
      {
        return _currentOrder[a] < _currentOrder[b];
      }
    }
  };
  
protected:
  const Matrix& _A;
};

#endif // SFS_H
