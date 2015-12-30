#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <cstdio>
#include <fstream>
#include <vector>
#include "code_token.h"

using namespace std;

/*
Lexer:
cut an AQL into tokens
# static int line_                         : the No. of reading line
# string file_                             : the name of the file
# char peek_                               : the handling char
# bool is_end_                             : it will be true if reaches the EOF
# std::ifstream instream_                  : the ifstream of the file
# map<string, CodeToken> words_            : the reserved words
# vector<CodeToken> code_token_list_       : records all tokens of the given AQL

* void readch()                            : move to the next char 
* void reserve(CodeToken)                  : handle reserved words
* bool is_letter(char)                     : judge whether the char is a letter
* bool is_letter_or_num(char)              : judge whether the char is a letter or a num
* CodeToken scan()                         : get the next token
* vector<CodeToken> get_list()             : returns all tokens of the given AQL

*/

class Lexer {
public:
    static int line_;
    string file_;
    char peek_;
    bool is_end_;
    std::ifstream instream_;
    map<string, CodeToken> words_;
    std::vector<CodeToken> code_token_list_;

    Lexer(const char* file);
    ~Lexer();
    void readch();
    void reserve(CodeToken w);
    //bool readch(char c);
    bool is_letter(char c);
    bool is_letter_or_num(char c);
    CodeToken scan();
    vector<CodeToken> get_list();
};

#endif