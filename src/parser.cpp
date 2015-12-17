#include "parser.h"
#include <string>
#include <exception>
#include <iostream>
#include <map>

extern vector<vector<int> >
findall(const char *regex, const char *content);

Parser::Parser(vector<CodeToken> lexer_list, const Tokenizer &t) {
    lexer_list_ = lexer_list;
    doc_token_list_ = t.get_doc_token_list();
    peek_pos_ = 0;
    is_end_ = false;
    first_print_ = true;
    file_name_ = t.file_name_;

    //create initial view as Document D.text
    Column text;
    text.set_name("text");
    string str = t.get_all_as_string();
    // cout << str << endl;
    Span s(0, str.length(), str);
    text.add_span(s);
    View Document("Document");
    Document.add_column(text);
    view_list_.push_back(Document);

    analyse_program();
}

Parser::~Parser() {
    //do nothing 
}

//announce error
void Parser::error(string func) {
    if (!is_end_) {
        cout << "Error in line: " << peek_.line_ << endl
            << " the Token is: " << peek_.toString() << endl
            << "call by " << func << endl;
        exit(0);
    }
}

// to ensure not null analysis
void Parser::analyse_program() {
    while (!is_end_) {
        analyse_aql_stmt();
    }
}

/*
*aql_stmt → create_stmt; 
*           |output_stmt ;
*/
void Parser::analyse_aql_stmt() {
    reset_all_view_alias();
    scan();
    if (peek_is_match("create")) {
        //cout << "create stmt" << endl;
        analyse_create_stmt();
    } else if (peek_is_match("output")) {
        analyse_output_stmt();
    } else {
        error("analyse_aql_stmt()");
    }
}


/*
*output_stmt → output view ID alias
*
*/
void Parser::analyse_output_stmt() {
    scan();
    View output_view;
    // cout << peek_.toString() << endl;
    if (scan() && peek_has_type_of(Tag::ID)) {
        // cout << peek_.toString() << endl;
        output_view = get_view_by_name(peek_.toString());
    }

    if (scan() && peek_is_match("as")) {
        if (scan() && peek_has_type_of(Tag::ID)) {
            output_view.set_alias(peek_.toString());
        }
    }
    print_view(output_view);
    if (scan() && peek_is_match(";")) {
        // success
    }
}

void Parser::print_view(View output_view) {
    if (first_print_) {
        first_print_ = false;
        cout << "Processing " << file_name_ << endl;
    }
    if (output_view.get_alias().length() == 0) {
        cout << "View: "<< output_view.get_name() << endl;
    } else {
        cout << "View: "<< output_view.get_alias() << endl;
    }

    
}

/*
*create_stmt → create  view ID 
*                as    view_stmt
*/
void Parser::analyse_create_stmt() {
    // create view PerLoc as
    scan();
    if (!peek_is_match("view")) {
        error("analyse_create_stmt()");
    } else {
        scan();
    }

    CodeToken view_id;
    if (peek_.tag_ != Tag::ID) {
        error("analyse_create_stmt()");
    } else {
        view_id = peek_;
        scan();
    }
    
    if (!peek_is_match("as")) {
        error("analyse_create_stmt()");
    } else {
        View result_from_stmt(view_id.toString());
        vector<Column> c = analyse_view_stmt();
        result_from_stmt.set_list(c);
        view_list_.push_back(result_from_stmt);
    }
}

/*
*view_stmt → select_stmt 
*            | extract_stmt
*/
vector<Column> Parser::analyse_view_stmt() {

    vector<Column> result_vector;

    scan();
    if (peek_is_match("select")) {
        //cout << "select stmt" << endl;
        
    } else if (peek_is_match("extract")) {
        //cout << "extract stmt" << endl;
        result_vector = analyse_extract_stmt();
    } else {
        error("analyse_view_stmt()");
    }
    
    return result_vector;
}


/*
*extract_stmt → extract   extract_spec 
*                from     from_list
*/
vector<Column> Parser::analyse_extract_stmt() {
    scan();
    vector<Column> result_vector;

    //extract_spec
    if (peek_is_match("regex")) {

        //analyse from_list first to determine the alias names
        int tmp_pos = peek_pos_;
        analyse_from_list();
        peek_pos_ = tmp_pos;

        result_vector = (analyse_regex_spec());
    } else if (peek_is_match("pattern")) {
        cout << "pattern_spec" << endl;
        
    } else {
        error("analyse_extract_stmt()");
    }

    return result_vector;
}

