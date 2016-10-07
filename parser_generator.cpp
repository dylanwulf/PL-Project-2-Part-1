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
        int lhs_nonterm_num = nonterminals[nonterminal_name];
        vector<int> production = vector<int>(1, lhs_nonterm_num);
        if (str_prods.size() > 1) {
            vector<string> rhs_str = split(str_prods[1], " ");
            for (vector<string>::iterator it = rhs_str.begin(); it != rhs_str.end(); it++) {
                int term_num = terminals[*it];
                int nonterm_num = nonterminals[*it];
                if (term_num != 0)
                    production.push_back(-1*term_num);
                else
                    production.push_back(nonterm_num);
            }
        }
        prod_out.push_back(production);
    }
    for(vector<int> v: prod_out) {
        cout << v[0] << endl;
    }
    return prod_out;
}

vector<bool> fill_eps(vector< vector<int> > prods, vector<int> prod_to_nons, int num_nonterms) {
    vector<bool> out(num_nonterms + 1, false);
    for (int i = 0; i < prods.size(); i++) {
        if (prods[i].empty()) {
            out[prod_to_nons[i]] = true;
        }
    }
    return out;
}

bool vector_contains(vector<int> v, int n) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == n)
            return true;
    }
    return false;
}

//add v2 stuff into v1
vector<int> vector_union(vector<int> v1, vector<int> v2) {
    vector<int> out(v1);
    for (int i = 0; i < v2.size(); i++) {
        if (!vector_contains(out, v2[i])) {
            out.push_back(v2[i]);
        }
    }
    return out;
}

vector< vector<int> > fill_first(vector< vector<int> > prods, vector<bool> eps, vector<int> prod_to_nons, int num_non_terms) {
    vector< vector<int> > firsts;
    for (int i = 0; i < num_non_terms; i++) //make a vector for each nonterminal
        firsts.push_back(vector<int>());

    for(int lhs = 0; lhs < prods.size(); lhs++) {
        for (int rhs = 0; rhs < prods[lhs].size(); rhs++) {
            int term = prods[lhs][rhs];
            int first_loc = prod_to_nons[lhs];
            if (term < 0) {
                firsts[first_loc] = vector_union(firsts[first_loc], vector<int>(1, term));
            }
            else
                firsts[first_loc] = vector_union(firsts[first_loc], firsts[term]);
            if (term > 0 && eps[term-1] == false)
                rhs = prods[lhs].size();
        }
    }
    return firsts;
}

// vector< vector<int> > fill_follow(vector< vector<int> > productions, vector<bool> eps, vector<int> prods_to_nons,
// int number_of_symbols, vector< vector<int> > firsts) {
//     vector< vector<int> > follows;
//     for (int i = 0; i < number_of_symbols; i++)
//         follows.push_back(vector<int>());
//
//     for(int i = 0; i < productions.size(); i++) {
//         for(int j = 0; j < productions[i].size(); j++) {
//             if(productions[i] )
//         }
//     }
// }

vector<int> prod_to_nonterms(vector<string> productions, map<string, int> nonterms) {
    vector<int> out;
    out.push_back(0);
    out[0] = 0; //productions start at #1, so we'll just ignore index 0
    for (int i = 1; i < productions.size() + 1; i++) {
        string nonterm_str = split(split(productions[i-1], "->")[0], " ")[0];
        out.push_back(nonterms[nonterm_str]);
    }
    return out;
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

    vector<int> prod_to_nons = prod_to_nonterms(productions, nonterminals_map);

    vector< vector<int> > prods = fill_productions(terminals_map, nonterminals_map, productions);
    vector<bool> eps = fill_eps(prods, prod_to_nons, nonterminals_map.size());

    vector< vector<int> > firsts = fill_first(prods, eps, prod_to_nons, nonterminals_map.size());
    for (int i = 0; i < firsts.size(); i++) {
        for (int j = 0; j < firsts[i].size(); j++) {
            cout << firsts[i][j] << ", ";
        }
        cout << endl;
    }

    // vector< vector<int> > follows = fill_follow(prods, eps, prod_to_nons,
    //     nonterminals_map.size() + terminals_map.size(), firsts);

    //set_variables(terminals, productions);

    return 0;
}
