using namespace std;

int LevenshteinDistance_pooya(string s, string t);

bool sortBySecond(const pair<std::string, pair<float, int>>& left, const pair<std::string, pair<float, int>>& right) {
	return left.second.first < right.second.first;
}

char filterNonAlphabetic(char& letter) {
	if (isalpha(letter)) { return tolower(letter); }
	return '-';
}

float maximum(float x, float y)
{
	return (x > y) ? x : y;
}

float minimum(float x, float y)
{
	return (x < y) ? x : y;
}

class SpellCorrector
{
	private:
		void edits(const std::string& word, std::vector<std::string>& result)
		{
			for (string::size_type i = 0;i < word.size();    i++)
			{
				result.push_back(word.substr(0, i) + word.substr(i + 1)); //deletions
				counter++;
//				cout << word.substr(0, i) + word.substr(i + 1) << endl;
			}
			for (string::size_type i = 0;i < word.size();    i++)
			{
				if(word[i] == 'i')
				result.push_back(word.substr(0, i) + "ea" + word.substr(i + 1)); 
				counter++;
//				cout << word.substr(0, i) + word.substr(i + 1) << endl;
			}
			for (string::size_type i = 0;i < word.size() - 1;i++)
			{
				result.push_back(word.substr(0, i) + word[i + 1] + word[i] + word.substr(i + 2)); //transposition
				counter++;
//				cout << word.substr(0, i) + word[i + 1] + word[i] + word.substr(i + 2) << endl;
			}
			for (char j = 'a';j <= 'z';++j)
			{
				for (string::size_type i = 0;i < word.size();    i++)
				{
					result.push_back(word.substr(0, i) + j + word.substr(i + 1)); //alterations
					counter++;
//					cout << word.substr(0, i) + j + word.substr(i + 1) << endl;
				}
				for (string::size_type i = 0;i < word.size() + 1;i++)
				{
					result.push_back(word.substr(0, i) + j + word.substr(i)); //insertion
					counter++;
//					cout << word.substr(0, i) + j + word.substr(i) << endl;
				}
			}	
		}
		void known(const std::string& word, std::vector<std::string>& results, Dictionary& candidates)
		{
			Dictionary::iterator end = dictionary.end(); 
			for(unsigned int i = 0; i < results.size(); i++)
			{
				Dictionary::iterator value = dictionary.find(results[i]);
				if(value != end)
				{
					if(candidates.find(value->first) == candidates.end())
					{
						string tmp = value->first;
//						cout << value->first << " " << candidates[value->first].first << endl;
						candidates[value->first].first = log10((value->second).first) + log10((value->second).second) - LevenshteinDistance_pooya(word, value->first) + likeness1(word, value->first, word.length() - tmp.length(), 0);
//						cout << value->first << " " << candidates[value->first].first << endl;
//						cout << "---------------------" << endl;	
					}
				}
			}
		}
	public:
		Dictionary dictionary;
		miss_Dictionary miss_dictionary;
		Letters letter_place;
		float distance(char letter1, char letter2);
		float zero_likeness1(string word, string candidate, int past_diffrence);
		float likeness1(string word, string candidate, int n, int past_diffrence);
		float zero_likeness2(string word, string candidate, int past_difference);
		float likeness2(string word, string candidate, int n, int past_diffrence);
		void load(const std::string& dic, const std::string& miss_dic)
		{
			ifstream file(dic.c_str(), ios_base::binary | ios_base::in);
			string word;
			float num;
			while(!file.eof())
			{
				file >> word;
				lowwercase_change(word);
				file >> num;
				dictionary[word].first = num;
				file >> num;
				dictionary[word].second = num;
			}
			file.close();
			file.open(miss_dic.c_str());
			int i = 0;
			string line;
			while(!file.eof())
			{
				getline(file, line);
				while(line[i] != '-')
				{
					i++;
				}
				word = line.substr(0, i);
				i += 2;
				miss_dictionary[word] = line.substr(i);
				i = 0;
			}
			file.close();
			char letter;
			int x = 0, y = 0;
			file.open("letter_place");
			while(!file.eof())
			{
				file >> letter;
				file >> y;
				file >> x;
//				cout << letter << " " << y << " " << x << endl;
				letter_place[letter].first = x;
				letter_place[letter].second = y;
			}
		}
		std::string correct(const std::string& word)
		{
			std::vector<std::string> result;
			Dictionary candidates;
			if(dictionary.find(word) != dictionary.end())
				return word;
			edits(word, result);
			known(word, result, candidates);
			if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }
			for (unsigned int i = 0;i < result.size();i++) {
				std::vector<std::string> subResult;
		
				edits(result[i], subResult);
				known(word, subResult, candidates);
			}
			if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }
			return "";
		}
};

