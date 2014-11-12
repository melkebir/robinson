/*
 * permute.cpp
 *
 *  Created on: 11-nov-2014
 *      Author: M. El-Kebir
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "matrix.h"

typedef std::vector<int> IntVector;

bool parsePermStr(const std::string& permStr,
                  IntVector& permVector)
{
  permVector.clear();

  std::stringstream ss(permStr);
  while (ss.good())
  {
    int val = -1;
    ss >> val;
    if (val >= 0)
    {
      permVector.push_back(val);
    }
    else
    {
      std::cerr << "Value '" << val << "' must be nonnegative" << std::endl;
      return false;
    }
  }
  return true;
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    return 1;
  }

  std::string filename(argv[1]);
  std::string permStr(argv[2]);

  Matrix* pMatrix = NULL;
  if (filename == "-")
  {
    pMatrix = Matrix::create(std::cin);
  }
  else
  {
    std::ifstream inFile(filename.c_str());

    if (!inFile.good())
    {
      std::cerr << "Error: could not open file '"
                << filename << "' for reading" << std::endl;
      return 1;
    }
    pMatrix = Matrix::create(inFile);
  }

  IntVector perm;
  if (!parsePermStr(permStr, perm))
  {
    return 1;
  }
  else if (perm.size() != pMatrix->n())
  {
    std::cerr << "Invalid dimension of perm vector" << std::endl;
    return 1;
  }
  else if (!Matrix::isPermutation(perm))
  {
    std::cerr << "Not a valid permutation vector" << std::endl;
    return 1;
  }

  pMatrix->permute(perm);
  pMatrix->write(std::cout);

  if (pMatrix->isRobinson())
  {
    std::cout << "Robinson" << std::endl;
  }
  else
  {
    std::cout << "Not Robinson" << std::endl;
  }

  return 0;
}