/*
*regex_spec → regex REG 
*              on   column  
*                   name_spec
*/
vector<Column> Parser::analyse_regex_spec() {
    vector<Column> result_vector_of_column;

    //get REG
    string reg = "";

    //remember the regex codeToken to analyse later
    if (scan() && peek_.tag_ == Tag::REGEX_EXPR) {
        reg = peek_.toString();
    }

    if (!(scan() && peek_is_match("on"))) error("analyse_regex_spec()");
    //get colomn_stmt, and remember them, 
    //View name, Col name
    vector<CodeToken> colomn_ids = analyse_colomn(); 

    //analyse name_spec, and remember
    vector<GroupRecord> group_records = analyse_name_spec();

    //really extract here
    //get the exact col and view first
    View source_view = get_view_by_alias(colomn_ids[0].toString());
    Column source_column = source_view.get_column_by_name(colomn_ids[1].toString());

    if (group_records.size() == 1) {

        Column single_column;
        single_column.set_name(group_records[0].colomn_id_);

        vector<vector<int> >result_from_engine;

        vector<Span>::iterator span_it = source_column.get_span_list().begin();
        vector<Span>::iterator span_end = source_column.get_span_list().end();

        for (; span_it != span_end; span_it++) {
            string str = (*span_it).value_;
            result_from_engine = findall(reg.c_str(), str.c_str());
            //cout << "result_from_engine : " <<result_from_engine.size() << endl;
            int length = result_from_engine.size();
            for (int i = 0; i < length; i++) {

                int j;
                string match_span = "";
                for (j = result_from_engine[i][0]; j < result_from_engine[i][1]; j++) {
                    match_span += str[j];
                }
                //cout << match_span << result_from_engine[i][0] << j << endl;
                Span tmp_span(result_from_engine[i][0], j, match_span);
                single_column.add_span(tmp_span);
            }
        }
        result_vector_of_column.push_back(single_column);
        
    }

    //skip from_list stmt;
    analyse_from_list();
    
    return result_vector_of_column;
}


/*
*name_spec → as ID | return group_spec
*
*group_spec → single_group | group_spec and single_group
*/
vector<GroupRecord> Parser::analyse_name_spec() {
    vector<GroupRecord> records;
    scan();
    if (peek_is_match("as")) {
        //cout << "for short in name spec " << endl;
        if (scan() && peek_has_type_of(Tag::ID)) {
            GroupRecord record(0, peek_.toString());
            records.push_back(record);
        } else {
            error("analyse_name_spec()");
        }

    } else if (peek_is_match("return")) {
        //cout << "many group_spec " << endl;
        while (scan()) {
            records.push_back(analyse_single_group());
            scan();
            if (!peek_is_match("and")) break;
        }
    } else {
        error("analyse_name_spec()");
    }
    return records;
}

/*
*single_group → group NUM as ID
*
*/

GroupRecord Parser::analyse_single_group() {
    while ((!peek_has_type_of(Tag::NUM))) {
        // in order to skip "and" "group"
        scan();
    }
    GroupRecord record;
    record.group_num_ = peek_.num_value_;
    cout << record.group_num_ << endl;
    if (!(scan() && peek_is_match("as"))) {
        error("analyse_single_group()");
    }
    scan();
    record.colomn_id_ = peek_.toString();
    cout << record.colomn_id_ << endl;
    return record;
}

/*
*column → ID . ID
*
*/
vector<CodeToken> Parser::analyse_colomn() {

    vector<CodeToken> colomn_ids;

    if (!(scan() && peek_has_type_of(Tag::ID))) {
        error("analyse_colomn()1");
    }

    colomn_ids.push_back(peek_);

    if (!(scan() && peek_is_match("."))) {
        error("analyse_colomn()2");
    }

    if (!(scan() && peek_has_type_of(Tag::ID))) {
        error("analyse_colomn()3");
    }

    colomn_ids.push_back(peek_);
    return colomn_ids;
}

/*
*from_list → from_item | from_list , from_item
*
*/
void Parser::analyse_from_list() {
    
    while (!peek_is_match("from")) {
        //cout << peek_.toString() << endl;
        scan();
    }
    while (!peek_is_match(";")) {
        analyse_from_item();
        if (peek_is_match(",") || peek_is_match(";")) {
            // do noting
        } else {
            error("analyse_from_list()");
        }
    }
}

/*
*from_item → ID ID
*
*/
void Parser::analyse_from_item() {
    scan();
    string origin_name;
    string alias_name;
    if (peek_.tag_ != Tag::ID) {
        error("analyse_from_item()1");
    } else {
        origin_name = peek_.toString();
        scan();
    }

    if (peek_.tag_ != Tag::ID) {
        error("analyse_from_item()2");
    } else {
        alias_name = peek_.toString();
        scan();
    }

    vector<View>::iterator it = view_list_.begin();

    for (; it != view_list_.end(); it++) {

        if ((*it).get_name() == origin_name) {
            (*it).set_alias(alias_name);
            //cout << it->get_name() << " has alias: " << it->get_alias() << endl;
            break;
        }
    }
}

// to cmp the peek_ lexme with reserve word
bool Parser::peek_is_match(const char* reserve) {
    string cmp(reserve);
    return (peek_.toString() == cmp);
}

/*to move peek_ forward
* return false if reach an end
*
*/
bool Parser::scan() {
    int length = lexer_list_.size();
    if (peek_pos_ < length) {
        peek_ = lexer_list_[peek_pos_++];
        return true;
    } else {
        is_end_ = true;
    }
    return false;
}

bool Parser::peek_has_type_of(int tag) {
    return (peek_.tag_ == tag);
}

View Parser::get_view_by_alias(string alias) {
    View v;
    vector<View>::iterator it = view_list_.begin();
    for (; it != view_list_.end();it++) {
        if ((*it).get_alias() == alias) {
            v = (*it);
            break;
        }
    }
    return v;
}

View Parser::get_view_by_name(string name) {
    View v;
    vector<View>::iterator it = view_list_.begin();
    for (; it != view_list_.end();it++) {
        if ((*it).get_name() == name) {
            v = (*it);
            break;
        }
    }
    return v;
}

void Parser::reset_all_view_alias() {
    vector<View>::iterator it = view_list_.begin();
    for (; it != view_list_.end();it++) {
        if ((*it).get_alias().length() > 0) {
            (*it).set_alias(string(""));
        }
    }
}
