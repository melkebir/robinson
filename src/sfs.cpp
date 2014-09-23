/*
 * sfs.cpp
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#include "sfs.h"
#include <iostream>
#include <algorithm>
#include <assert.h>

SFS::IntVector SFS::solve() const
{
  int n = _A.n();

  // construct identity permutation
  IntVector tau(n, 0), tau_inv(n, 0);
  for (int v = 0; v < n; ++v)
  {
    tau_inv[v] = tau[v] = v;
  }
  
  IntVector sigma_inv = sfs(tau_inv, tau);

  std::cout << "sigma_inv =";
  for (int i = 0; i < n; ++i)
  {
    std::cout << " " << sigma_inv[i];
  }
  std::cout << std::endl;

  std::reverse(sigma_inv.begin(), sigma_inv.end());

  // construct sigma
  IntVector sigma(n, 0);
  for (int v = 0; v < n; ++v)
  {
    sigma[sigma_inv[v]] = v;
  }

  IntVector sigma_plus_inv = sfs(sigma_inv, sigma);

  std::cout << "sigma_plus_inv =";
  for (int i = 0; i < n; ++i)
  {
    std::cout << " " << sigma_plus_inv[i];
  }
  std::cout << std::endl;

  std::reverse(sigma_plus_inv.begin(), sigma_plus_inv.end());

  // construct sigma_plus
  IntVector sigma_plus(n, 0);
  for (int v = 0; v < n; ++v)
  {
    sigma_plus[sigma_plus_inv[v]] = v;
  }

  IntVector pi_inv = sfs(sigma_plus_inv, sigma_plus);

  std::cout << "pi_inv =";
  for (int i = 0; i < n; ++i)
  {
    std::cout << " " << pi_inv[i];
  }
  std::cout << std::endl;
  
  return pi_inv;
}

SFS::IntVector SFS::sfs(const IntVector& tau_inv,
                        const IntVector& tau) const
{
  // tau_inv : mapped -> original
  // tau : original -> mapped
  const int n = _A.n();
  
  IntVector sigma_inv(n, -1);
  IntMatrix label(n, IntVector(n, 0));
  BoolVector visited(n, false);
  
  int p = tau_inv[0];
  sigma_inv[0] = p;
  visited[p] = true;
  for (int v = 0; v < n; ++v)
  {
    if (v != p)
    {
      label[v][0] = _A(p, v);
    }
  }
  
  for (int i = 1; i < n; ++i)
  {
    p = pivot(tau, label, visited, i);
    visited[p] = true;
    sigma_inv[i] = p;
    
    // update labels
    for (int v = 0; v < n; ++v)
    {
      if (!visited[v])
      {
        label[v][i] = _A(p, v);
      }
    }
  }
  
  return sigma_inv;
}

int SFS::pivot(const IntVector& tau,
               const IntMatrix& label,
               const BoolVector& visited,
               int i) const
{
  const int n = _A.n();
  int p = -1;
  
  IntVector S = slice(tau, label, visited, i-1);
  int j = i - 2;
  while (S.size() > 1 && j >= 0)
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
  
  if (S.size() == 1)
  {
    p = S.front();
  }
  else
  {
    assert(j == -1);
    // take first according to tau
    int m = static_cast<int>(S.size());
    int smallest_tau = n;
    for (int k = 0; k < m; ++k)
    {
      if (smallest_tau > tau[S[k]])
      {
        smallest_tau = tau[S[k]];
        p = S[k];
      }
    }
  }
  
  return p;
}

SFS::IntVector SFS::slice(const IntVector& tau,
                          const IntMatrix& label,
                          const BoolVector& visited,
                          int i) const
{
  const int n = _A.n();
  
  // first determine largest unvisited label
  int largest_label = -1;
  for (int v = 0; v < n; ++v)
  {
    if (!visited[v])
    {
      largest_label = std::max(largest_label, label[v][i]);
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
