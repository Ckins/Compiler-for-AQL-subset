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
    DocToken(int s = 0, int e = 0, std::string c = 0) : 
        start_pos_(s), end_pos_(e), content_(c) {}
};

class Tokenizer {
  public:
    vector<DocToken> doc_vector_;
    string file_name_;
    string whole_string_;

    Tokenizer(const char* file);

    void make_token();

    void display();

    bool is_num_or_letter(char& c);

    vector<DocToken> get_doc_token_list() const;

    string get_all_as_string() const;
}; 

#endif