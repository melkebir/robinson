/*
 * main.cpp
 *
 *  Created on: 22-sep-2014
 *      Author: M. El-Kebir
 */

#include <iostream>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>
#include "matrix.h"
#include "sfs.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Usage instructions")
    ("max-entry,L", po::value<int>(), "Maximum entry")
    ("solve,s", po::value<std::string>(), "Robinson similarity recognition")
    ("generate,g", po::value<int>(), "Generate Robinson matrix");

  po::variables_map vm;
  try
  {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch (po::error& e)
  {
    std::cerr << "Error: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  }

  if (vm.count("help"))
  {
    std::cout << desc << "\n";
    return 1;
  }
  else if (vm.count("generate") && vm.count("max-entry"))
  {
    int n = vm["generate"].as<int>();
    int L = vm["max-entry"].as<int>();
    
    Matrix* pMatrix = Matrix::create(n, L);
    
    pMatrix->write(std::cout);

    delete pMatrix;
  }
  else if (vm.count("solve"))
  {
    Matrix* pMatrix = NULL;
    
    std::string filename = vm["solve"].as<std::string>();
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
      SFS sfs(*pMatrix);
      SFS::IntVector pi_inv = sfs.solve();
      pMatrix->permute(pi_inv);
      if (pMatrix->isRobinson())
      {
        std::cout << "Matrix is Robinsonian" << std::endl;
        pMatrix->write(std::cout);
      }
      else
      {
        std::cout << "Matrix is not Robinsonian" << std::endl;
        pMatrix->write(std::cout);
      }
    }

    delete pMatrix;
  }
  
  return 0;
}
