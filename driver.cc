#include <iostream>
#include <iomanip>
#include <stack>
using std::cout;
using std::setw;
using std::left;
using std::right;

#include "scan.h"
#include "parser_generator.cpp"
using namespace scanner;

bool grammar_contains(int token_number) {
    map<string, int> terminals_map = fill_terminals(terminals);
    for(auto elem: terminals_map)
        if(elem.second == token_number) return true;
    return false;
}

/*
    Uses a parse table to creates a working parser for the CGF in put to the
    parser_generator.cpp file. This parser takes in any strings that are of the
    language defined by the CFG. The predictions and matches of the parser are
    traced and printed as the strings are processed. Incorrect input is marked
    as such for the user. If the grammar is not LL(1), then the driver does not
    accept any input and terminates.
 */
int main() {
    if (generate_parse_table("grammar.txt")) return 0;
    stack<int> parse_stack;
    parse_stack.push(prods[0][0]);
    cout << endl << endl << "Start typing!" << endl;
    token input = scan();
    while (true) {
        if (!grammar_contains(input.num)) {
            cout << "Error: incorrect syntax" << endl;
            input = scan();
        }
        int expected = parse_stack.top();
        parse_stack.pop();
        if (expected < 0) {
            if (expected * -1 == input.num) {
                cout << "Match " << input.image << endl;
                input = scan();
            }
            else {
                cout << "ERROR: expected: " << expected << " input: " << input.num << endl;
                continue;
            }
            if (expected * -1 == tok_eof) {
                cout << "Done! Bye!" << endl;
                exit(0);
            }
        }
        else {
            if (parse_table[expected-1][input.num-1] == 0) {
                cout << "Parse error :( Exiting" << endl;
                cout << "expected: " << expected << " input: " << input.num << endl;
                exit(1);
            }
            else {
                int prediction = parse_table[expected-1][input.num-1];
                cout << "Predict " << productions[prediction-1] << endl;
                for (int i = prods[prediction-1].size() - 1; i > 0; i--){
                    parse_stack.push(prods[prediction-1][i]);
                }
            }
        }
    }

    token tok;
    do {
        tok = scan();
        cout << setw(12) << left << tok.name
             << setw(5) << right << tok.line
             << setw(4) << right << tok.column
             << "   " << tok.image << "\n";
    } while (tok.num != tok_eof);
    return 0;
}
