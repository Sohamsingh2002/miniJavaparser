
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <bits/stdc++.h>

using namespace std;

enum TokenType {
    ID, NUM, STRING, TRUE, FALSE, THIS, NULL_, SEMI, COMMA, DOT, ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, LBRACK, RBRACK, PLUS, MINUS, MULTIPLY, DIVIDE, INC, DEC, MOD, LT, GT, LEQ, GEQ, EQ, NEQ, AND, OR, NOT, IF, ELSE, WHILE, FOR, RETURN, CLASS, EXTENDS, PUBLIC, PRIVATE, STATIC, VOID, INT, BOOLEAN, STRINGTYPE, BOOL, EOFF, NEW, UNKNOWN
};

string TokenTypeStrings[] = {
    "ID", "NUM", "STRING", "TRUE", "FALSE", "THIS", "NULL_", "SEMI", "COMMA", "DOT", "ASSIGN", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "INC", "DEC", "MOD", "LT", "GT", "LEQ", "GEQ", "EQ", "NEQ", "AND", "OR", "NOT", "IF", "ELSE", "WHILE", "FOR", "RETURN", "CLASS", "EXTENDS", "PUBLIC", "PRIVATE", "STATIC", "VOID", "INT", "BOOLEAN", "STRINGTYPE", "BOOL", "EOFF", "NEW", "UNKNOWN"
};

bool valid=true;

struct Token {
    string lexeme;
    TokenType type;
    int lineNo;
};

void unknown_char(char c,int l){
    printf("Lexical Error : Unknown char %c in line no %d \n",c,l);
    valid=false;
}


class Lexer {
private:
    string sourceCode;
    unordered_set<string> keywords = {"true", "false", "this", "null", "if", "else", "while", "for", "return", "class", "extends", "public", "private", "static", "void", "int", "boolean", "String", "bool", "new"};
    size_t currentPos = 0;
    char currentChar;
    int currentLine = 1;

    char getNextChar() {
        if (currentPos < sourceCode.size()) {
            char ch = sourceCode[currentPos++];
            if (ch == '\n') currentLine++;
            return ch;
        }
        return '\0'; // End of input
    }

    void consumeWhiteSpace() {
        while (isspace(currentChar)) {
            if (currentChar == '\n') currentLine++;
            currentChar = getNextChar();
        }
    }

    string getNextTokenLexeme(TokenType type) {
        string lexeme;
        lexeme += currentChar;
        currentChar = getNextChar();
        while (isalnum(currentChar) || currentChar == '_') {
            lexeme += currentChar;
            currentChar = getNextChar();
        }
        return lexeme;
    }

    string getNumericLiteral() {
        string lexeme;
        while (isdigit(currentChar)) {
            lexeme += currentChar;
            currentChar = getNextChar();
        }
        return lexeme;
    }

    string getStringLiteral() {
        string lexeme;
        currentChar = getNextChar(); // Consume starting double quote
        while (currentChar != '"' && currentChar != '\0') {
            lexeme += currentChar;
            currentChar = getNextChar();
        }
        currentChar = getNextChar(); // Consume ending double quote
        return lexeme;
    }

public:
    Lexer(const string& source) : sourceCode(source) {
        currentChar = getNextChar();
    }

