using namespace std;

class id_trail;
class boolean_trie;
words* word_place(string input);
template<typename T, typename U>
id_trail* and_words(T p1, U p2);
template<typename T, typename U>
id_trail* or_words(T p1, U p2);
id_trail* not_words(id_trail* p1, id_trail* p2);

void find_word(string input);
id_trail* find_input();
id_trail* convert(string input);
int input_trie(string& input, int level, boolean_trie* previous_level, int start, int end);
int check_parantez(const string& input, int pos);
int subset_num(int level, int strat, int end);
void arr_initialize(const string& input);
int check_down(int strat, int end);
int cond_place(int level, int i, int status, int start, int end);
id_trail* calc(boolean_trie* p);
string spell_check(string request);
bool is_more_than_one_word(string input);

int arr[3][1000] = {0};

class id_trail
{
	public:
		char id[15];
		id_trail* next_file;	
};

class boolean_trie
{
	public:
		boolean_trie* parts;
		char word[35];
		int number;
		int before_cond;
};

id_trail* find_input()
{
	char tmp;
	int tmp1 = 0, i = 0, cond = 0;
	string input = "komajdoon";
	string word;
	string line = "";
	string tmp2 = "AND";
	string tmp3 = "";
	string sabet = "AND";
	cout << "data structure load shod. hala shoma mitavanid be search bepardazid." << endl;
	cout << "ghabl az shorooe jost o joo, be nokat zir tavajoh konid:" << endl;
	cout << "1) shoma mitavanid az kalamate kelidie OR ya AND ya NOT beiene kalamte made nazaretoon estefade konid." << endl;
	cout << "2) agar beine 2 kalame az OR ya AND ya NOT estefade nakonid, be tore pish farz beine anha AND dar nazar gerefte mishavad." << endl;
	cout << "3) Shoma mitavanid az parantez estefade konid. faghat mibayest beine kalame va parantez va hamchenin beine 2 paranteze motevalii az space estefade konid" << endl;
	while (true) {
		while (input == "komajdoon")
		{
			cout << "lotfan kalamatoon ro vared konid: ";
			input = "";
//			cin >> input;
			getline(cin, input);
			lowwercase_change(input);
			delete_stop_word(input);
			if(input.length() == 0) {
				cout << "khaheshan kalamte stop word ra vared nafarmaiid!!" << endl;
			} else if (!is_more_than_one_word(input)) {
				input = spell_check(input);
			}
			if (input == "finish") {
				return NULL;
			}
		}
		arr_initialize(input);
		boolean_trie* root = (boolean_trie*) calloc(1, sizeof(boolean_trie));
		input_trie(input, 0, root, 0, input.length() - 1);
		id_trail* p = calc(root);
		id_trail* first_p = p;
		ofstream fout;
		fout.open("result.txt", std::ios_base::app);
		fout << input << endl << endl;
		while(p)
		{
			fout << p->id << endl;
			p = p->next_file;
		}
		fout << "---------" << endl;
		fout.close();
		cout << "farayande jost o joo be etmam resid. natije dar file \"result.txt\" zakhire shode ast." << endl;
		input = "komajdoon";
	}
}

bool is_more_than_one_word(string input) {
	int i = 0;
	bool flag = 0;
	for(i = 0; i < input.length(); i++) {
		if(input[i] == 32) {
			return true;
		}
	}
	return false;
}

template<typename T, typename U>
id_trail* and_words(T p1, U p2)
{
	id_trail* p = 0;
	id_trail* first_p = 0;
	id_trail* tmp = 0;
	while(p1 && p2)
	{
		if(strcmp(p1->id, p2->id) == 0)
		{
			tmp = (id_trail*) calloc(1, sizeof(id_trail));
			tmp->next_file = 0;
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
			strcpy(p->id, p1->id);
			p1 = p1->next_file;
			p2 = p2->next_file;
		}
		else if(((strlen(p1->id) == strlen(p2->id)) && (strcmp(p1->id, p2->id) < 0)) || (strlen(p1->id) < strlen(p2->id)))
			p1 = p1->next_file;
		else
			p2 = p2->next_file;
	}
	return first_p;
}

