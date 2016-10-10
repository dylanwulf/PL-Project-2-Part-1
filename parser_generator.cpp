#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "parser_generator.h"
using namespace std;

//--globals--
int max_terminal;
vector<string> terminals;
vector<string> productions;
vector< vector<int> > prods;
vector< vector<int> > firsts;
vector< vector<int> > follows;
vector< vector<int> > parse_table;
map<string, int> terminals_map;
map<string, int> nonterminals_map;

/*
    Takes in a string to be split and a delimiter around which to split the
    string. The resulting substrings are stored in a vector, which is returned.

    Used by functions: fill_terminals(), fill_nonterminals(), fill_productions()
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
    Takes in a file's name in string form and reads the contents of the file.
    Each line is separately added to a vector of strings, which is returned.

    Used by functions: generate_parse_table()
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
    Takes in a vector of all terminals in string form. These terminals are added
    as keys to a map with their integer equivalent as their keys.

    Used by functions: generate_parse_table()
 */
map<string, int> fill_terminals(vector<string> terminals_str) {
    map<string, int> terminals_map;
    for (vector<string>::iterator it = terminals_str.begin(); it != terminals_str.end(); it++) {
        vector<string> split_t = split(*it, " ");
        int termnum = atoi(split_t[1].c_str());
        if (termnum > max_terminal)
            max_terminal = termnum;
        terminals_map.insert(pair<string, int>(split_t[0], termnum));
    }
    return terminals_map;
}

/*
    Takes in a vector of all nonterminals in string form. These terminals are
    added as keys to a map with their integer equivalent as their keys.

    Used by functions: generate_parse_table()
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

/*
    Takes in the map of terminals and the map of nonterminals, as well as the
    vector of strings holding all of the productions. These inputs are used to
    create a 2D vector of integers representing the productions numerically,
    which is returned.

    Used by functions: generate_parse_table()
 */
vector< vector<int> > fill_productions(map<string, int> terminals, map<string, int> nonterminals, vector<string> productions) {
    vector< vector<int> > prod_out;
    //for each production
    for (vector<string>::iterator it = productions.begin(); it != productions.end(); it++) {
        //split string at -> symbol
        vector<string> str_prods = split(*it, "->");
        string nonterminal_name = split(str_prods[0], " ")[0];
        int lhs_nonterm_num = nonterminals[nonterminal_name];
        vector<int> production = vector<int>(1, lhs_nonterm_num);
        //if this production isn't empty
        if (str_prods.size() > 1) {
            //split apart symbols at spaces
            vector<string> rhs_str = split(str_prods[1], " ");
            //for each symbol in the production
            for (vector<string>::iterator it = rhs_str.begin(); it != rhs_str.end(); it++) {
                //find the number ID of that symbol using the maps
                int term_num = terminals[*it];
                int nonterm_num = nonterminals[*it];
                if (term_num != 0)
                    production.push_back(-1*term_num); //mark terminals with negative sign
                else
                    production.push_back(nonterm_num);
            }
        }
        prod_out.push_back(production);
    }
    return prod_out;
}

/*
    Takes in the 2D vector containing the numeric representation of all
    productions, as well as the number of unique nonterminals in the
    productions. These are used to determine which nonterminals can go to
    epsilon. The bool value representing whether a nonterminal can produce
    epsilon is added to a vector, which is returned.

    Used by functions: generate_parse_table()
*/
vector<bool> fill_eps(vector< vector<int> > prods, int num_nonterms) {
    vector<bool> out(num_nonterms + 1, false);
    for (int i = 0; i < prods.size(); i++) {
        if (prods[i].size() <= 1) {
            out[prods[i][0]] = true;
        }
    }
    return out;
}

/*
    Takes in two vectors and adds every element in the second vector not already
    contained by the first vector into the first vector. Return false if nothing
    was added and true if something was added.

    Used by functions: fill_first(), string_first(), fill_follow(), fill_predict()
 */
