using namespace std;

void pass_words(string file_address, string file_name, int NumberofLines, string text[]);
void table_data( string word, string file_address, string file_name, int parag);

class words;
class files;

time_t t;
int seed = time(&t);
words *hash_table[10000] = {0};

class words
{
	public:
		int repetition;
		char word_name[35];
		files* files_ptr;
		words* next_word;
};

class files
{
	public:
		int repetition;
		char address[50];
		char id[15];
		int parag_arr[200];
		int parag_i;
		files* next_file;
};

void table_data(string word, string file_address, string file_name, int parag)
{
	int cond = 0;
	words* ptr;
	words* previous_ptr;
	files* p;
	files* previous_p;
	files* current_p;
	int index = murmur3_32(&word[0], word.length(), seed) % 10000;
	ptr = hash_table[index];
	while((ptr != 0) && strcmp(ptr->word_name, &word[0]))
	{
		previous_ptr = ptr;
		ptr = ptr->next_word;
	}
	if(ptr == 0)
	{
		ptr = (words*) calloc(1, sizeof(words));
		ptr->next_word = 0;
		strcpy(ptr->word_name, &word[0]);
		ptr->repetition = 1;
		ptr->files_ptr = (files*)calloc(1, sizeof(files));
		ptr->files_ptr->next_file = 0;
		strcpy(ptr->files_ptr->address, &file_address[0]);
		strcpy(ptr->files_ptr->id, &file_name[0]);
		ptr->files_ptr->parag_i = 0;
		ptr->files_ptr->parag_arr[ptr->files_ptr->parag_i] = parag;
		ptr->files_ptr->repetition = 1;
		if(hash_table[index] == 0)
			hash_table[index] = ptr;
		else
			previous_ptr->next_word = ptr;
	}
	else
	{
		ptr->repetition += 1;
		current_p = ptr->files_ptr;
		while((current_p != 0) && (strlen(current_p->id) <= file_name.length()))
		{
			if((strlen(current_p->id) < file_name.length()))
			{
				cond = 1;
				previous_p = current_p;
				current_p = current_p->next_file;
			}
			else if(strcmp(current_p->id, &file_name[0]) < 0)
			{
				cond = 1;
				previous_p = current_p;
				current_p = current_p->next_file;
			}
			else
			{
				break;
			}
		}
		if(current_p == 0)
		{
			p = (files*) calloc(1, sizeof(files));
			p->next_file = 0;
			strcpy(p->address, &file_address[0]);
			strcpy(p->id, &file_name[0]);
			p->parag_i = 0;
			p->parag_arr[p->parag_i] = parag;
			p->repetition = 1;
			previous_p->next_file = p;
		}
		else if((strcmp(current_p->id, &file_name[0]) == 0))
		{
			if(current_p->parag_arr[current_p->parag_i] != parag)
			{
				current_p->parag_i += 1;
				current_p->parag_arr[current_p->parag_i] = parag;
			}
			current_p->repetition += 1;
		}
		else
		{
			p = (files*) calloc(1, sizeof(files));
			p->next_file = 0;
			strcpy(p->address, &file_address[0]);
			strcpy(p->id, &file_name[0]);
			p->parag_i = 0;
			p->parag_arr[p->parag_i] = parag;
			p->repetition = 1;
			if(cond)
			{
				previous_p->next_file = p;
				p->next_file = current_p;
			}
			else
			{
				ptr->files_ptr = p;
				p->next_file = current_p;				
			}
		}
	}
}

void pass_words(string file_address, string file_name, int NumberofLines, string text[])
{
	string Punctuation = " ";
	ifstream fin;
	fin.open(file_address.c_str());
	string line;
	string word;
	int parag = 1;
	int tmp = 0;
	char* tokenptr;
	int i = 0, j = 0, size = 0;
	for(i = 0; i < NumberofLines; i++)
	{
		if(text[i].length() == 0)
		{
			if(tmp)
			{
				tmp = 0;
				parag++;
			}
			continue;
		}
		tokenptr = strtok(&text[i][0], Punctuation.c_str());
		while(tokenptr != NULL)
		{
			word = tokenptr;
			//cout << word << " parag: " << parag << endl;
			table_data(word, file_address, file_name, parag);
			tokenptr = strtok(NULL, Punctuation.c_str());
		}
		tmp = text[i].length();
	}
	fin.close();
}