template<typename T, typename U>
id_trail* or_words(T p1, U p2)
{
	id_trail* p = 0;
	id_trail* first_p = 0;
	id_trail* tmp = 0;
	ofstream fout;
	fout.open("or.txt");
	while(p1 && p2)
	{
		tmp = (id_trail*) calloc(1, sizeof(id_trail));
		tmp->next_file = 0;
		if(strcmp(p1->id, p2->id) == 0)
		{
			fout << p1->id << endl;
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
			strcpy(p->id, p1->id);
			p1 = p1->next_file;
			p2 = p2->next_file;
		}
		else if(((strlen(p1->id) == strlen(p2->id)) && (strcmp(p1->id, p2->id) < 0)) || (strlen(p1->id) < strlen(p2->id)))    
		{
			fout << p1->id << endl;
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
			strcpy(p->id, p1->id);
			p1 = p1->next_file;
		}
		else
		{
			fout << p2->id << endl;
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
			strcpy(p->id, p2->id);
			p2 = p2->next_file;
		}
	}
	while(p1 != 0)
	{
		fout << p1->id << endl;
		tmp = (id_trail*) calloc(1, sizeof(id_trail));
		tmp->next_file = 0;
		if(first_p == 0)
		{
			first_p = tmp;
			p = first_p;
		}
		else
		{
			p->next_file = tmp;
			p = p->next_file;
		}
		strcpy(p->id, p1->id);
		p1 = p1->next_file;
	}
	while(p2 != 0)
	{
		fout << p2->id << endl;
		tmp = (id_trail*) calloc(1, sizeof(id_trail));
		tmp->next_file = 0;
		if(first_p == 0)
		{
			first_p = tmp;
			p = first_p;
		}
		else
		{
			p->next_file = tmp;
			p = p->next_file;
		}
		strcpy(p->id, p2->id);
		p2 = p2->next_file;
	}
	p = first_p;
	fout.close();
	return first_p;
}

id_trail* not_words(id_trail* p1, id_trail* p2)
{
	id_trail* p;
	id_trail* first_p = 0;
	id_trail* ptr = and_words(p1, p2);
	if(ptr == 0)
		return p1;
	id_trail* tmp;
	while(p1 && ptr)
	{
		if(strlen(p1->id) < strlen(ptr->id))
		{
			tmp = (id_trail*) calloc(1, sizeof(id_trail));
			tmp->next_file = 0;
			strcpy(tmp->id, p1->id);
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
		}
		else if((strlen(p1->id) == strlen(ptr->id)) && (strcmp(p1->id, ptr->id) < 0))
		{
			tmp = (id_trail*) calloc(1, sizeof(id_trail));
			tmp->next_file = 0;
			strcpy(tmp->id, p1->id);
			if(first_p == 0)
			{
				first_p = tmp;
				p = first_p;
			}
			else
			{
				p->next_file = tmp;
				p = p->next_file;
			}
		}
		else
			ptr = ptr->next_file;
		p1 = p1->next_file;
	}
	while(p1)
	{
		tmp = (id_trail*) calloc(1, sizeof(id_trail));
		tmp->next_file = 0;
		strcpy(tmp->id, p1->id);
		if(first_p == 0)
		{
			first_p = tmp;
			p = first_p;
		}
		else
		{
			p->next_file = tmp;
			p = p->next_file;
		}
		p1 = p1->next_file;
	}
	return first_p;
}

words* word_place(string input)
{
	words* ptr;
	int index = murmur3_32(&input[0], input.length(), seed) % 10000;
	ptr = hash_table[index];
	while(ptr != 0)
	{
		if(strcmp(ptr->word_name, &input[0]) == 0)
			return ptr;
		ptr = ptr->next_word;
	}
	return 0;
}

void find_word(string input)
{
	words* ptr;
	files* p;
	ofstream fout;
	fout.open(input.c_str());
	fout << left << setw(20) << "file name" << "repetition" << endl;
	int index = murmur3_32(&input[0], input.length(), seed) % 10000;
	ptr = hash_table[index];
	while(ptr != 0)
	{
		if(strcmp(ptr->word_name, &input[0]) == 0)
		{
			p = ptr->files_ptr;
			while(p != 0)
			{
				fout << left << setw(20) << p->id << p->repetition << endl;
				p = p->next_file;
			}
			break;
		}
		ptr = ptr->next_word;
	}
	fout.close();
}

id_trail* calc(boolean_trie* p)
{
	int i = 0, n = 0, cond = 1;
	n = p->number;
	id_trail* tmp1;
	id_trail* tmp2;
	if(n == 1)
		return convert(p->word);
	for(i = 0; i < n; i++)
	{
		tmp1 = calc(p->parts + i);
//		cout << "i: " << i << " - before_cond: " << p->parts[i].before_cond << endl;
		if(cond)
		{
			cond = 0;
			tmp2 = tmp1;
		}
		else if(p->parts[i].before_cond == 1)
			tmp2 = or_words(tmp1, tmp2);
		else if(p->parts[i].before_cond == 0)
			tmp2 = and_words(tmp1, tmp2);
		else if(p->parts[i].before_cond == -1)
			tmp2 = not_words(tmp2, tmp1);
		//free(tmp1);
	}
	return tmp2;
}

