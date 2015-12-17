#ifndef VIEW_H
#define VIEW_H

#include "column.h"

using namespace std;

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

  private:
    std::vector<Column> column_list_;
    string view_name_;
    string view_alias;
};

#endif