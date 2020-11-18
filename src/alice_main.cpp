#include "../include/libraries.h"
#include "../include/server.h"
#include "../include/alice.h"
#include <thread>
#define PORT 1212
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	Alice *A = new Alice();;
	std::thread server_thread(&Alice::GoLive, A, PORT);
	server_thread.join();
	delete A;
	return 0; 
} 
