#include <fstream> 

#include "Dictionary.h"

using namespace std;

bool Dictionary::generate_secondary_hash(std::vector<std::string> & secondary_datas, Hash24 & secondary_hash, int & secondary_size, bool type)
{
	secondary_size = secondary_datas.size()*secondary_datas.size(); 
	vector<string> get_secondary_datas(secondary_size,""); 
	if (type)
	{
		Hash24 new_hash_data;
		secondary_hash = new_hash_data;
	}

	for (int i = 0; i < secondary_datas.size();i++)
	{

		int get_hash_num = secondary_hash.hash(secondary_datas[i])% secondary_size;

		if (get_secondary_datas[get_hash_num] == "")
		{
			get_secondary_datas[get_hash_num] = secondary_datas[i];
		}
		else
		{
			return false;
		}
	}

	secondary_datas = get_secondary_datas; 
	return true;
}

Dictionary::Dictionary()
{
	tsize = 0;
}

Dictionary::Dictionary(std::string fname, int tsize)
{
	int all_words_num = 0;
	primary_datas = vector<slot>(tsize); 
	vector<int> collisions(21,0); 
	vector<int> rand_num(20,0);
	this->tsize = tsize;   
	int max_collisions_posi = 0;       
	int max_collisions_num = 0;       


	int the_primary_num;
	ifstream infile;   
	infile.open(fname.c_str(), ios::in);
	string middata;
	infile >> middata;  
	while (!infile.eof())            
	{
		all_words_num++;
		the_primary_num = (primary_hash.hash(middata))% tsize; 
		primary_datas[the_primary_num].secondary_datas.push_back(middata); 
		infile >> middata;
	}
	infile.close();


	for (int i = 0; i < primary_datas.size(); i++)
	{

		if (primary_datas[i].secondary_datas.size() > max_collisions_num)
		{
			max_collisions_num = primary_datas[i].secondary_datas.size(); 
			max_collisions_posi = i; 
		}
		if ((primary_datas[i].secondary_datas.size()>=0)&&(primary_datas[i].secondary_datas.size() <= 20))
		{
			collisions[primary_datas[i].secondary_datas.size()]++; 
		}
	}

	for (int i = 0; i < primary_datas.size(); i++)
	{
		primary_datas[i].word_num = primary_datas[i].secondary_datas.size();
		
		if (primary_datas[i].secondary_datas.size()>1)
		{
			int try_num = 0;
			if (!generate_secondary_hash(primary_datas[i].secondary_datas, primary_datas[i].secondary_hash, primary_datas[i].secondary_size, false))
			{
				try_num++;
				while (!generate_secondary_hash(primary_datas[i].secondary_datas, primary_datas[i].secondary_hash, primary_datas[i].secondary_size, true))
				{
					try_num++;
				}
			}
			
			if (try_num>0)
			{
				rand_num[try_num - 1]++;
			}
		}
	}

	


	primary_hash.dump(); 
	
	cout << "Number of words = " << all_words_num<< endl;
	
	cout << "Table size = "<< tsize<<endl;
	
	cout << "Max collisions = " << endl;
	
	for (int i = 0; i < 21;i++)
	{
		cout << "# of primary slots with "<<i<<" words = " << collisions[i]<<endl;
	}
	
	cout << "** Words in the slot with most collisions ***" << endl;
	for (int i = 0; i < primary_datas[max_collisions_posi].secondary_datas.size();i++)
	{
		if (primary_datas[max_collisions_posi].secondary_datas[i]!="")
		{
			cout << primary_datas[max_collisions_posi].secondary_datas[i] << endl;
		}
	}
	
	for (int i = 0; i < 20; i++)
	{
		cout << "# of secondary hash tables trying "<<i+1<<" hash functions = " << rand_num[i] << endl;
	}
	long all_num = 0;
	long all_num_2 = 0;
	for (int i = 2; i < 21; i++)
	{
		all_num_2 += collisions[i];
	}
	for (int i = 0; i < 20; i++)
	{
		all_num += rand_num[i];
	}
	cout << "Average # of hash functions tried = " <<(double)(all_num)/ all_num_2 <<endl;
}

Dictionary::~Dictionary()
{
}

