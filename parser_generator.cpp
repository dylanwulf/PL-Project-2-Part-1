#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int number_of_productions;
int number_of_terminals;
int number_of_nonterminals;
int number_of_symbols;
vector<string> terminals;
vector<int> terminal_numbers;
vector<string> nonterminals;
vector<int> nonterminal_numbers;
vector< vector<int> > right_hand_sides;

/*
    splits the input string into strings around a delimiter, stored in a vector.
 */
vector<string> split(const char *str, char c) {
    vector<string> result;
    do {
        const char *begin = str;
        while(*str != c && *str)
            str++;
        result.push_back(string(begin, str));
    } while (0 != *str++);
    return result;
}

/*
    reads a file and returns the contents of it in a vector string
 */
vector<string> read_file(string filename) {
    vector<string> contents;
    string line;
    ifstream file((char*)filename.c_str());
    if (file.is_open()) {
        while(getline(file, line))
            contents.push_back(line);
        file.close();
    }
    return contents;
}

/*
    populates the terminals vector with all of the terminal symbols
 */
void fill_terminals(vector<string> contents) {
    for(int i = 0; i < number_of_terminals; i++) {
        vector<string> temp = split(contents[i].c_str(), ' ');

        if(find(terminals.begin(), terminals.end(), temp[0]) == terminals.end()) {
            terminals.push_back(temp[0]);
            for(int i = 1; i < temp.size(); i++) {
                if(!temp[i].empty())
                    terminal_numbers.push_back(atoi(temp[i].c_str()));
            }
        }
    }
}

/*
    populates the nonterminals vector with all of the nonterminal symbols
 */
void fill_nonterminals(vector<string> productions) {
    int count = 0;
    for(int i = 0; i < number_of_productions; i++) {
        vector<string> temp = split(productions[i].c_str(), ' ');
        if(find(nonterminals.begin(), nonterminals.end(), temp[0]) == nonterminals.end()) {
            count++;
            nonterminals.push_back(temp[0]);
            nonterminal_numbers.push_back(count);
        }
    }
}

/*
    populates the right_hand_side vector with vectors of the right hand sides of
    the populations
 */
void set_right_hand_sides(vector<string> productions, vector<string> contents) {
    vector<int> first;
    first.push_back(0);
    right_hand_sides.push_back(first);

    for(int i = 0; i < number_of_productions; i++) {
        vector<string> temp = split(productions[i].c_str(), ' ');
        vector<string> rhs_strings;

        if(temp.size() < 3) {
            rhs_strings.push_back("0");
        }

        for(int j = 2; j < temp.size(); j++)
            rhs_strings.push_back(temp[j]);

        vector<int> rhs;
        rhs.push_back(0);
        for(int i = 0; i < rhs_strings.size(); i++) {
            if(find(terminals.begin(), terminals.end(), rhs_strings[i]) != terminals.end()) {
                int pos = find(terminals.begin(), terminals.end(), rhs_strings[i]) - terminals.begin();
                rhs.push_back(0 - terminal_numbers[pos]);
            }
            else if(find(nonterminals.begin(), nonterminals.end(), rhs_strings[i]) != nonterminals.end()) {
                int pos = find(nonterminals.begin(), nonterminals.end(), rhs_strings[i]) - nonterminals.begin();
                rhs.push_back(nonterminal_numbers[pos]);
            }
        }
        for(int i: rhs) {
            cout << i;
            cout << " ";
        }
        cout << endl;
        right_hand_sides.push_back(rhs);
    }
}

/*
    assigns values to the variables declared at the beginning
 */
void set_variables(vector<string> contents, vector<string> productions) {
    number_of_terminals = contents.size() - 1;
    number_of_productions = productions.size();

    fill_terminals(contents);
    fill_nonterminals(productions);

    number_of_nonterminals = nonterminals.size();
    number_of_symbols = number_of_nonterminals + number_of_terminals;

    set_right_hand_sides(productions, contents);
}

/*
    splits the input file into a vector of tokens and a vector of productions,
    then uses these vectors as arguments to set_variables
 */
int main() {
    vector<string> contents = read_file("grammar.txt");
    vector<string> productions;

    while(!contents.back().empty()) {
        productions.push_back(contents.back());
        contents.pop_back();
    }

    reverse(productions.begin(), productions.end());

    set_variables(contents, productions);

    return 0;
}
