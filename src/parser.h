#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include "code_token.h"
#include "lexer.h"
#include "tokenizer.h"
#include "view.h"
#include "column.h"
#include "span.h"

using namespace std;

class Parser {
  public:
    Parser(vector<CodeToken> lexer_list, const Tokenizer &t);
    ~Parser();

    //basic step
    bool scan();
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

    //data operation
    View get_view_by_alias(string alias);
    View get_view_by_name(string name);
    void reset_all_view_alias();
    void print_view(View output_view);

    //auxiliary function
    bool peek_is_match(const char*reserve);
    bool peek_has_type_of(int tag);
    void error(string func);

  private:
    vector<CodeToken> lexer_list_;
    vector<DocToken> doc_token_list_;

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