bool Dictionary::find(std::string word)
{
	int primary_num = primary_hash.hash(word)% tsize; 
	if (primary_datas[primary_num].secondary_datas.size() == 0)
	{
		return false;
	}
	else if (primary_datas[primary_num].secondary_datas.size() == 1)
	{
		if (word == primary_datas[primary_num].secondary_datas[0])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		int secondary_num = primary_datas[primary_num].secondary_hash.hash(word)%(primary_datas[primary_num].secondary_size);
		if (word == primary_datas[primary_num].secondary_datas[secondary_num])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void Dictionary::writeToFile(std::string fName)
{

	ofstream outfile;   
	outfile.open(fName, ios::out);
	if (!outfile.is_open())
		cout << "Open file failure" << endl;
	else
	{
		
		outfile << tsize<<endl;
		
		outfile << primary_hash.get_random_a() << " ";
		outfile << primary_hash.get_random_b() << " ";
		outfile << primary_hash.get_random_c() << endl;
		
		for (int i = 0; i < primary_datas.size(); i++)
		{
			
			if (primary_datas[i].secondary_datas.size()>1)
			{
				outfile << 0 << " ";
				outfile << i<<" "; 
				outfile << primary_datas[i].secondary_size << " "; 
				
				outfile << primary_datas[i].secondary_hash.get_random_a() << " ";
				outfile << primary_datas[i].secondary_hash.get_random_b() << " ";
				outfile << primary_datas[i].secondary_hash.get_random_c() << " ";
				
				outfile << primary_datas[i].word_num << " ";
				for (int j = 0; j < primary_datas[i].secondary_datas.size(); j++)
				{
					
					if (primary_datas[i].secondary_datas[j] != "")
					{
						outfile << j << " "; 
						outfile << primary_datas[i].secondary_datas[j] << " ";
					}
				}
				outfile << endl;
			}
			else if (primary_datas[i].secondary_datas.size()==1)
			{
				outfile << 1 << " ";
				outfile << i << " "; 
				outfile << primary_datas[i].secondary_datas[0]<< " "; 
				outfile << endl;
			}
		}
	}
	outfile.close();
}

Dictionary Dictionary::readFromFile(std::string fName)
{
	Dictionary one_Dictionary; 
	ifstream infile;   
	infile.open(fName.c_str(), ios::in);

	infile >> one_Dictionary.tsize;  
	one_Dictionary.primary_datas = vector<slot>(one_Dictionary.tsize); 
	long mid_get_random;

	infile >> mid_get_random; 
	one_Dictionary.primary_hash.set_random_a(mid_get_random);
	infile >> mid_get_random;
	one_Dictionary.primary_hash.set_random_b(mid_get_random);
	infile >> mid_get_random;
	one_Dictionary.primary_hash.set_random_c(mid_get_random);

	slot mid_slot; 
	int slot_posi; 
				   
	int slot_type;
	infile >> slot_type;
	if (slot_type == 0)
	{
		infile >> slot_posi;
		infile >> mid_slot.secondary_size; 
		mid_slot.secondary_datas = vector<string>(mid_slot.secondary_size);
		
		infile >> mid_get_random;
		mid_slot.secondary_hash.set_random_a(mid_get_random);
		infile >> mid_get_random;
		mid_slot.secondary_hash.set_random_b(mid_get_random);
		infile >> mid_get_random;
		mid_slot.secondary_hash.set_random_c(mid_get_random);

		infile >> mid_slot.word_num;
		for (int all_word_flag = 0; all_word_flag < mid_slot.word_num; all_word_flag++)
		{
			int the_word_posi;
			string the_word_data;
			infile >> the_word_posi;
			infile >> the_word_data;
			mid_slot.secondary_datas[the_word_posi] = the_word_data;
		}
	}
	else if (slot_type == 1)
	{
		infile >> slot_posi;
		string the_word_data;
		infile >> the_word_data;
		mid_slot.secondary_datas.clear();
		mid_slot.secondary_datas.push_back(the_word_data); 
	}
	while (!infile.eof())           
	{
		one_Dictionary.primary_datas[slot_posi] = mid_slot;
		
		int slot_type;
		infile >> slot_type;
		
		if (infile.eof())
		{
			break;
		}

		if (slot_type == 0)
		{
			infile >> slot_posi;
			infile >> mid_slot.secondary_size; 
			mid_slot.secondary_datas = vector<string>(mid_slot.secondary_size);

			infile >> mid_get_random;
			mid_slot.secondary_hash.set_random_a(mid_get_random);
			infile >> mid_get_random;
			mid_slot.secondary_hash.set_random_b(mid_get_random);
			infile >> mid_get_random;
			mid_slot.secondary_hash.set_random_c(mid_get_random);

			infile >> mid_slot.word_num;
			for (int all_word_flag = 0; all_word_flag <  mid_slot.word_num; all_word_flag++)
			{
				int the_word_posi;
				string the_word_data;
				infile >> the_word_posi;
				infile >> the_word_data;
				mid_slot.secondary_datas[the_word_posi] = the_word_data;
			}
		}
		else if (slot_type == 1)
		{
			infile >> slot_posi;
			string the_word_data;
			infile >> the_word_data;
			mid_slot.secondary_datas.clear();
			mid_slot.secondary_datas.push_back(the_word_data); 
		}
	}
	infile.close();
	return one_Dictionary;  
}

Dictionary::slot::slot()
{
	secondary_size = 0;
}

Dictionary::slot::~slot()
{
}
