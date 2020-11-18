#include "../include/libraries.h"
#include "../include/client.h"
#include "../include/bob.h"
#include <thread>
#include <iostream>
#define PORT 1212
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	Bob *B = new Bob();
	char ip[] = "127.0.0.1";
	std::thread client_thread(&Bob::GoLive, B, ip, PORT);
	client_thread.join();
	delete B;
	return 0; 
} 
