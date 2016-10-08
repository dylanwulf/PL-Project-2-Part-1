Dylan Wulf & David Shull
Project2 - Part1

To run this code, first type the command "make" at the command line. Next,
type "./driver".

The input should consist of a text file structured as follows:

    1. A list of token names and numbers, one per line. Token names are strings
    symbol name, the meta- symbol “→”, and a sequence of zero or more symbol
    names. I will test your code with only pure BNF: no alternation, no Kleene
    closure. You may assume that productions with the same left-hand side will
    be consecutive, and that the start symbol is the left-hand side of the first
    production.

The output should be a trace of the predictions and matches made by the
combined efforts of the parser generator, scanner, and driver. This trace will
be printed directly to the terminal.

For the most part, this project uses vectors to store data. Vectors were chosen
over arrays because indexing was needed for the collections, but the collections
also needed to be grow as data was added to them.

In the "fill_follow()" function, we encountered a memory issue in which the
program would crash occasionally when run. To fix this issue, we allocated a
larger-than-needed vector size to hold onto the follow set vectors. This has
made it so that the program no longer crashes, and we were careful to ignore the
empty space in the vector from that point on.

One major negative aspect of our program is its runtime efficiency. Many of the
algorithms within the program have a big O runtime efficiency of n^2 and higher.
For a large input, this would cause an incredibly slow program. However,
because the algorithms return the correct results and this program is intended
for purely academic use, we decided to keep the result as is.

Additionally, we did not have time to implement any sort of error handling. The
comments for the functions indicate that we handle incorrect grammar input
(such as a malformed grammar or a non-LL(1) grammar), but we ran out of time
and were unable to implement this. Additionally, we ran out of time before
being able to implement syntax error recovery, even though, once again, our
comments indicate that this is something we implemented.