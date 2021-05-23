#include <iostream>
#include <sstream>
#include "Dictionary.h"

using namespace std;

int main(int arg, char* argv[])
{
	if (arg != 3)
	{
		cout << "Please input right arguments"<<endl;
		return 0;
	}
	Dictionary theDictionary = Dictionary::readFromFile(argv[1]);  
	string find_data = argv[2];
	int n = find_data.size();
	for (int i = 0; i < n; ++i) {
		if (find_data[i] == ',') {
			find_data[i] = ' ';
		}
	}
	istringstream out(find_data);
	string one_word;
	while (out >> one_word)
	{
		cout << one_word;
		if (theDictionary.find(one_word))
		{
			cout << " found";
		}
		else
		{
			cout << " not found";
		}
		cout << endl;
	}
	return 0;
}