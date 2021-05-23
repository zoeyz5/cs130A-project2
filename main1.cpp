#include <iostream>

#include "Dictionary.h"

using namespace std;

int main(int arg, char* argv[])
{
	Dictionary theDictionary(argv[1],16000);
	theDictionary.writeToFile(argv[2]);       
	return 0;
}