float SpellCorrector::distance(char letter1, char letter2)
{
//	int arr[3][26] = {{q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m}, {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7}, {3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1}};
	int delta_x = letter_place[letter1].first - letter_place[letter2].first;
//	cout << delta_x << endl;
	int delta_y = letter_place[letter1].second - letter_place[letter2].second;
//	cout << delta_y << endl;
	return sqrt(delta_x * delta_x + delta_y * delta_y);
}

float SpellCorrector::zero_likeness1(string word, string candidate, int past_difference)
{
	int tmp = 0, i = 0, p_ones = 0;
	float counter = 0;
	for(i = 0; i < word.length(); i++)
	{
		if(word[i] != candidate[i])
			p_ones++;
	}
	if(p_ones > (int)((word.length() + past_difference) / (float)2))
	{
		return -1;
	}
	i = 0;
	while(i < word.length())
	{
		tmp = distance(word[i], candidate[i]);
		if(tmp == 0)
			counter += 2;
		else if(tmp < 2)
			counter += 1.5;
		else if((i < word.length() - 1) && (word[i] == candidate[i + 1]) && (word[i + 1] == candidate[i]))
			counter += 3;
		else if(((word[i] == 'c') && (candidate[i] == 'k')) || ((word[i] == 'k') && (candidate[i] == 'c')))
			counter += 2;
		else if(((word[i] == 'g') && (candidate[i] == 'j')) || ((word[i] == 'j') && (candidate[i] == 'g')))
			counter += 2;
//		else if(((word[i] == 'a') && (candidate[i] == 'i')) || ((word[i] == 'e') && (candidate[i] == 'i')))
//			counter += 4;
		i++;
	}
//	if(past_difference)
//	 << "zero_likeness: " << counter << endl;
	return counter;
}

float SpellCorrector::likeness1(string word, string candidate, int n ,int past_difference)
{
	if(n == 0)
		return zero_likeness1(word, candidate, past_difference);
	if(n < 0)
		return likeness2(candidate, word, candidate.length() - word.length(), 0);
	int deleted_place = 0, max_counter = 0;
	string subword;
	while(deleted_place < word.length())
	{
		subword = word.substr(0, deleted_place) + word.substr(deleted_place + 1);
//		cout << "subword: " << subword << endl;
		int tmp = 0, i = 0;
		float counter = likeness1(subword, candidate, subword.length() - candidate.length(), past_difference + 1);
		if(counter == -1)
		{
			deleted_place++;
			continue;
		}
		if(deleted_place > 0)
		{
			if(deleted_place < (word.length() - 1))
				tmp = minimum(distance(word[deleted_place], candidate[deleted_place - 1]), distance(word[deleted_place], candidate[deleted_place]));
			else
				tmp = distance(word[deleted_place], candidate[deleted_place - 1]);
			if(tmp == 0)
				counter += 2;
			else if(tmp < 2)
				counter += 1.5;
		}
		else
		{
			tmp = distance(word[0], candidate[0]);
			if(tmp == 0)
				counter += 2;
			else if(tmp < 2)
				counter += 1;
		}
//		cout << counter << endl;
		max_counter = maximum(max_counter, counter);
		deleted_place++;
	}
	return max_counter;
}

float SpellCorrector::zero_likeness2(string word, string candidate, int past_difference)
{
	int tmp = 0, i = 0, p_ones = 0;
	float counter = 0;
	for(i = 0; i < word.length(); i++)
	{
		if(word[i] != candidate[i])
			p_ones++;
	}
	if(p_ones > (int)((word.length() + past_difference) / (float)2))
	{
		return -1;
	}
	i = 0;
	return 2 * (word.length() - p_ones);
}

float SpellCorrector::likeness2(string word, string candidate, int n ,int past_difference)
{
	if(n == 0)
		return zero_likeness2(word, candidate, past_difference);
	int deleted_place = 0, max_counter = 0;
	string subword;
	while(deleted_place < word.length())
	{
		subword = word.substr(0, deleted_place) + word.substr(deleted_place + 1);
//		cout << "subword: " << subword << endl;
		int tmp = 0, i = 0;
		float counter = likeness2(subword, candidate, subword.length() - candidate.length(), past_difference + 1);
		if(counter == -1)
		{
			deleted_place++;
			continue;
		}
//		cout << counter << endl;
		max_counter = maximum(max_counter, counter);
		deleted_place++;
	}
	return max_counter;
}

