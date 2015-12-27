#ifndef TOKENIZER_H
#define TOKENIZER_H 

#include <string>
#include <vector>
#include <cstring>

using namespace std;

class DocToken {
  public:
    int start_pos_;
    int end_pos_;
    std::string content_;
    int word_num_;
    DocToken(int s = 0, int e = 0, std::string c = "", int w = 0) : 
        start_pos_(s), end_pos_(e), content_(c), word_num_(w) { }
};

class Tokenizer {
  public:
    int word_num_;
    vector<DocToken> doc_vector_;
    string file_name_;
    string whole_string_;

    Tokenizer(std::string file);

    void make_token();

    void display() const;

    bool is_num_or_letter(char& c);

    vector<DocToken> get_doc_token_list() const;

    string get_all_as_string() const;
}; 

#endif