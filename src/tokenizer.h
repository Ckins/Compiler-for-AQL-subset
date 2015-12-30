#ifndef TOKENIZER_H
#define TOKENIZER_H 

#include <string>
#include <vector>
#include <cstring>

using namespace std;

/*
DocToken:
records the info of every token in an article:
# int start_pos_   : the token's start position in the article
# int end_pos_     : the token's start position in the article
# string content_  ：the content of the token
# int word_num_    : the NO. of the token (the first token int the article is 1)
*/


class DocToken {
  public:
    int start_pos_;
    int end_pos_;
    std::string content_;
    int word_num_;
    DocToken(int s = 0, int e = 0, std::string c = "", int w = 0) : 
        start_pos_(s), end_pos_(e), content_(c), word_num_(w) { }
};


/*
Tokenizer:
records an article and cut it into DocTokens:
# int word_num_                     : the quantity of tokens in the article
# vector<DocToken> doc_vector_      : a vector recording all tokens of the article
# string file_name_                 : the name of the article
# whole_string_                     : the whole article in string

* void make_token()                            : handle the article from the beginning 
* void display() const                         : show all tokens and its No.
* vector<DocToken> get_doc_token_list() const  : get all tokens in vector
* bool is_num_or_letter(char&)               : judge whether a char is a num or letter
* string get_all_as_string() const             : returns the whole article in string

*/

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