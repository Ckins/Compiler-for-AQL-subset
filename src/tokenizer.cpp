#include "tokenizer.h"
#include <fstream>
#include <cstdio>
#include <iostream>

using namespace std;

Tokenizer::Tokenizer(const char* file) {
    std::string tmp(file);
    file_name_ = tmp;
    whole_string_ = "";
    make_token();
}

bool Tokenizer::is_num_or_letter(char& c) {
    return ((c <= '9' && c >= '0') || (c <= 'Z' && c >= 'A') ||
        (c <= 'z' && c >= 'a'));
}

void Tokenizer::display() {
    cout << doc_vector_.size() << endl;
    std::vector<DocToken>::iterator it = doc_vector_.begin();
    for (; it != doc_vector_.end(); it++) {
        std::cout << (*it).content_ << "(" << (*it).start_pos_ << ", " << (*it).end_pos_ << ")\n";
    }
}

string Tokenizer::get_all_as_string() const{
    return whole_string_;
}

void Tokenizer::make_token() {
    std::ifstream ifile;
    ifile.open(("./../dataset/"+file_name_).c_str());
    
    // @param pos : locate the postion in the document
    char single_char;
    int pos = 1;
    int token_start = 1;
    std::string token_content = std::string("");

    // read char by char to get token
    while (!ifile.eof()) {
        single_char = ifile.get();
        if (!ifile.eof()) whole_string_ += single_char;
        //std::cout << token_content << std::endl;
        if (is_num_or_letter(single_char)) {
            token_content += single_char;
        } else {
            doc_vector_.push_back(DocToken(token_start, pos, token_content));
            if (single_char == ' ' || single_char == '\n') {
                token_content = std::string("");
                token_start = pos+1;
            } else {
                token_content = std::string("");
                token_content += single_char;
                token_start = pos;
            }
        }
        pos++;
    }

    ifile.close();
}

std::vector<DocToken> Tokenizer::get_doc_token_list() const {
    return doc_vector_;
}        
            