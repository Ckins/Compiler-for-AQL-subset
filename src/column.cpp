#include "column.h"

Column::Column() {
    col_name_ = "";
}

Column::Column(vector<Span> span_list, string name) {
    span_list_ = span_list;
    col_name_ = name;
}

Column::~Column() {

}

vector<Span> Column::get_span_list() {
    return span_list_;
}

void Column::set_name(string name) {
    col_name_ = name;
}

string Column::get_name() {
    return col_name_;
}

void Column::add_span(Span token) {
    span_list_.push_back(token);
}

void Column::set_list(vector<Span> v) {
    span_list_ = v;
}
