// Client side C/C++ program to demonstrate Socket programming
#include "libraries.h"
#include "server.h"
#include "client.h"
#include "helper.h"
#include "alice.h"
#include "bob.h"
#include <thread>
#include <iostream>
#define PORT 1212
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	
	// Helper::LoadNextRandomPermutation(0, 16);
	// Helper::LoadNextRandomPermutation(2, 9);
	// Helper::LoadNextRandomPermutation(0, 16);
	
	// Helper::GenerateShuffleVectors(16, 9, 1000);
	// Helper::GenerateRandomPermutations(16, 9, 1000);
	// Helper::GeneratePermutedVectors(16, 9, 1000);

	// Helper::GenerateCorrelatedRandoms(1000);
	Helper h(16, 9, 10000);
	h.Init();
	// Alice *A = new Alice();;
	// Bob *B = new Bob();
	// char ip[] = "127.0.0.1";
	// std::thread server_thread(&Alice::GoLive, A, PORT);
	// std::thread client_thread(&Bob::GoLive, B, ip, PORT);
	
	// server_thread.join();
	// client_thread.join();
	// delete A;
	// delete B;
	return 0; 
} 

/*#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

int main() //rw vectors from files
{
	// std::vector<std::string> example;
    // example.push_back("this");
    // example.push_back("is");
    // example.push_back("a");
    // example.push_back("test");

    // std::ofstream output_file("./example.txt");
    // std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    // std::copy(example.begin(), example.end(), output_iterator);

	// std::ifstream is("Randoms_B.txt");
	// std::istream_iterator<long> start(is), end;
	// std::vector<long> numbers(start, end);
	// std::cout << "Read " << numbers.size() << " numbers" << std::endl;
	// for(int i=0; i<numbers.size(); i++)
	// 	cout<<numbers[i]<<" ";
	// cout<<endl;
	// //print the numbers to stdout
	// std::cout << "numbers read in:\n";
	// std::copy(numbers.begin(), numbers.end(),
	// 		  std::ostream_iterator<long>(std::cout, " "));
	// std::cout << std::endl;
}*/