#include "../include/libraries.h"
#include "../include/helper.h"
#include <iostream>
#define PORT 1212
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	int n_p = 0;
    int n_d = 0;
    cout<<"\nPlease input program size: ";
    cin>> n_p;
    cout<<"\nPlease input data size: ";
    cin>> n_d;
    if(n_p > 0 && n_d > 0)
    {
        Helper h(n_p, n_d, 1000);
	    h.Init();
    }
    else
        cout<<"\nInvalid input! Terminating..."<<endl;
        
	return 0; 
} 