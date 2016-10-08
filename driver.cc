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

// Call scanner repeatedly, printing results.
// You'll throw this code away once you've written your parser driver.



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
                input = scan();
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
