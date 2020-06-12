#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <set>
#include <iomanip> // setw


#include "dictionary.h"

using namespace std;

void lower(string& s);
string stripPunct(const string& s);
void checkSpelling(ifstream& in, Dictionary& dict);



// program arguments to run, example: main.exe ../../res/wordlist.txt ../../res/test.txt
int main(int argc, char* argv[]) {
	
	// Output usage message if improper command line args were given.
	if (argc != 3)
    {
		cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
		return EXIT_FAILURE;
	}

	ifstream inf(argv[2]);
	if (! inf) 
    {
		cerr << "Could not open " << argv[2] << "\n";
		return EXIT_FAILURE;
	}
	
	// Read dictionary, but let user know what we are working on.
	cout << "Loading dictionary, this may take awhile...\n";

	Dictionary d(argv[1]);
    
	checkSpelling(inf, d);

	inf.close();

	return EXIT_SUCCESS;
}

void transposingAdjacentLetters(std::string& word, Dictionary& dict, std::set<std::string>& correctionSet)
{
    std::string tmp;
    for (int i = 0; i < word.length() - 1; ++i)
    {
        for (int j = 0; j < word.length(); ++j)
        {
            if(i == j){
                 tmp += word[i + 1];
            }
            else
            {
                if(i + 1 == j){
                    tmp += word[i];
                }
                else
                {
                    tmp += word[j];
                }
            }
        }
        if(dict.search(tmp))
        {
            correctionSet.insert("\t" + tmp + "\n");
        }
        tmp = "";
    }
}

void RemoveLetters(std::string& word, Dictionary& dict, std::set<std::string>& correctionSet)
{
    std::string tmp;
    for (int i = 0; i < word.length(); ++i)
    {
        for (int j = 0; j < word.length(); ++j)
        {
            if(j != i)
                tmp += word[j];
        }
        if(dict.search(tmp))
        {
            correctionSet.insert("\t" + tmp + "\n");
        }
        tmp = "";
    }
}

void ReplacementLetters(std::string& word, Dictionary& dict, std::set<std::string>& correctionSet)
{
    std::string tmp;
    for (int i = 0; i < word.length(); ++i)
    {
        for (int j = 'a'; j < 'z' + 1; ++j)
        {
            for (int k = 0; k < word.length(); ++k)
            {
                if(i != k)
                {
                    tmp += word[k];
                }
                else
                {
                    tmp += static_cast<char>(j);
                }
            }
            if(dict.search(tmp))
            {
                correctionSet.insert("\t" + tmp + "\n");
            }
            tmp = "";
        }
    }
}

void InsertingLetters(std::string& word, Dictionary& dict, std::set<std::string>& correctionSet)
{
    std::string tmp;
    for (int i = 0; i < word.length() + 1; ++i)
    {
        for (int j = 'a'; j < 'z' + 1; ++j)
        {
            for (int k = 0; k < word.length(); ++k)
            {
                if(i == k)
                {
                    tmp += static_cast<char>(j);
                    tmp += word[k];
                }
                else
                {
                    if(i == word.length() && k == word.length() - 1)
                    {
                        tmp += word[k];
                        tmp += static_cast<char>(j);
                    }
                    else
                    {
                        tmp += word[k];
                    }
                }
            }
            if(dict.search(tmp))
            {
                correctionSet.insert("\t" + tmp + "\n");
            }
            tmp = "";
        }
    }
}

void checkSpelling(ifstream& in, Dictionary& dict) {

	int line_number = 0;

	while (in) 
    {
		line_number++;
		set<string> totest;
		string line;
		getline(in, line);

  	    stringstream ss (stringstream::in | stringstream::out);
		ss << line;
		
		string word;
		while (ss >> word) 
        {
		    word = stripPunct(word);
		    lower(word);
            if (!dict.search(word))
            {
                std::set<std::string> correctionSet;
                std::cout << "\n" << word << ":\n";
                transposingAdjacentLetters(word, dict, correctionSet);
                RemoveLetters(word, dict, correctionSet);
                ReplacementLetters(word, dict, correctionSet);
                InsertingLetters(word, dict, correctionSet);
                for(std::string correction : correctionSet)
                {
                    std::cout << correction;
                }
            }
		}
	}
}

void lower(string& s) {

    // Ensures that a word is lowercase
	for (int i = 0; i < s.length(); i++)
    {
		s[i] = tolower(s[i]);
	}
}

string stripPunct(const string& s) {

	// Remove any single trailing
	// punctuation character from a word.  
	if (ispunct(s[s.length() - 1]) ) 
    {
		return s.substr (0, s.length() - 1);
	}
	else 
    {
		return s;
	}
}
