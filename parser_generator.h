#include <vector>
#include <map>

using namespace std;

int max_terminal = 0;
vector<string> terminals;
vector<string> productions;
vector< vector<int> > prods;
vector< vector<int> > firsts;
vector< vector<int> > follows;
vector< vector<int> > parse_table;
map<string, int> terminals_map;
map<string, int> nonterminals_map;

int generate_parse_table(string grammar_file_name);
