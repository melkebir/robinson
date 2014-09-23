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
  
protected:
  IntVector sfs(const IntVector& tau_inv,
                const IntVector& tau) const;
  
  int pivot(const IntVector& tau,
            
            const IntMatrix& label,
            const BoolVector& visited,
            int i) const;
  
  IntVector slice(const IntVector& tau,
                  const IntMatrix& label,
                  const BoolVector& visited,
                  int i) const;
  
protected:
  const Matrix& _A;
};

#endif // SFS_H
