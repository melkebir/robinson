/*
 * sfs.cpp
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#include "sfs.h"

SFS::IntVector SFS::solve() const
{
  int n = _A.n();

  // construct identity permutation
  IntVector perm(n, 0);
  for (int i = 0; i < n; ++i)
  {
    perm[i] = i;
  }
  
  return perm;
}

SFS::IntVector SFS::sfs(const IntVector& tau) const
{
  const int n = _A.n();
  
  IntVector sigma(n);
  IntMatrix label(n, IntVector(n, 0));
  
  int u = tau[0];
  for (int v = 0; v < n; ++v)
  {
    if (v != u)
    {
      label[v][0] = _A(u, v);
    }
  }
  
  for (int i = 1; i < n; ++i)
  {
    int p = pivot(tau, label, visited, i);
    visited[p] = true;
    sigma[i] = p;
  }
  
  return sigma;
}

int SFS::pivot(const IntVector& tau,
               const IntMatrix& label,
               const BoolVector& visited,
               int i) const
{
  const int n = _A.n();
  int p = -1;
  
  for (int v = 0; v < n; ++v)
  {
    IntVector S = slice(tau, label, visited, i-1);
    if (S.size() == 1)
    {
      p = S.front();
    }
    else
    {
      int j = i - 2;
      while (S.size() > 1)
      {
        if (j == 0)
        {
          // take first according to tau
          for (int k = 0; k < static_cast<int>(S.size()); ++k)
          {
          }
        }
        else
        {
          // determine items to remove from S by finding largest label at j
          int largest_label = -1;
          for (int k = 0; k < static_cast<int>(S.size()); ++k)
          {
            largest_label = std::max(label[S[k]][j], largest_label);
          }
          
          // only retain items in S with largest_label
          IntVector newS;
          for (int k = 0; k < static_cast<int>(S.size()); ++k)
          {
            if (label[S[k]][j] == largest_label)
            {
              newS.push_back(S[k]);
            }
          }
          S = newS;
          --j;
        }
      }
    }
  }
  
  return p;
}

SFS::IntVector SFS::slice(const IntMatrix& label,
                          const BoolVector& visited,
                          int i) const
{
  const int n = _A.n();
  
  // first determine largest label
  int largest_label = -1;
  for (int v = 0; v < n; ++v)
  {
    if (!visited[v])
    {
      if (largest_label < label[v][i])
      {
        largest_label = label[v][i];
      }
    }
  }
  
  // now determine slice
  IntVector slice;
  for (int v = 0; v < n; ++v)
  {
    if (!visited[v] && label[v][i] == largest_label)
    {
      slice.push_back(v);
    }
  }
  
  return slice;
}
