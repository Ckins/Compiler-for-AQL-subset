#ifndef COLUMN_H
#define COLUMN_H

#include "span.h"

using namespace std;

/*
Column:
records the info of a column
# vector<Span> span_list_   : all spans of the column
# string col_name_          : the name of the column
# int col_width_            : the max width of the column (uesd in print)

* vector<Span> get_span_list()  : returns all spans of the column
* string get_name()             : returns the name of the column
* void set_name(string)         : set a name to the column
* void add_span(Span)           : add a span to the column
* void set_list(vector<Span>)   : set a span list to the column (drop the original one)
* int calculate_col_width()     : calculate the max length of all spans in the column
* int get_col_width()           : returns the max length of the column
* string to_string(int)         : conver a num to a string
*/

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
    int calculate_col_width();
    int get_col_width();
    string to_string(int num);

  private:
    std::vector<Span> span_list_;
    string col_name_;
    int col_width_;
};

#endif