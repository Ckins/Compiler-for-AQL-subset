#include "column.h"
#include "code_token.h"
#include <string>
#include <sstream>

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

int Column::calculate_col_width() {
    int length = span_list_.size();
    int max = col_name_.length()+2;
    for (int i = 0; i < length; i++) {
        int tmp = span_list_[i].value_.length()+to_string(span_list_[i].start_pos_).length()+
            to_string(span_list_[i].end_pos_).length()+6;
        if (tmp > max) {
            max = tmp;
        }
    }
    col_width_ = max;
    return col_width_;
}

int Column::get_col_width() {
    return col_width_;
}