bool vector_union(vector<int> &v1, vector<int> &v2) {
    vector<int> temp(v1.size() + v2.size());
    vector<int>::iterator it;
    it = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), temp.begin());
    temp.resize(it - temp.begin());
    v1.swap(temp);
    if (v1.size() != temp.size())
        return true;
    return false;
}

/*
    Takes in a 2D vector of the numeric representations of productions, the
    vector of booleans representing whether a nonterminal can produce epsilon,
    and the number of unique nonterminals. These are used to calculate the
    FIRST set of all nonterminals, which are stored inside of a 2D vector, which
    is then returned.

    Used by functions: generate_parse_table()
 */
vector< vector<int> > fill_first(vector< vector<int> > prods, vector<bool> eps, int num_non_terms) {
    //initialize by making a vector for each nonterminal
    vector< vector<int> > firsts(num_non_terms + 1, vector<int>());

    //Keep looping until no progress
    bool changed = true;
    while (changed) {
        changed = false;
        //for each production
        for(int lhs = 0; lhs < prods.size(); lhs++) {
            //for each symbol in current production
            //start at 1 because the lhs is stored at location 0
            for (int rhs = 1; rhs < prods[lhs].size(); rhs++) {
                int term = prods[lhs][rhs];
                if (term < 0) { //if terminal
                    vector<int> term_vector(1, term);
                    //add this term to the first set of the symbol on the lhs
                    changed = changed || vector_union(firsts[prods[lhs][0]], term_vector);
                    rhs = prods[lhs].size() + 5; //continue outer loop.
                }
                else { //if nonterminal
                    //add the firsts of this term to the firsts of the lhs
                    changed = changed || vector_union(firsts[prods[lhs][0]], firsts[term]);
                }
                if (term > 0 && eps[term] == false)
                    rhs = prods[lhs].size() + 5;
                if (rhs == prods[lhs].size() - 1 && term > 0)
                    eps[prods[lhs][0]] = true;
            }
        }
    }
    return firsts;
}

/*
    Implementation of the string_EPS function seen in the textbook algorithm
    Used by functions: fill_follow()
 */
bool string_eps(vector<int> X, int begin, int end, vector<bool> eps) {
    for (int i = begin; i < end; i++) {
        if ( X[i] < 0 || !eps[X[i]])
            return false;
    }
    return true;
}

/*
    Implementation of the string_first function seen in the textbook algorithm
    Used by functions: fill_follow()
 */
vector<int> string_first(vector<int> X, int begin, int end, vector< vector<int> > firsts, vector<bool> eps) {
    vector<int> out;
    for (int i = begin; i < end; i++) {
        vector<int> first;
        if (X[i] < 0)
            first = vector<int>(1, X[i]);
        else
            first = firsts[X[i]];
        vector_union(out, first);
        if (X[i] < 0 || !eps[X[i]]){
            return out;
        }
    }
    return out;
}

/*
    Takes in a 2D vector of the numeric representations of productions, the
    vector of booleans representing whether a nonterminal can produce epsilon,
    the number of unique nonterminals, and the 2D vector of all FIRST sets.
    These are used to calculate the FOLLOW set of all nonterminals, which are
    stored inside of a 2D vector, which is then returned.

    Used by functions: generate_parse_table()
 */
vector< vector<int> > fill_follow(vector< vector<int> > productions, vector<bool> eps,
int number_of_nonterms, vector< vector<int> > firsts) {
    //initialize follow vectors
    vector< vector<int> > follows(number_of_nonterms + 1, vector<int>());

    //Keep looping until no progress
    bool change = true;
    while (change) {
        change = false;
        //for each production
        for (int i = 0; i < productions.size(); i++){
            //for each symbol in production rhs
            for (int j = 1; j < productions[i].size(); j++) {
                //if nonterminal and not the last symbol
                if(j < productions[i].size() - 1 && productions[i][j] > 0) {
                    //compute first set of all symbols following the current one
                    vector<int> str_first = string_first(productions[i], j+1, productions[i].size(), firsts, eps);
                    vector<int> *follow_current = &follows[productions[i][j]-1];
                    //add str_first to follow_current
                    change = change || vector_union(*follow_current, str_first);
                }
                //if nonterminal and (this is the last symbol in the production OR
                //all symbols after current one can go to epsilon)
                if(productions[i][j] > 0 && (j == productions[i].size() - 1 || string_eps(productions[i], j+1, productions[i].size(), eps))) {
                    vector<int> *follow_current = &follows[productions[i][j]-1];
                    vector<int> follow_lhs = follows[productions[i][0]-1];
                    //add follow of lhs to follow of current symbol
                    change = change || vector_union(*follow_current, follow_lhs);
                }
            }
        }
    }
    return follows;
}

