#include "view.h"

View::View() {
    view_name_ = "";
}

View::~View() {

}

View::View(string name) {
    view_name_ = name;
}

vector<Column> View::get_column_list() {
    return column_list_;
}

void View::add_column(Column col) {
    column_list_.push_back(col);
}

void View::set_list(vector<Column> column_list) {
    column_list_ = column_list;
}

void View::set_alias(string name) {
    view_alias = name;
}

string View::get_name() {
    return view_name_;
}

string View::get_alias() {
    return view_alias;
}

Column View::get_column_by_name(string name) {
    Column col;
    vector<Column>::iterator it = column_list_.begin();
    for (; it != column_list_.end(); it++) {
        if ((*it).get_name() == name) {
            col = (*it);
            break;
        }
    }
    return col;
}

int View::calculate_row_num() {
    int res = 0;
    unsigned int len = column_list_.size();
    for (unsigned int i = 0; i < len; i++) {
        int tmp_size = column_list_[i].get_span_list().size();
        if (tmp_size > res) {
            res =tmp_size;
        }
    }
    return res;
}
