#ifndef PARSER_GENERATOR_H
#define PARSER_GENERATOR_H

#include <vector>
#include <map>

using namespace std;

extern int max_terminal;
extern vector<string> terminals;
extern vector<string> productions;
extern vector< vector<int> > prods;
extern vector< vector<int> > firsts;
extern vector< vector<int> > follows;
extern vector< vector<int> > parse_table;
extern map<string, int> terminals_map;
extern map<string, int> nonterminals_map;

extern int generate_parse_table(string grammar_file_name);

#endif
