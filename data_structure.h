#include"porter2_stemmer.h"
#include"porter2_stemmer.cpp"
#include"murmur.h"
#include"file_normalize.h"
#include"dirent.h"
#include"table_data2.h"
using namespace std;

void print_data();
void find_word(string input);
void data_structure();

void data_structure()
{
	string folder_address;
	string file_address;
	string folder_name;
	string file_name;
	string add = ".\\files\\";
	string tmp1;
	cout << "be MIB search engine khosh amadid." << endl;
	cout << "sakhte shode tavasote Kamyar Maroufi & Farzad Bolourchi Fard." << endl;
	cout << "dar ebteda, folderhaii ke havie file hast ro dar kenare file main dar dakhele pooshe files berizid." << endl;
	cout << "zamani ke an ra anjam dadid, baraye edame, dokmeii ra be delkhah az safheye kelid feshard dahid." << endl;
	getch();
	cout << "data structure is loading. Please wait!" << endl;
	DIR *folders_dir;
	struct dirent *ent1;
	DIR *files_dir;
	struct dirent *ent2;
	folders_dir = opendir (add.c_str());
	ent1 = readdir (folders_dir);
	ent1 = readdir (folders_dir);
	while((ent1 = readdir (folders_dir)) != NULL)
	{
			folder_name = ent1->d_name;
			folder_address = add + folder_name;
			files_dir = opendir(folder_address.c_str());
			folder_address = folder_address + "\\";
			ent2 = readdir (files_dir);
			ent2 = readdir (files_dir);
			while((ent2 = readdir (files_dir)) != NULL)
			{
				file_name = ent2->d_name;
				file_address = folder_address + file_name;
				int NumberofLines = number_of_lines(file_address.c_str());
				string text[NumberofLines];
				file_normalize(file_address.c_str(), NumberofLines, &text[0]);
				pass_words(file_address.c_str(), file_name.c_str(), NumberofLines, &text[0]);
				//print_data();
			}
			closedir(files_dir);
			add = ".\\files\\";
	}
	closedir (folders_dir);
}


void print_data()
{
	int i = 0, j = 0;
	words* ptr;
	files* p;
	ofstream fout;
	fout.open("data.txt");
	fout << left << setw(25) << "\"word\"" <<  left << setw(25) << "\"repetition\"" << left << setw(25) <<  "\"id\"" << endl;
	for(i = 0; i < 10000; i++)
	{
		ptr = hash_table[i];
		while(ptr != 0)
		{
			fout << left << setw(25) << ptr->word_name << left << setw(5) <<ptr->repetition;
			p = ptr->files_ptr;
			while(p != 0)
			{
				fout << left << setw(10) << p->id;
				p = p->next_file;
			}
			fout << endl;
			ptr = ptr->next_word;
		}
	}
	fout.close();
}


