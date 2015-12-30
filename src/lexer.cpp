#include <fstream>
#include <cstdio>
#include <iostream>
#include "lexer.h"
#include "code_token.h"

int Lexer::line_ = 1;

Lexer::Lexer(const char* file) {
    peek_ = ' ';
    is_end_ = false;
    file_ = string(file);

    instream_.open((string("./../dataset/")+file).c_str());

    // reserve words
    reserve(CodeToken(string("create"), Tag::CREATE));
    reserve(CodeToken(string("view"), Tag::VIEW));
    reserve(CodeToken(string("as"), Tag::AS));
    reserve(CodeToken(string("output"), Tag::OUTPUT));
    reserve(CodeToken(string("select"), Tag::SELECT));
    reserve(CodeToken(string("from"), Tag::FROM));
    reserve(CodeToken(string("extract"), Tag::EXTRACT));
    reserve(CodeToken(string("regex"), Tag::REGEX));
    reserve(CodeToken(string("on"), Tag::ON));
    reserve(CodeToken(string("return"), Tag::RETURN));
    reserve(CodeToken(string("group"), Tag::GROUP));
    reserve(CodeToken(string("and"), Tag::AND));
    reserve(CodeToken(string("Token"), Tag::TOKEN));
    reserve(CodeToken(string("pattern"), Tag::PATTERN));

    while(!is_end_) {
        CodeToken tmp = scan();
        if (!instream_.eof()) {
            code_token_list_.push_back(tmp);
        }
    }

    instream_.close();
}

Lexer::~Lexer() {
    //nothing
}

void Lexer::readch() {
    if (!instream_.eof()) {
        peek_ = instream_.get();
    } else {
        is_end_ = true;
        peek_ = ' ';
    }
}

void Lexer::reserve(CodeToken w) {
    words_[w.lexme_] = w;
}

bool Lexer::is_letter(char c) {
    if (c <= 'z' && c >= 'a') return true;
    if (c <= 'Z' && c >= 'A') return true;
    return false;
}

bool Lexer::is_letter_or_num(char c) {
    return ((c <= '9' && c >= '0') || (c <= 'Z' && c >= 'A') ||
        (c <= 'z' && c >= 'a'));
}

CodeToken Lexer::scan() {
    for ( ;!is_end_; readch()) {
        if (peek_ == ' ' || peek_ == '\t') continue;
        else if (peek_ == '\n') line_++;
        else break;
    }

    //recognise number
    if (peek_ >= '0' && peek_ <= '9') {
        int v = 0;
        do {
            v = 10*v + (peek_-'0');
            readch();
        } while (peek_ >= '0' && peek_ <= '9');
        CodeToken num(v);
        num.line_ = line_;
        return num;
    }

    // recognise identifier
    if (is_letter(peek_)) {
        string b = "";
        do {
            b += peek_;
            readch();
        } while (is_letter_or_num(peek_));
        if (words_.find(b) != words_.end()) {
            CodeToken tmpWord = words_[b];
            tmpWord.line_ = line_;
            return tmpWord;
        }
        CodeToken w(b, Tag::ID);
        w.line_ = line_;
        return w;
    }

    //recognise regex expression
    if (peek_ == '/') {
        int dead = 0;
        string reg = "";
        char last_char = '/';
        readch();
        do {
            reg += peek_;
            last_char = peek_;
            readch();
        } while (!((peek_ == '/') && (last_char != '\\')));
        peek_ = ' ';
        CodeToken reg_expr(reg, Tag::REGEX_EXPR);
        reg_expr.line_ = line_;
        return reg_expr;
    }

    // the single token like "("
    CodeToken tok(peek_);
    tok.line_ = line_;
    peek_ = ' ';
    return tok;
}

vector<CodeToken> Lexer::get_list() {
    return code_token_list_;
}
