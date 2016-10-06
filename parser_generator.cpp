#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

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



int main() {
    vector<string> contents = read_file("grammar.txt");
    vector<string> productions;
    while(!contents.back().empty()) {
        productions.push_back(contents.back());
        contents.pop_back();
    }
    reverse(productions.begin(), productions.end());

    return 0;
}
