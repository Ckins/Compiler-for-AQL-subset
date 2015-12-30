#ifndef PARSER_H
#define PARSER_H

#define DEBUG false

#include <iostream>
#include <vector>
#include "code_token.h"
#include "lexer.h"
#include "tokenizer.h"
#include "view.h"
#include "column.h"
#include "span.h"
#include <sstream>

using namespace std;

/*
A Detailed Document of aql_master

Parser:
    The detail of each function is described in parser.cpp

*/

class Parser {
  public:
    Parser(vector<CodeToken> lexer_list, const Tokenizer &t);
    ~Parser();

    //basic step
    bool scan();
    void step_back();
    void analyse_program();
    
    //language analyse
    void analyse_aql_stmt();
    void analyse_create_stmt();
    void analyse_output_stmt();
    vector<Column> analyse_view_stmt();
    vector<Column> analyse_extract_stmt();
    vector<Column> analyse_regex_spec();
    void analyse_from_list();
    void analyse_from_item();
    vector<CodeToken> analyse_colomn();
    vector<GroupRecord> analyse_name_spec();
    GroupRecord analyse_single_group();

    vector<Column> analyse_select_stmt();
    vector<Column> analyse_select_list();
    Column analyse_select_item();

    //analyse pattern
    vector<Column> analyse_pattern_spec();
    vector<PatternGroup> analyse_pattern_expr();
    PatternGroup analyse_pattern_pkg();
    // PatternGroup analyse_pattern_group();
    Atom analyse_atom();
    Column get_column_from_tokenizer();
    Column get_column_as_regex(string regex);

    void calculate_span_word_seq(Column &content_col);
    vector<Span> get_standard_word_span_list_();

    //data operation
    View get_view_by_alias(string alias);
    View get_view_by_name(string name);
    void reset_all_view_alias();

    //display
    void print_view(View& output_view);
    void print_format_line(View& output_view);
    void print_format_column(View& output_view);
    void print_format_span(View& output_view);

    //auxiliary function
    bool peek_is_match(const char*reserve);
    bool peek_has_type_of(int tag);

    bool assert_next_peek_is_match(const char*reserve);
    bool assert_next_peek_has_type(int tag);
    void error(string func);
    string to_string(int num);

  private:
    vector<CodeToken> lexer_list_;
    vector<DocToken> doc_token_list_;
    vector<Span> standard_word_span_list_;

    //for scan
    bool is_end_;
    int peek_pos_;
    CodeToken peek_;

    //store data
    std::vector<View> view_list_;
    bool first_print_;
    string file_name_;
};

#endif