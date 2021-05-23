#ifndef DICTIONARY_H
#define DICTIONARY_H
#include<string>
#include<vector>
#include<iostream>
#include "hash24.h"

class Dictionary{
private:

	struct slot {
		std::vector<std::string> secondary_datas;  
		Hash24 secondary_hash;
		int secondary_size; 
		int word_num; 
		slot(); 
		~slot();
	};
private:
	std::vector<slot> primary_datas;
	int tsize; 
	Hash24 primary_hash; 

	
	bool generate_secondary_hash(std::vector<std::string> & secondary_datas, Hash24 & secondary_hash, int & secondary_size, bool type);
public:
	Dictionary(); 
	Dictionary(std::string fname, int tsize); 
	~Dictionary();  

	bool find(std::string word); 
	void writeToFile(std::string fName); 
	static Dictionary readFromFile(std::string fName); 
};
#endif
