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
  
  return solve(tau_inv, tau);
}

SFS::IntVector SFS::solveEnumPivot() const
{
  int n = _A.n();

  for (int v = 0; v < n; ++v)
  {
    IntVector tau_inv, tau(n, 0);
    tau_inv.push_back(v);
    for (int vv = 0; vv < n; ++vv)
    {
      if (v != vv)
        tau_inv.push_back(vv);
    }
    
    // construct tau
    for (int vv = 0; vv < n; ++vv)
    {
      tau[tau_inv[vv]] = vv;
    }
    
    IntVector res_perm_inv = solve(tau_inv, tau);
    Matrix B = _A;
    B.permute(res_perm_inv);
    std::cout << (B.isRobinson() ? "Robinson" : "NOT Robinson") << std::endl;
  }
  
  return IntVector();
}

SFS::IntVector SFS::solve(const IntVector& tau_inv,
                          const IntVector& tau) const
{
  int n = _A.n();

  assert(Matrix::isPermutation(tau_inv));
  std::cout << "tau_inv =";
  for (int i = 0; i < n; ++i)
  {
    std::cout << " " << tau_inv[i];
  }
  std::cout << std::endl;
  
  IntVector sigma_inv = sfs(tau_inv, tau);
  assert(Matrix::isPermutation(sigma_inv));

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
  assert(Matrix::isPermutation(sigma_plus_inv));
  
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
  assert(Matrix::isPermutation(pi_inv));

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
  IntVector label(n, 0);
  BoolVector visited(n, false);
  
  IntVector order = tau_inv;
  IntVector inv_order = tau;
  
  int p = tau_inv[0];
  sigma_inv[0] = p;
  visited[p] = true;
  label[p] = std::numeric_limits<int>::min();
  for (int v = 0; v < n; ++v)
  {
    if (v != p)
    {
      label[v] = _A(p, v);
    }
  }
  
  for (int i = 1; i < n; ++i)
  {
    int m = n - i + 1;
    // update order
    std::sort(order.begin(), order.begin() + m, Comparison(inv_order, label));
    // update inv_order
    for (int v = 0; v < m; ++v)
    {
      inv_order[order[v]] = v;
    }
    
    p = order[0];
    visited[p] = true;
    sigma_inv[i] = p;
    label[p] = std::numeric_limits<int>::min();
    
    // update labels
    for (int v = 0; v < m; ++v)
    {
      int vv = order[v];
      if (!visited[vv])
      {
        label[vv] = _A(p, vv);
      }
    }
  }
  
  return sigma_inv;
}
