Dylan Wulf & David Shull
Project2 - Part1


To run this code, first type the command "make" at the command line to compile.
Next, input your grammar definition into the grammar.txt file.  
To run the program, type "./driver" to enter an interactive mode to type in strings as the  
program runs. To input an EOF character while in interactive mode, press ctrl+D.   
Alternatively, you can run the program using a string stored in a file instead of interactive  
mode by typing "./driver < input.txt" to redirect input to the file instead of stdin.  


For the most part, this project uses vectors to store data. Vectors were chosen
over arrays because indexing was needed for the collections, but the collections
also needed to be grow as data was added to them.


The input to grammar.txt should consist of a text file structured as follows:

    1. A list of token names and numbers, one per line. Token names are strings  
    of printable, non-white- space characters. Token numbers are small  
    non-negative integers, not necessarily contiguous or in any particular  
    order. (This will make it easier for you to use the same scanner for several  
    different grammars. You may assume that all token numbers are less than  
    128, that the value 0 is not used, and that the value 1 is reserved for  
    end-of-file.)  
      
    2. A blank line.  
    3. A list of productions, one per line. Each production should consist of a  
    symbol name, the meta- symbol “→”, and a sequence of zero or more symbol  
    names. I will test your code with only pure BNF: no alternation, no Kleene  
    closure. You may assume that productions with the same left-hand side will  
    be consecutive, and that the start symbol is the left-hand side of the first  
    production.  

The output should be a trace of the predictions and matches made by the
combined efforts of the parser generator, scanner, and driver. This trace will
be printed directly to the terminal.


