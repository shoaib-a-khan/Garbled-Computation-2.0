#include <iostream>
#include <fstream>
#include "../include/preprocessor.h"

// #define S_PARAM 1000 		//Range of Randoms

Preprocessor::Preprocessor(std::string prog_path, std::string data_path)
{
  p_A_path = "../inputs/alice/";
  p_B_path = "../inputs/bob/";
  p_path = prog_path;
  std::string prog_filename = prog_path.substr(prog_path.find_last_of("/\\") + 1);
  std::string::size_type const p(prog_filename.find_last_of('.'));
  std::string prog_filename_without_extension = prog_filename.substr(0, p);
  p_A_path.append(prog_filename_without_extension + "_A.txt");
  p_B_path.append(prog_filename_without_extension + "_B.txt");

  d_A_path = "../inputs/alice/";
  d_B_path = "../inputs/bob/";
  d_path = data_path;
  std::string data_filename = data_path.substr(data_path.find_last_of("/\\") + 1);
  std::string::size_type const q(data_filename.find_last_of('.'));
  std::string data_filename_without_extension = data_filename.substr(0, q);
  d_A_path.append(data_filename_without_extension + "_A.txt");
  d_B_path.append(data_filename_without_extension + "_B.txt");
  
}

Preprocessor::~Preprocessor()
{
}

void Preprocessor::Init()
{
  std::ifstream P, D;
  std::ofstream P_A, P_B, D_A, D_B;
  P.open(p_path.c_str());
  P_A.open(p_A_path.c_str());
  P_B.open(p_B_path.c_str());

  if (P.fail())
  {
    std::cout << "Preprocessor: Unable to locate input program file at: "<<p_path<<std::endl;
    P.clear();
    exit(1);
  }
  if (P_A.fail())
  {
    std::cout << "Preprocessor: Unable to create output program file at: "<<p_A_path<<std::endl;
    P_A.clear();
    exit(1);
  }
  if (P_B.fail())
  {
    std::cout << "Preprocessor: Unable to create output program file at: "<<p_B_path<<std::endl;
    P_B.clear();
    exit(1);
  }

  int read = 0, write = 0;
  int r = 0;
  int count = 0;
  bool eol = 0;
  srand(time(NULL));

  while (P >> read)
  {
    eol = ++count % 5;
    r = rand() % S_PARAM;
    read = read - r;
    P_A << read << ((eol == 0) ? "\n" : " ");
    P_B << r << ((eol == 0) ? "\n" : " ");
  }
  P.close();
  P_A.close();
  P_B.close();

  count = 0;
  eol = 0;
  D.open(d_path.c_str());
  D_A.open(d_A_path.c_str());
  D_B.open(d_B_path.c_str());
  
  if (D.fail())
  {
    std::cout << "Preprocessor: Unable to locate input data file at: "<<d_path<<std::endl;
    D.clear();
    exit(1);
  }
  if (D_A.fail())
  {
    std::cout << "Preprocessor: Unable to create output data file at: "<<d_A_path<<std::endl;
    D_A.clear();
    exit(1);
  }
  if (D_B.fail())
  {
    std::cout << "Preprocessor: Unable to create output data file at: "<<d_B_path<<std::endl;
    D_B.clear();
    exit(1);
  }
  
  while (D >> read)
  {
    eol = ++count % 2;
    r = rand() % S_PARAM;
    read = read - r;
    D_A << read << ((eol == 0) ? "\n" : " ");
    D_B << r << ((eol == 0) ? "\n" : " ");
  }
  D.close();
  D_A.close();
  D_B.close();
}
