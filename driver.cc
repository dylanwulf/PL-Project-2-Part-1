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

/*
    Uses a parse table to creates a working parser for the CGF in put to the
    parser_generator.cpp file. This parser takes in any strings that are of the
    language defined by the CFG. The predictions and matches of the parser are
    traced and printed as the strings are processed. Incorrect input is marked
    as such for the user.
 */
int main() {
    generate_parse_table();
    stack<int> parse_stack;
    parse_stack.push(prods[0][0]);
    cout << "Pushed start symbol " << prods[0][0] << endl;
    cout << "Predict " << productions[0] << endl;
    token input = scan();
    while (true) {
        int expected = parse_stack.top();
        parse_stack.pop();
        if (expected < 0) {
            if (expected * -1 == input.num) {
                cout << "Match " << input.image << endl;
            }
            else {
                cout << "ERROR: expected: " << expected << " input: " << input.num << endl;
                exit(1);
            }
            if (expected * -1 == tok_eof) {
                cout << "Done! Bye!" << endl;
                exit(0);
            }
        }
        else {
            if (parse_table[expected-1][input.num-1] == 0) {
                cout << "Parse error :( Exiting" << endl;
                exit(1);
            }
            else {
                int prediction = parse_table[expected-1][input.num-1];
                cout << "Predict " << productions[prediction] << endl;
                for (int i = prods[prediction].size() - 1; i > 0; i--){
                    parse_stack.push(prods[prediction][i]);
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
