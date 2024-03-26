# How to run 

1 Compilation
```bash
  g++ parse.cpp -o parser
```
2 Running Parser
```bash
  ./parser
```
3 Give input file name(file should be present in same directory as parse.cpp) Ex- input.java

4 Generation of tree.dot and token.txt file for given input


# Interpreting the Result 

1. Generated tokens are stored in token.txt file in format (lexeme, token type , line no)
2. Copy and paste the content of tree.dot file here (https://dreampuf.github.io/GraphvizOnline) for visualising the parse tree.