/*
    Takes in a 2D vector of the numeric representations of productions, the
    vector of booleans representing whether a nonterminal can produce epsilon,
    the 2D vector of the numeric representations of all FIRST sets, and the 2D
    vector of the numeric represenations of all FOLLOW sets. These are used to
    calculate the FIRST set of all nonterminals, which are stored inside of a 2D
    vector, which is then returned.

    Used by functions: generate_parse_table()
 */
vector< vector<int> > fill_predict(vector< vector<int> > productions, vector<bool> eps,
vector< vector<int> > firsts, vector< vector<int> > follows) {
    vector< vector<int> > predicts;
    for(int i = 0; i < productions.size(); i++) {
        vector<int> predict;
        if(!(productions[i].size() < 2)) {
            if(productions[i][1] < 0) {
                vector<int> temp = vector<int>(1, productions[i][1]);
                vector_union(predict, temp);
            }
            else {
                vector_union(predict, firsts[productions[i][1]]);
                if(eps[productions[i][1]]) {
                    vector_union(predict, follows[productions[i][0]]);
                }
            }
        }
        else {
            vector_union(predict, follows[productions[i][0]-1]);
        }
        predicts.push_back(predict);
    }
    return predicts;
}

/*
    Takes in a number of rows and columns, the 2D vector of the numeric
    representations of the predict sets, and the 2D vector of the numeric
    representations of the productions. These are used to created a 2D vector
    that contains the full parse table for the grammar.

    Used by functions: generate_parse_table()
 */
vector< vector<int> > fill_parse_table(int rows, int cols, vector< vector<int> > predict, vector< vector<int> > prods) {
    vector< vector<int> > p_tab(rows, vector<int>(cols, 0));
    for (int i = 0; i < predict.size(); i++) {
        for (int j = 0; j < predict[i].size(); j++) {
            int row = prods[i][0] - 1;
            int col = predict[i][j]*(-1) - 1;
            if(p_tab[row][col] == 0)
                p_tab[row][col] = i + 1;
            else {
                cout << "Grammar is not LL(1)." << endl;
                vector< vector<int> > incorrect_input(0, vector<int>(0, 0));
                return incorrect_input;
            }
        }
    }
    return p_tab;
}

/*
    Runs the preceding functions in order to generate a parse table from the
    grammar passed to the read_file() function. If the input grammar is
    malformed or not LL(1), an error message is printed to the terminal.

    Used by: driver.cc
 */
int generate_parse_table(string grammar_file_name) {
    vector<string> contents = read_file(grammar_file_name);

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

    //calculate map from terminal names to terminal numbers
    terminals_map = fill_terminals(terminals);
    //calculate map from nonterminal names to nonterminal numbers
    nonterminals_map = fill_nonterminals(productions);

    //use the maps to generate a 2D int vector of productions
    prods = fill_productions(terminals_map, nonterminals_map, productions);
    vector<bool> eps = fill_eps(prods, nonterminals_map.size());

    int num_nonterms = nonterminals_map.size();
    int num_terms = terminals_map.size();
    firsts = fill_first(prods, eps, num_nonterms);

    follows = fill_follow(prods, eps, num_nonterms, firsts);

    vector< vector<int> > predicts = fill_predict(prods, eps, firsts, follows);

    parse_table = fill_parse_table(num_nonterms, max_terminal, predicts, prods);
    if(parse_table.size() == 0) {
        cout << "Please input an LL(1) grammar." << endl;
        return 1;
    }
    return 0;
}