    Token getNextToken() {
        consumeWhiteSpace();

        if (currentChar == '\0') {
            return {"$", EOFF, currentLine}; // End of input
        }

        if (isalpha(currentChar) || currentChar == '_') {
            string lexeme = getNextTokenLexeme(ID);
            

            if (keywords.find(lexeme) != keywords.end()) {
                if (lexeme == "true") {
                    return {lexeme, TRUE, currentLine};
                } else if (lexeme == "false") {
                    return {lexeme, FALSE, currentLine};
                } else if (lexeme == "this") {
                    return {lexeme, THIS, currentLine};
                } else if (lexeme == "null") {
                    return {lexeme, NULL_, currentLine};
                } else if (lexeme == "bool") {
                    return {lexeme, BOOL, currentLine};
                } else if (lexeme == "if") {
                    return {lexeme, IF, currentLine};
                } else if (lexeme == "else") {
                    return {lexeme, ELSE, currentLine};
                } else if (lexeme == "while") {
                    return {lexeme, WHILE, currentLine};
                } else if (lexeme == "for") {
                    return {lexeme, FOR, currentLine};
                } else if (lexeme == "return") {
                    return {lexeme, RETURN, currentLine};
                } else if (lexeme == "class") {
                    return {lexeme, CLASS, currentLine};
                } else if (lexeme == "extends") {
                    return {lexeme, EXTENDS, currentLine};
                } else if (lexeme == "public") {
                    return {lexeme, PUBLIC, currentLine};
                } else if (lexeme == "private") {
                    return {lexeme, PRIVATE, currentLine};
                } else if (lexeme == "static") {
                    return {lexeme, STATIC, currentLine};
                } else if (lexeme == "void") {
                    return {lexeme, VOID, currentLine};
                } else if (lexeme == "int") {
                    return {lexeme, INT, currentLine};
                } else if (lexeme == "boolean") {
                    return {lexeme, BOOLEAN, currentLine};
                } else if (lexeme == "String") {
                    return {lexeme, STRINGTYPE, currentLine};
                } else if (lexeme == "new") {
                    return {lexeme, NEW, currentLine};
                }
                // Keywords start from 3
            } else {
                return {lexeme, ID, currentLine};
            }
        }

        if (isdigit(currentChar)) {
            string lexeme = getNumericLiteral();
            return {lexeme, NUM, currentLine};
        }

        switch (currentChar) {
            case ';': currentChar = getNextChar(); return {";", SEMI, currentLine};
            case ',': currentChar = getNextChar(); return {",", COMMA, currentLine};
            case '.': currentChar = getNextChar(); return {".", DOT, currentLine};
            case '(': currentChar = getNextChar(); return {"(", LPAREN, currentLine};
            case ')': currentChar = getNextChar(); return {")", RPAREN, currentLine};
            case '{': currentChar = getNextChar(); return {"{", LBRACE, currentLine};
            case '}': currentChar = getNextChar(); return {"}", RBRACE, currentLine};
            case '[': currentChar = getNextChar(); return {"[", LBRACK, currentLine};
            case ']': currentChar = getNextChar(); return {"]", RBRACK, currentLine};
            case '+':
                currentChar = getNextChar();
                if (currentChar == '+') {
                    currentChar = getNextChar();
                    return {"++", INC, currentLine};
                }
                return {"+", PLUS, currentLine};
            case '-':
                currentChar = getNextChar();
                if (currentChar == '-') {
                    currentChar = getNextChar();
                    return {"--", DEC, currentLine};
                }
                return {"-", MINUS, currentLine};
            case '=':
                currentChar = getNextChar();
                if (currentChar == '=') {
                    currentChar = getNextChar();
                    return {"==", EQ, currentLine};
                }
                return
            {"=", ASSIGN, currentLine};
            case '!':
                currentChar = getNextChar();
                if (currentChar == '=') {
                    currentChar = getNextChar();
                    return {"!=", NEQ, currentLine};
                }
                return {"!", NOT, currentLine};
            case '<':
                currentChar = getNextChar();
                if (currentChar == '=') {
                    currentChar = getNextChar();
                    return {"<=", LEQ, currentLine};
                }
                return {"<", LT, currentLine};
            case '>':
                currentChar = getNextChar();
                if (currentChar == '=') {
                    currentChar = getNextChar();
                    return {">=", GEQ, currentLine};
                }
                return {">", GT, currentLine};
            case '*': currentChar = getNextChar(); return {"*", MULTIPLY, currentLine};
            case '/':
                currentChar = getNextChar();
                if (currentChar == '/') {
                    while (currentChar != '\n' && currentChar != '\0') {
                        currentChar = getNextChar();
                    }
                    return getNextToken();
                } else if (currentChar == '*') {
                    char prevChar = '\0';
                    while (!(prevChar == '*' && currentChar == '/') && currentChar != '\0') {
                        prevChar = currentChar;
                        currentChar = getNextChar();
                    }
                    currentChar = getNextChar(); // Consume '/'
                    return getNextToken();
                } else {
                    return {"/", DIVIDE, currentLine};
                }
            case '%': currentChar = getNextChar(); return {"%", MOD, currentLine};
            case '&': currentChar = getNextChar(); return {"&&", AND, currentLine};
            case '|': currentChar = getNextChar(); return {"||", OR, currentLine};
            case '"': return {getStringLiteral(), STRING,currentLine};

        }

        // Unknown token
        unknown_char(currentChar,currentLine);
        currentChar = getNextChar();
        return {"", UNKNOWN, currentLine};

    }
};




pair<vector<Token>,bool> getTokenList(string filename) {
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error: Could not open input file." << endl;
        exit(1);
    }

    string sourceCode((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    Lexer lexer(sourceCode);
    vector<Token> T;
    Token token;

    do {
        token = lexer.getNextToken();
        T.push_back(token);
    } while (token.type != EOFF);

    pair<vector<Token>,bool> an={T,valid};
    return an;
    
}


#endif
