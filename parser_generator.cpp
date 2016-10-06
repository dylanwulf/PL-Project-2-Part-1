#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int number_of_productions;
int number_of_tokens;
vector<string> terminals;
vector<string> nonterminals;

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

void set_variables(vector<string> contents, vector<string> productions) {
    number_of_tokens = contents.size();
    number_of_productions = productions.size();

    for(int i = 0; i < contents.size(); i++) {
        vector<string> temp = split(contents[i].c_str(), ' ');
        if(std::find(terminals.begin(), terminals.end(), temp[0]) == terminals.end())
            terminals.push_back(temp[0]);
    }

    for(int i = 0; i < productions.size(); i++) {
        vector<string> temp = split(productions[i].c_str(), ' ');
        if(std::find(nonterminals.begin(), nonterminals.end(), temp[0]) == nonterminals.end())
            nonterminals.push_back(temp[0]);
    }
}

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
