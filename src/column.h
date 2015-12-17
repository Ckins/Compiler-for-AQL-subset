#ifndef COLUMN_H
#define COLUMN_H

#include "span.h"

using namespace std;

class Column{
  public:
    Column();
    Column(vector<Span> span_list, string name);

    ~Column();
    vector<Span> get_span_list();
    string get_name();
    void set_name(string name);
    void add_span(Span token);
    void set_list(vector<Span> v);

  private:
    std::vector<Span> span_list_;
    string col_name_;
};

#endif