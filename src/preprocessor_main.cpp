#include "../include/libraries.h"
#include "../include/preprocessor.h"
#include <iostream>
#define PORT 1212
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	
	string prog_path, data_path;
	cout<<"\nPlease enter Program Path: ";
	getline(std::cin, prog_path);
	cout<<"\nPlease enter Data Path: ";
	getline(std::cin, data_path);
	Preprocessor p(prog_path, data_path);
	p.Init();
	return 0; 
} 
