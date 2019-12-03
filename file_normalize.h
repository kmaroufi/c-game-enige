using namespace std;

void file_normalize(string address, int NumberofLines, string text[]);
void word_normalize(string& word);
void lowwercase_change(string& word);
void delete_stop_word(string& word);
int number_of_lines(string address);

void file_normalize(string address, int NumberofLines, string text[])
{
	string Punctuation = " !@#$%^&*(),./?><':;][}{|-_`\"~\\		";
	ifstream fin;
	string line;
	string word;
	char* tokenptr;
	int i = 0, j = 0, size = 0;
	fin.open(address.c_str());
	for(i = 0; i < NumberofLines; i++)
	{
		getline(fin, line);
		text[i] = line;	
	}
	line = "";
	for(i = 0; i < NumberofLines; i++)
	{
		tokenptr = strtok(&text[i][0], Punctuation.c_str());
		while(tokenptr != NULL)
		{
			word = tokenptr;
			word_normalize(word);
			tokenptr = strtok(NULL, Punctuation.c_str());
			if(word.length())
			{
				if(tokenptr != NULL)
					line += word + " ";
				else
					line += word;
			}
		}
		text[i] = line;
		line = "";
	}
	fin.close();
//	ofstream fout;
//	fout.open("test_out.txt");
//	for(i = 0; i < NumberofLines; i++)
//	{
//		fout << text[i];
//		fout << endl;
//	}
//	fout.close();
}

void word_normalize(string& word)
{
	lowwercase_change(word);
	delete_stop_word(word);
	if(word.length() != 0)
	Porter2Stemmer::stem(word);
}

void lowwercase_change(string& word)
{
	int i = 0;
	for(i = 0; i < word.length(); i++)
		if((65 <= word[i]) && (word[i] <= 90))
			word[i] += 32;
}

void delete_stop_word(string& word)
{
	string stop_word;
	ifstream fin;
	fin.open("stop_words.txt");
	while(!fin.eof())
	{
		fin >> stop_word;
		if(stop_word == word)
		{
			word.erase();
			break;
		}
	}
	fin.close();
}

int number_of_lines(string address)
{
	string line;
	ifstream fin;
	fin.open(address.c_str());
	int Num = 0;
	while(!fin.eof())
	{
		getline(fin, line);
		Num++;
	}
	fin.close();
	return Num;
}

