#ifndef SPAN_H
#define SPAN_H

#include <string>
#include <vector>

using namespace std;

class Span {
  public:
    int start_pos_;
    int end_pos;
    string value_;
    Span() { }
    Span(int s = 0, int e = 0, string v = "") :
        start_pos_(s), end_pos(e), value_(v) {}
    ~Span() { } 
};

class GroupRecord {
  public:
    int group_num_;
    string colomn_id_;
    GroupRecord() { }
    GroupRecord(int n, string id) :
        group_num_(n), colomn_id_(id) { }
    ~GroupRecord() { }
    
};

#endif