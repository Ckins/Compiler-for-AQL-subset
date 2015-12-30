#ifndef SPAN_H
#define SPAN_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

/*
Span:
records the info of a span
# int start_pos_                    : the span's start position in the article
# int end_pos_                      : the span's end position in the article
# string value_                     : the content of the span
# vector<int> pattern_col_marks     : 
# int word_start_pos_               : a span may include more than one token, so this variable records the No. of the first token in the span
# int word_end_pos_                 : it records the No. of the last token in the span

* void display()                 : print the span
* bool operator==(const Span&)     : judge if two span are same (having same start_pos_ and end_pos_)
*/

class Span {
  public:
    int start_pos_;
    int end_pos_;
    string value_;
    vector<int> pattern_col_marks;

    int word_start_pos_;
    int word_end_pos_;

    Span() { }
    Span(int s = 0, int e = 0, string v = "") :
        start_pos_(s), end_pos_(e), value_(v) {}
    ~Span() { } 

    void display() {
        cout << value_ << "(" << start_pos_ << "," << end_pos_ << ") " 
            "word_pos : " << word_start_pos_ <<" to " << word_end_pos_ << endl;
    }

    bool operator==(const Span &sp) {
        return (this->start_pos_ == sp.start_pos_ && this->end_pos_ == sp.end_pos_);
    }

};

/*

GroupRecord:
records the info of a group (this is a redundant copy, because it is also recorded in class PatternGroup)
# int group_num_      : the No. of the group (0 means the whole pattern)
# string column_id_   : the name of the group


*/

class GroupRecord {
  public:
    int group_num_;
    string column_id_;
    GroupRecord() { }
    GroupRecord(int n, string id) :
        group_num_(n), column_id_(id) { }
    ~GroupRecord() { }
    
};

/*
Atom:
records the info of a column (this is similiar to class column, but we use this in pattern)
# int type_            : the type(Tag) of the column (including regex, a column of a view and token)
# int repeat_min_      : the min repeat time (the first num in a {n, m})
# int repeat_max_      : the max repeat time (the second num in a {n, m})
# string view_alias_   : the alias of the atom
# string col_name_     : the name of the atom
# string regex_expr_   : if the atom is a regex, it records the regular expression

*/

class Atom {
public:
    Atom() { 
        repeat_min_ = 1;
        repeat_max_ = 1;
    }
    ~Atom() { };
    int type_;
    int repeat_min_;
    int repeat_max_;
    string view_alias_;
    string col_name_;
    string regex_expr_;
};

/*
PatternGroup:
records the detailed info of a group (surrounded by a pair of paren)
# int group_num_                  : the No. of the group
# string column_id_               : the name of the group
# int start_col_seq_              : the No. of the start atom of the group (begin with 0, from left to right in the AQL)
# int end_col_seq_                : the No. of the end atom of the group
# vector<Atom> content_atoms_     : all atoms of the group
*/

class PatternGroup {
public:
    PatternGroup() { };
    ~PatternGroup() { };
    int group_num_;
    string column_id_; // assigned by group Record
    int start_col_seq_;
    int end_col_seq_;
    vector<Atom> content_atoms_;
};

#endif