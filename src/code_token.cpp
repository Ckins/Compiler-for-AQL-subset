#include "code_token.h"

CodeToken::CodeToken() {
    tag_ = -1;
    line_ = -1;
    num_value_ = -999;
    lexme_ = string("");
}

CodeToken::CodeToken(char c) {
    tag_ = Tag::SINGLE_TOKEN;
    lexme_ = string("")+c;
    num_value_ = -999;
    line_ = -1;
}

CodeToken::CodeToken(string s, const int tag) {
    tag_ = tag;
    lexme_ = s;
    num_value_ = -999;
    line_ = -1;
}

CodeToken::CodeToken(int val) {
    num_value_ = val;
    tag_ = Tag::NUM;
    lexme_ = string("");
    line_ = -1;
}

void CodeToken::set(CodeToken* p1, CodeToken* p2) {
    p1->num_value_ = p2->num_value_;
    p1->tag_ = p2->tag_;
    p1->lexme_ = p2->lexme_;
    p1->line_ = p2->line_;
}

CodeToken& CodeToken::operator=(const CodeToken &tok) {
    set(this, (CodeToken*)&tok);
    return *this;
}

CodeToken::CodeToken(const CodeToken &tok) {
    this->num_value_ = tok.num_value_;
    this->tag_ = tok.tag_;
    this->lexme_ = tok.lexme_;
    this->line_ = tok.line_;
}

string CodeToken::toString() {
    // number
    if (tag_ == Tag::NUM) {
        string Result;
        stringstream convert;
        convert << num_value_;
        Result = convert.str();
        return Result;
    } else {
        return lexme_;
    }
}