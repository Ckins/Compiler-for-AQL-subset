#ifndef VIEW_H
#define VIEW_H

#include "column.h"

using namespace std;

/*
View:
records the info of a view
# vector<Column> column_list_    : all columns of the view
# string view_name_              : the name of the view
# string view_alias              : the alial of the view

* vector<Column> get_column_list()     : returns all columns of the view
* void add_column(Column)              : add a column to the view
* void set_list(vector<Column>)        : set a column list to the view (drop the original one)
* void set_alias(string name)          : set a alias to the view
* string get_name()                    : returns the name of the view
* string get_alias()                   : returns the alias of the view
* Column get_column_by_name(string)    : returns a column of the view with a given name
* int calculate_row_num()              : calculate the max size of spans of all columns (used in print)
* bool is_empty()                      : judge if all columns are empty (uesd in print)
*/

class View {
  public:
    View();
    View(string name);
    ~View();
    vector<Column> get_column_list();
    void add_column(Column col);
    void set_list(vector<Column> column_list);
    void set_alias(string name);
    string get_name();
    string get_alias();
    Column get_column_by_name(string name);
    int calculate_row_num();
    bool is_empty();

  private:
    std::vector<Column> column_list_;
    string view_name_;
    string view_alias;
};

#endif