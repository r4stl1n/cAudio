//****************************************************************
//cAudio 2.3.0 Tutorial 8
//Custom Manager Event Handler
//****************************************************************

#include <iostream>
#include <string>

//Include the custom handler
#include "cTestManager.h"

using namespace std;

int main(int argc, char* argv[])
{
	//Some fancy text
	cout << "cAudio 2.3.0 Tutorial 8: Custom Manager Event Handler. \n \n";
	
	//Create our background audio thread then forget about it.
    // onUpdate() callbacks will be triggered and handled without this
    // main thread's knowledge
    cTestManager *manager = new cTestManager();

	std::cout << "Press any key to quit \n";
	std::cin.get();
	std::cin.get();
    
    delete(manager);

    return 0;
}
