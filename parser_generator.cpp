#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
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
vector<string> split(string str, string delimiter) {
    vector<string> out;
    string substr;
    int current_part = 0;
    for (int i = 0; i < str.size() - delimiter.size() + 1; i++) {
        substr = str.substr(i, delimiter.size());
        if (substr == delimiter) {
            string new_entry = str.substr(current_part, i - current_part);
            if (!new_entry.empty())
                out.push_back(new_entry);
            current_part = i + delimiter.size();
        }
    }
    if (current_part != str.size())
        out.push_back(str.substr(current_part, str.size() - current_part));
    return out;
}

/*
    reads a file and returns the contents of it in a vector string
 */
vector<string> read_file(string filename) {
    vector<string> contents;
    ifstream file((char*)filename.c_str());
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            contents.push_back(line);
        file.close();
    }
    return contents;
}

/*
    populates the terminals vector with all of the terminal symbols
 */
map<string, int> fill_terminals(vector<string> terminals_str) {
    map<string, int> terminals_map;
    for (vector<string>::iterator it = terminals_str.begin(); it != terminals_str.end(); it++) {
        vector<string> split_t = split(*it, " ");
        terminals_map.insert(pair<string, int>(split_t[0], atoi(split_t[1].c_str())));
    }
    return terminals_map;
}

/*
    populates the nonterminals vector with all of the nonterminal symbols
 */
map<string, int> fill_nonterminals(vector<string> productions) {
    map<string, int> nonterminals_map;
    for (vector<string>::iterator it = productions.begin(); it != productions.end(); it++) {
        vector<string> str_prods = split(*it, "->");
        string nonterminal_name = split(str_prods[0], " ")[0];
        nonterminals_map.insert(pair<string, int>(nonterminal_name, nonterminals_map.size()+1));
    }
    return nonterminals_map;
}

vector< vector<int> > fill_productions(map<string, int> terminals, map<string, int> nonterminals, vector<string> productions) {
    vector< vector<int> > prod_out;
    for (vector<string>::iterator it = productions.begin(); it != productions.end(); it++) {
        vector<string> str_prods = split(*it, "->");
        string nonterminal_name = split(str_prods[0], " ")[0];
        vector<int> rhs;
        if (str_prods.size() > 1) {
            vector<string> rhs_str = split(str_prods[1], " ");
            for (vector<string>::iterator it = rhs_str.begin(); it != rhs_str.end(); it++) {
                int term_num = terminals[*it];
                int nonterm_num = nonterminals[*it];
                if (term_num != 0)
                    rhs.push_back(-1*term_num);
                else
                    rhs.push_back(nonterm_num);
            }
        }
        prod_out.push_back(rhs);
    }
    return prod_out;
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

    //set_right_hand_sides(productions, contents);
}

/*
    splits the input file into a vector of tokens and a vector of productions,
    then uses these vectors as arguments to set_variables
 */
int main() {
    vector<string> contents = read_file("grammar.txt");
    vector<string> terminals;
    vector<string> productions;

    //split contents into terminals and productions
    bool found_blank = false;
    for (vector<string>::iterator it = contents.begin(); it != contents.end(); it++) {
        if (!found_blank) {
            if (*it != "")
                terminals.push_back(*it);
            else
                found_blank = true;
        }
        else {
            productions.push_back(*it);
        }
    }
    
    map<string, int> terminals_map = fill_terminals(terminals);
    map<string, int> nonterminals_map = fill_nonterminals(productions);

    vector< vector<int> > prods = fill_productions(terminals_map, nonterminals_map, productions);

    for (vector< vector<int> >::iterator it = prods.begin(); it != prods.end(); it++) {
        reverse((*it).begin(), (*it).end());
        for (vector<int>::iterator jt = (*it).begin(); jt != (*it).end(); jt++)
            cout << *jt << ", ";
        cout << endl;
    }

    //set_variables(terminals, productions);

    return 0;
}