int input_trie(string& input, int level, boolean_trie* previous_level, int start, int end)
{
	if(check_down(start, end) == 0)
	{
		previous_level->parts = 0;
		previous_level->number = 1;
		char* token_ptr = strtok(&input[0] + start, "() ");
		strcpy(previous_level->word, token_ptr);
//		cout << "level: " << (level - 1) << " - word: " << previous_level->word << " - before_cond: " << previous_level->before_cond << " - start: " << start << " - end: " << end << endl;
		return 1;
	}
	int tmp1 = 0, tmp2 = 0, i = 0;
	int n = subset_num(level, start, end);
	previous_level->number = n;
	boolean_trie* tmp = (boolean_trie*) calloc(n, sizeof(boolean_trie));
	for(i = 0; i < n; i++)
	{
		if((i != 0) && (i < (n - 1)))
		{
			tmp1 = cond_place(level, i, 1, start, end); // 1 : after
			tmp2 = cond_place(level, i + 1, 0, start, end); // 0 : before
			if(arr[0][tmp1 - 2])
				(tmp + i)->before_cond = 1;
			else if(arr[1][tmp1 - 3] == 0)
				(tmp + i)->before_cond = 0;
			else if(arr[1][tmp1 - 3] == -1)
				(tmp + i)->before_cond = -1;
			input_trie(input, level + 1, tmp + i , tmp1, tmp2);
		}
		else if(i == 0)
		{
			tmp1 = start;
			tmp2 = cond_place(level, i + 1, 0, start, end);
			input_trie(input, level + 1, tmp + i , tmp1, tmp2);
		}
		else
		{
			tmp1 = cond_place(level, i, 1, start, end);
			tmp2 = end;
			if(arr[0][tmp1 - 2])
				(tmp + i)->before_cond = 1;
			else if(arr[1][tmp1 - 3] == 0)
				(tmp + i)->before_cond = 0;
			else if(arr[1][tmp1 - 3] == -1)
				(tmp + i)->before_cond = -1;
			input_trie(input, level + 1, tmp + i , tmp1, tmp2);
		}
	}
	previous_level->parts = tmp;
}

int subset_num(int level, int start, int end)
{
	int counter = 0, i = start;
	while(i <= end)
	{
		if(arr[0][i] && (arr[2][i] == level))
			counter++;
		i++;
	}
	return counter + 1;
}

void arr_initialize(const string& input)
{
	for(int i = 0; i < 1000; i++) {
		arr[0][i] = 0;
		arr[1][i] = 0;
		arr[2][i] = 0;
	}
	
	int pos = input.find("OR");
	while(pos != std::string::npos)
	{
		arr[0][pos] = 1;
		arr[1][pos] = 1;
		arr[2][pos] = check_parantez(input, pos);
		pos = input.find("OR", pos + 2);
	}
	pos = input.find("AND");
	while(pos != std::string::npos)
	{
		arr[0][pos] = 1;
		arr[1][pos] = 0;
		arr[2][pos] = check_parantez(input, pos);
		pos = input.find("AND", pos + 3);
	}
	pos = input.find("NOT");
	while(pos != std::string::npos)
	{
		arr[0][pos] = 1;
		arr[1][pos] = -1;
		arr[2][pos] = check_parantez(input, pos);
		pos = input.find("NOT", pos + 3);
	}
}

int check_down(int start, int end)
{
	int i = start;
	while(i <= end)
	{
		if(arr[0][i])
			return 1;
		i++;
	}
	return 0;
}

int cond_place(int level, int i, int status, int start, int end)
{
	int counter = 0, j = 0;
	for(j = start; j <= end; j++)
	{
		if(arr[0][j] && (arr[2][j] == level))
		{
			counter++;
		}
		if(i == counter)
		{
			if(arr[1][j] == 1)
			{
				if(status)
					return j + 2;
				else
					return j - 1;
			}
			else
			{
				if(status)
					return j + 3;
				else
					return j - 1;
			}
		}
	}
	return -1;
}

int check_parantez(const string& input, int pos)
{
	int i = 0, counter = 0;
	for(i = 0; i < pos; i++)
	{
		if(input[i] == ')')
			counter--;
		else if(input[i] == '(')
			counter++;
	}
	return counter;
}

id_trail* convert(string input)
{
	id_trail* p;
	id_trail* tmp1;
	id_trail* first_p;
	files* file_ptr;
	int cond = 1;
	if(word_place(input) == 0)
		return 0;
	file_ptr = word_place(input)->files_ptr;
	while(file_ptr)
	{
		if(cond)
		{
			cond = 0;
			first_p = (id_trail*) calloc(1, sizeof(id_trail));
			strcpy(first_p->id, file_ptr->id);
			p = first_p;
		}
		else
		{
			tmp1 = (id_trail*) calloc(1, sizeof(id_trail));
			strcpy(tmp1->id, file_ptr->id);
			p->next_file = tmp1;
			p = p->next_file;
		}
		file_ptr = file_ptr->next_file;
	}
	return first_p;
}
