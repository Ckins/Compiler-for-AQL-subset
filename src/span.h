#ifndef SPAN_H
#define SPAN_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

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

class GroupRecord {
  public:
    int group_num_;
    string column_id_;
    GroupRecord() { }
    GroupRecord(int n, string id) :
        group_num_(n), column_id_(id) { }
    ~GroupRecord() { }
    
};

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