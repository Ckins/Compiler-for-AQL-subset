#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <cstdio>
#include <fstream>
#include <vector>
#include "code_token.h"

using namespace std;

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