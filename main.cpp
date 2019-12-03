#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<iomanip>
#include<cstring>
#include<cstdlib>
#include<conio.h>
#include<ctime>
#include<algorithm>
#include<iterator>
#include<vector>
#include<tr1/unordered_map>
#include<map>
#include<cmath>
#include"data_structure.h"
#include"find_input.h"
typedef std::tr1::unordered_map<std::string, pair<float, int>> Dictionary;
typedef std::tr1::unordered_map<std::string, std::string> miss_Dictionary;

typedef map<char, pair<int, int>> Letters;
int counter = 0;
int dogtag = 0;

#include"my.h"
using namespace std;

int LevenshteinDistance_pooya(string s, string t);
string spell_check(string request);
int LevenshteinDistance(string s, int len_s, string t, int len_t);
int minimum(int a, int b, int c);

SpellCorrector corrector;

int main()
{
	ofstream fout;
	fout.open("result.txt");
	fout.close();
	corrector.load("dictionary2", "miss_dic");
	data_structure();
	find_input();
	fout.open("dictionary2");
	Dictionary::iterator itr2 = corrector.dictionary.begin();
	for(; itr2 != corrector.dictionary.end(); itr2++)
	fout << itr2->first << " " << itr2->second.first << " " << itr2->second.second << endl;
}

string spell_check(string request)
{
	int result = 0, tmp = 0;
    miss_Dictionary::iterator itr;
    Dictionary::iterator itr3;
    itr = corrector.miss_dictionary.find(request);
	if(itr != corrector.miss_dictionary.end())
	{
		cout << "Did you mean: " << corrector.miss_dictionary[request] << "?" << endl;
		cout << "1: Yes! Thank you!" << endl;
		cout << "2: No. My original keyword was true." << endl;
		cout << "3: Just ignore it. Let me search again." << endl;
		cout << "(enter 1, 2 or 3)" << endl;
		cin >> result;
		if(result == 1)
		{
			corrector.dictionary[corrector.miss_dictionary[request]].first++;
			corrector.dictionary[corrector.miss_dictionary[request]].second++;
			return corrector.miss_dictionary[request];
		}
		else if(result == 2)
		{
			corrector.dictionary[request].first = 1;
			corrector.dictionary[request].second = 1;
			return request;
		} else {
			return "komajdoon";
		}
	}
	else
	{
		string temp, before_edit;
		for (string::size_type i = 0;i < request.size();)
		{
			tmp = i;
			while((i < request.length()) && (request[i] == request[tmp]))
				i++;
			if((i - tmp) < 3)
				temp += request.substr(tmp, i - tmp);
			else
				temp += request.substr(tmp, 2);
		}
		before_edit = request;
		request = temp;
		string correct(corrector.correct(request));
		if((correct == request) && (before_edit == request))
		{
			corrector.dictionary[request].first++;
			return request;
		}
		if (correct != "")
		{
			cout << "Did you mean: " << correct << "?" << endl;
//			cout << "To improve our search engine, please choose one of these sentences about above suggestion:\n(just enter 1, 2 or 3)" << endl;
			cout << "1: Yes! Thank you!" << endl;
			cout << "2: No. My original keyword was true." << endl;
			cout << "3: Just ignore it. Let me search again." << endl;
			cout << "(enter 1, 2 or 3)" << endl;
			cin >> result;
			if(result == 1)
			{
				corrector.dictionary[correct].first++;
				corrector.dictionary[correct].second++;
				return correct;
			}
			else if(result == 2)
			{
				corrector.dictionary[request].first = 1;
				corrector.dictionary[request].second = 1;
				return before_edit;
			} else {
				return "komajdoon";
			}
		}
		else
		{
			cout << "Are you sure that your input word is correct?" << endl;
//			cout << "Please choose one of these sentences (just enter 1 or 2 ): " << endl;
			cout << "1: Yes! I'm sure!" << endl;
			cout << "2: Just ignore it. Let me search again." << endl;
			cout << "(enter 1 or 2 )" << endl;
			cin >> result;
			if(result == 1)
			{
				corrector.dictionary[request].first = 1;
				corrector.dictionary[request].second = 1;
				return before_edit;
			} else {
				return "komajdoon";
			}
		}
	}
//    cout << "lotfan ye bar dige kalamatatoon ro vared konid: ";
//    return "komajdoon";
    
}

int minimum(int a, int b, int c)
{
	if((a <= b) && (a <= c))
		return a;
	if((b <= a) && (b <= c))
		return b;
	if((c <= b) && (c <= a))
		return c;
}

int LevenshteinDistance_pooya(string s, string t)
{
	int m = s.length(), n = t.length(), substitutionCost;
	int d[m + 1][n + 1];
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			d[i][j] = 0;
	for(int i = 1; i < m + 1; i++)
		d[i][0] = i;
	for(int i = 1; i < n + 1; i++)
		d[0][i] = i;
	for(int j = 1; j < n + 1; j++)
		for(int i = 1; i < m + 1; i++)
		{
			if(s[i] == t[j])
				substitutionCost = 0;
			else
				substitutionCost = 1;
			d[i][j] = minimum(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + substitutionCost);
		}
	return d[m][n];
}
