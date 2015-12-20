#include "parser.h"
#include <string>
#include <exception>
#include <iostream>
#include <iomanip>
#include <map>

extern vector<vector<int> >
findall(const char *regex, const char *content);

Parser::Parser(vector<CodeToken> lexer_list, const Tokenizer &t) {

    if (DEBUG) {
        t.display();
    }

    lexer_list_ = lexer_list;
    doc_token_list_ = t.get_doc_token_list();
    peek_pos_ = 0;
    is_end_ = false;
    first_print_ = true;
    file_name_ = t.file_name_;
    peek_ = CodeToken(' ');

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

    standard_word_span_list_ = get_standard_word_span_list_();

    analyse_program();
}

Parser::~Parser() {
    //do nothing 
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
    if (peek_is_match(" ") || peek_is_match(";")) {
        scan();
    }
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

void Parser::print_view(View& output_view) {
    if (first_print_) {
        first_print_ = false;
        cout << "Processing " << file_name_;
    }
    if (output_view.get_alias().length() == 0) {
        cout  << endl << "View: "<< output_view.get_name() << endl;
    } else {
        cout  << endl << "View: "<< output_view.get_alias() << endl;
    }

    print_format_line(output_view);
    print_format_column(output_view);
    print_format_line(output_view);
    print_format_span(output_view);
    print_format_line(output_view);

    if (!output_view.is_empty()) {
        cout << output_view.get_column_list()[0].get_span_list().size();
        cout << " rows in set";
    } else {
        cout << "Empty set";
    }

    // to determin
    cout << endl;
}

//| Georgia:(20,27)    | Plains, Georgia:(12,27)        | Plains:(12,18)       |

void Parser::print_format_span(View& output_view) {
    vector<Column> col_list = output_view.get_column_list();
    unsigned int col_num = col_list.size();
    unsigned int row_num = output_view.calculate_row_num();
    
    for (unsigned int j = 0; j < row_num; j++) {
        cout << '|';
        for (unsigned int i = 0; i < col_num; i++) {
            int col_width = col_list[i].calculate_col_width();
            string single_span = "";
            if (j < col_list[i].get_span_list().size()) {
                single_span = col_list[i].get_span_list()[j].value_+":("
                + SSTR(col_list[i].get_span_list()[j].start_pos_) + ','
                + SSTR(col_list[i].get_span_list()[j].end_pos_) + ')';
            }       
            cout << setiosflags( ios::left );
            cout << ' ' << setw(col_width-1) << single_span;
            cout << '|';
        }
        cout << endl;
    }
}

/*
*print |Loc    |
*/
void Parser::print_format_column(View& output_view) {
    vector<Column> col_list = output_view.get_column_list();
    unsigned int col_num = col_list.size();
    cout << '|';
    for (unsigned int i = 0; i < col_num; i++) {
        int col_width = col_list[i].calculate_col_width();
        cout << setiosflags( ios::left );
        cout << ' ' << setw(col_width-1) << col_list[i].get_name();
        cout << '|';
    }
    cout << endl;
}

/*
*print +---------------+-------+----------
*/
void Parser::print_format_line(View& output_view) {
    vector<Column> col_list = output_view.get_column_list();
    int col_num = col_list.size();
    cout << "+";
    for (int i = 0; i < col_num; i++) {
        int col_width = col_list[i].calculate_col_width();
        //cout << "col_width" <<col_list[i].get_col_width() << endl;
        for (int j = 0; j < col_width; j++) {
            cout << '-';
        }
        cout << '+';
    }
    cout << endl;
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
        // cout << "select stmt" << endl;
        result_vector = analyse_select_stmt();
    } else if (peek_is_match("extract")) {
        //cout << "extract stmt" << endl;
        result_vector = analyse_extract_stmt();
    } else {
        error("analyse_view_stmt()");
    }
    
    return result_vector;
}

/*
*select_stmt → select   select_list 
*               from    from_list
*/
vector<Column> Parser::analyse_select_stmt() {
    vector<Column> result_vector;
    //analyse from_list first to determine the alias names
    int tmp_pos = peek_pos_;
    analyse_from_list();
    peek_pos_ = tmp_pos;

    result_vector = analyse_select_list();
    return result_vector;
}

/*
*select_list → select_item 
            |  select_list , select_item
*/
vector<Column> Parser::analyse_select_list() {
    vector<Column> result_vector;
    if (scan() && peek_has_type_of(Tag::ID)) {
        result_vector.push_back(analyse_select_item());
    }
    while (scan() && peek_is_match(",")) {
        if (scan() && peek_has_type_of(Tag::ID)) {
            result_vector.push_back(analyse_select_item());
        }
    }

    //skip from_list stmt;
    analyse_from_list();

    return result_vector;
}

/*
*select_item → ID . ID alias
*/
Column Parser::analyse_select_item() {
    // cout << "select_item" << endl;
    string view_alias = peek_.toString();
    string origin_col_name = "";
    string new_col_name = "";
    if (scan() && peek_is_match(".")) {
        if (scan() && peek_has_type_of(Tag::ID)) {
            origin_col_name = peek_.toString();
        } else {
            error("analyse_select_item()1");
        }
    } else {
        error("analyse_select_item()2");
    }
    // cout << view_alias << origin_col_name << endl;
    if (scan() && peek_is_match("as")) {
        if (scan() && peek_has_type_of(Tag::ID)) {
            new_col_name = peek_.toString();
        } else {
            error("analyse_select_item()3");
        }     
    }

    View src_view = get_view_by_alias(view_alias);
    Column result_col = src_view.get_column_by_name(origin_col_name);

    if (new_col_name.length() > 0) {
        result_col.set_name(new_col_name);
    }
    // cout << peek_.toString() << endl;
    return result_col;
}


/*
*extract_stmt → extract   extract_spec 
*                from     from_list
*/
vector<Column> Parser::analyse_extract_stmt() {
    scan();
    vector<Column> result_vector;

    //analyse from_list first to determine the alias names
    int tmp_pos = peek_pos_;
    analyse_from_list();
    peek_pos_ = tmp_pos;
    peek_ = lexer_list_[peek_pos_-1];
    //extract_spec
    if (peek_is_match("regex")) {
        
        result_vector = (analyse_regex_spec());
    } else if (peek_is_match("pattern")) {
        if (DEBUG) cout << "pattern_spec" << endl;
        result_vector = analyse_pattern_spec();
    } else {
        error("analyse_extract_stmt()");
    }

    return result_vector;
}

/*
* pattern_spec → pattern pattern_expr 
                 name_spec
*/
vector<Column> Parser::analyse_pattern_spec() {
    vector<Column> result_vector_of_column;

    // remember the pattern_expr in proper database structure
    // record the ()
    vector<PatternGroup> pattern_groups = analyse_pattern_expr();

    //analyse name_spec, and remember
    // record the require!
    vector<GroupRecord> group_records = analyse_name_spec();

    //really analyse here
    //content column fetch, all columns are include in default group 0
    vector<Column> content_base;
    PatternGroup default_group = pattern_groups[0];
    vector<Atom> all_atoms = default_group.content_atoms_;
    for (int i = 0; (unsigned int)i < all_atoms.size(); i++) {
        Column content_col;
        if (all_atoms[i].type_ == Tag::TOKEN) {
            content_col = get_column_from_tokenizer();
        } else if (all_atoms[i].type_ == Tag::REGEX_EXPR) {
            content_col = get_column_as_regex(all_atoms[i].regex_expr_);
        } else if (all_atoms[i].type_ == Tag::ID) {
            View tmp_view = get_view_by_alias(all_atoms[i].view_alias_);
            content_col = tmp_view.get_column_by_name(all_atoms[i].col_name_);
        }
        calculate_span_word_seq(content_col);
        content_base.push_back(content_col);
    }

    if (DEBUG) {
        cout << "\n\n\n\n" << "test content cols\n";
        for (int i = 0; i < content_base.size();i++) {
            for (int j = 0; j < content_base[i].get_span_list().size();j++) {
                content_base[i].get_span_list()[j].display();
            }
        }
        cout << "\n\n\n\n";
    }
    
    //match group num to column_id
    vector<PatternGroup> wanted_groups;
    for (int i = 0; i < group_records.size(); i++) {
        int tmp_num = 0;
        for (int j = 0; j < pattern_groups.size(); j++) {
            if (pattern_groups[j].group_num_ == group_records[i].group_num_) {
                pattern_groups[j].column_id_ = group_records[i].column_id_;
                wanted_groups.push_back(pattern_groups[j]);
            }
        }
    }

    // cout << "wanted_groups size = " << wanted_groups.size() << endl;

    //debug information
    if (DEBUG) {
        for (int i = 0; i < wanted_groups.size(); i++) {
            PatternGroup tmp_group = wanted_groups[i];
            cout << "group_num: " << tmp_group.group_num_ 
                << "group_name: " << tmp_group.column_id_ 
                << " from col " << tmp_group.start_col_seq_ << "to " << tmp_group.end_col_seq_ <<endl;
            for (int j = 0; j < tmp_group.content_atoms_.size();j++) {
                cout << "type: " << tmp_group.content_atoms_[j].type_ <<endl;
                cout << tmp_group.content_atoms_[j].view_alias_ << endl;
                cout << "re-min: " << tmp_group.content_atoms_[j].repeat_min_ <<endl;
                cout << "re-max: " << tmp_group.content_atoms_[j].repeat_max_ <<endl;
            }
        }        
    }

    vector<Span> cmp_span_list;
    //default group analysis 
    string doc_str = get_view_by_name("Document").get_column_by_name("text").get_span_list()[0].value_;
    for (int i = 0; i < 1; i++) {

        // target_span_list contains the result of the linking !!!
        Column group_n = content_base[wanted_groups[i].start_col_seq_];
        group_n.set_name(wanted_groups[i].column_id_);
        vector<Span> target_span_list = group_n.get_span_list();

        for (int j = 0; j < target_span_list.size();j++) {
            target_span_list[j].pattern_col_marks.push_back(target_span_list[j].start_pos_);
            target_span_list[j].pattern_col_marks.push_back(target_span_list[j].end_pos_);
        }

        for (int j = wanted_groups[i].start_col_seq_+1; j < wanted_groups[i].end_col_seq_;j++) {
            Column link_col_behind = content_base[j];
            vector<Span> link_span_list_behind = link_col_behind.get_span_list();

            //minial required, need to erase the unlinkable spans
            for (int k = 0; k < all_atoms[j].repeat_min_;k++) {
                if (DEBUG) {
                    cout << "-----------------repeat min------------------\n" << link_col_behind.get_name() << endl;
                }
                for (int loop_out = 0; loop_out < target_span_list.size(); loop_out++) {
                    bool is_link = false;
                    for (int loop_inner = 0; loop_inner < link_span_list_behind.size(); loop_inner++) {
                        if (target_span_list[loop_out].end_pos_ <= link_span_list_behind[loop_inner].start_pos_
                            && (link_span_list_behind[loop_inner].word_start_pos_ - target_span_list[loop_out].word_end_pos_>= 0)
                            && (link_span_list_behind[loop_inner].word_start_pos_ - target_span_list[loop_out].word_end_pos_<= 1)) {
                            is_link = true;

                            // link
                            //Span possible_span(target_span_list[loop_out].start_pos_, link_span_list_behind[loop_inner].end_pos_, string("new one"));
                            //target_span_list.push_back(possible_span);
                            target_span_list[loop_out].end_pos_ = link_span_list_behind[loop_inner].end_pos_;
                            target_span_list[loop_out].word_end_pos_ = link_span_list_behind[loop_inner].word_end_pos_;

                            // new or modify the corresponding mark
                            if (target_span_list[loop_out].pattern_col_marks.size() > j*2+1) {
                                target_span_list[loop_out].pattern_col_marks[j*2] = link_span_list_behind[loop_inner].start_pos_;
                                target_span_list[loop_out].pattern_col_marks[j*2+1] = link_span_list_behind[loop_inner].end_pos_;
                            } else {
                                target_span_list[loop_out].pattern_col_marks.push_back(link_span_list_behind[loop_inner].start_pos_);
                                target_span_list[loop_out].pattern_col_marks.push_back(link_span_list_behind[loop_inner].end_pos_);
                            }
                            if (DEBUG) cout << doc_str.substr(target_span_list[loop_out].start_pos_, target_span_list[loop_out].end_pos_ - target_span_list[loop_out].start_pos_) << endl;
                            break;
                        }
                    }
                    vector<Span>::iterator it = target_span_list.begin();
                    if (!is_link) {
                        //cout << target_span_list[loop_out].start_pos_ << "to" << target_span_list[loop_out].end_pos_ << endl;
                        target_span_list.erase(it+loop_out);
                        loop_out--;
                    }
                }
                if (DEBUG) cout << "------------------------------------------------\n\n\n\n\n";
            }

            //maximun limit
            // cout << all_atoms[j].repeat_max_-all_atoms[j].repeat_min_ << endl;
            for (int k = 0; k < all_atoms[j].repeat_max_-all_atoms[j].repeat_min_;k++) {
                if (DEBUG) cout << "-----------------repeat max------------------\n" << k << link_col_behind.get_name() << endl;
                unsigned int origin_size = target_span_list.size();
                for (int loop_out = 0; loop_out < origin_size; loop_out++) {
                    for (int loop_inner = 0; loop_inner < link_span_list_behind.size(); loop_inner++) {
                        if (target_span_list[loop_out].end_pos_ <= link_span_list_behind[loop_inner].start_pos_
                            && (link_span_list_behind[loop_inner].word_start_pos_ - target_span_list[loop_out].word_end_pos_> 0)
                            && (link_span_list_behind[loop_inner].word_start_pos_ - target_span_list[loop_out].word_end_pos_<= 1)) {

                            // link
                            // Span possible_span(target_span_list[loop_out].start_pos_, link_span_list_behind[loop_inner].end_pos_, string("new one"));
                            Span possible_span = target_span_list[loop_out];
                            possible_span.end_pos_ = link_span_list_behind[loop_inner].end_pos_;
                            possible_span.word_end_pos_ = link_span_list_behind[loop_inner].word_end_pos_;
                            if (target_span_list[loop_out].pattern_col_marks.size() > j*2+1) {
                                possible_span.pattern_col_marks[j*2] = link_span_list_behind[loop_inner].start_pos_;
                                possible_span.pattern_col_marks[j*2+1] = link_span_list_behind[loop_inner].end_pos_;
                            } else {
                                possible_span.pattern_col_marks.push_back(link_span_list_behind[loop_inner].start_pos_);
                                possible_span.pattern_col_marks.push_back(link_span_list_behind[loop_inner].end_pos_);
                            }

                            // to see if there exits!!!!!!!!!
                            bool repeat_flag = false;
                            for (unsigned int repeat_loop = 0; repeat_loop < target_span_list.size(); repeat_loop++) {
                                if (target_span_list[repeat_loop] == possible_span) {
                                    repeat_flag = true;
                                    break;
                                }
                            }
                            if (!repeat_flag) {
                                target_span_list.push_back(possible_span);
                                if (DEBUG) cout << doc_str.substr(possible_span.start_pos_, possible_span.end_pos_ - possible_span.start_pos_) << endl;
                                break;
                            }    
                        }
                    }
                }
                if (DEBUG) cout << "------------------------------------------------\n\n\n\n\n";
            }  
        }

        for (int seq = 0; seq < target_span_list.size(); seq++) {
            target_span_list[seq].value_ = doc_str.substr(target_span_list[seq].start_pos_, 
                target_span_list[seq].end_pos_-target_span_list[seq].start_pos_);
        }

        cmp_span_list = target_span_list;

        group_n.set_list(target_span_list);
        result_vector_of_column.push_back(group_n);
    }

    for (int i = 1; i < wanted_groups.size(); i++) {

        // target_span_list contains the result of the linking !!!
        Column group_n;
        group_n.set_name(wanted_groups[i].column_id_);
        vector<Span> target_span_list;

        int s_col = wanted_groups[i].start_col_seq_;
        int e_col = wanted_groups[i].end_col_seq_;

        // cout << i << " groups need col" << wanted_groups[i].start_col_seq_ << " to " << wanted_groups[i].end_col_seq_ << endl;

        for (int inner = 0 ; inner < cmp_span_list.size(); inner++) {
            int s_col_in_sub_span = cmp_span_list[inner].pattern_col_marks[s_col*2];
            int e_col_in_sub_span = cmp_span_list[inner].pattern_col_marks[e_col*2+1];
            Span sub_span(s_col_in_sub_span, e_col_in_sub_span, doc_str.substr(s_col_in_sub_span, e_col_in_sub_span - s_col_in_sub_span));
            target_span_list.push_back(sub_span);
        }

        group_n.set_list(target_span_list);
        result_vector_of_column.push_back(group_n);
    }

    //skip from_list stmt;
    analyse_from_list();

    //orgnise in dictionary order
    for (unsigned int i = 0; i < result_vector_of_column.size(); i++) {
        unsigned int choice = i;
        Column min = result_vector_of_column[i];
        for (unsigned int j = i+1; j < result_vector_of_column.size(); j++) {
            if (result_vector_of_column[j].get_name() < min.get_name()) {
                choice = j;
            }
        }
        Column tmp = result_vector_of_column[i];
        result_vector_of_column[i] = result_vector_of_column[choice];
        result_vector_of_column[choice] = tmp;
    }

    return result_vector_of_column;
}

/*
*pattern_expr →  pattern_pkg 
*             |  pattern_expr   pattern_pkg
*/
vector<PatternGroup> Parser::analyse_pattern_expr() {
    vector<PatternGroup> pattern_groups;

    //default group0 analyse, ignoring capture "()"
    PatternGroup group_0;
    int content_col_num = 0;
    while (scan() && !(peek_is_match("return") || peek_is_match("as")))  {
        Atom tmp_atom;
        if (peek_is_match("(") || peek_is_match(")")) {
            continue;
        } else if (peek_is_match("<") || peek_has_type_of(Tag::REGEX_EXPR)) {
            tmp_atom = analyse_atom();
            content_col_num++; 
            // see whether it needs repeat
            if (scan() && peek_is_match("{")) {
                assert_next_peek_has_type(Tag::NUM);
                tmp_atom.repeat_min_ = peek_.num_value_;
                assert_next_peek_is_match(",");
                assert_next_peek_has_type(Tag::NUM);
                tmp_atom.repeat_max_ = peek_.num_value_;
                assert_next_peek_is_match("}");
            } else {
                step_back();
            }
        }
        group_0.content_atoms_.push_back(tmp_atom);
    }
    group_0.group_num_ = 0;
    group_0.start_col_seq_ = 0;
    group_0.end_col_seq_ = content_col_num;
    pattern_groups.push_back(group_0);

    // return back to the beginning of the pattern_expr
    while (!peek_is_match("pattern")) {
        step_back();
    }
    scan();

    //get the sub group1 to group n if they exist

    int tmp_pos;
    int sub_group_num = 0;
    int col_seq = 0;

    PatternGroup group_tmp;
    while (scan() && !(peek_is_match("return") || peek_is_match("as"))) {
        if (peek_is_match("(")) {
            tmp_pos = peek_pos_;
            group_tmp = analyse_pattern_pkg();
            if (group_tmp.end_col_seq_ != -1) {
                // calculate begin
                group_tmp.start_col_seq_ = col_seq;
                group_tmp.end_col_seq_ = group_tmp.end_col_seq_+col_seq;
                group_tmp.group_num_ = ++sub_group_num;
                pattern_groups.push_back(group_tmp);
            }
            peek_pos_ = tmp_pos;
        } else if (peek_has_type_of(Tag::REGEX_EXPR)) {
            col_seq++;
        } else if (peek_is_match("<")) {
            col_seq++;
            while (!peek_is_match(">")) {
                scan();
            }
        }
    }

    step_back();
    return pattern_groups;
}

/*
* pattern_pkg → atom 
            |   atom { NUM , NUM } 
            |   pattern_group

  here gets group1 to group n if they exist
  insert group1 to group n if they exist 
*/
PatternGroup Parser::analyse_pattern_pkg() {

    PatternGroup no;
    int inner_capture = 0;
    int col_move = -1;
    no.end_col_seq_ = -1;
    while (scan() && !(peek_is_match("return") || peek_is_match("as"))) {
        if (peek_is_match(")") && inner_capture == 0) {
            no.end_col_seq_ = col_move;
            break;
        } else if (peek_is_match(")")) {
            inner_capture--;
        } else if (peek_is_match("(")) {
            inner_capture++;
        } else if (peek_has_type_of(Tag::REGEX_EXPR)) {
            col_move++;
        } else if (peek_is_match("<")) {
            col_move++;
            while (!peek_is_match(">")) {
                scan();
            }
        }
    }

    return no;
}

/*
* atom→ < column > | < Token > | REG
*/
Atom Parser::analyse_atom() {
    Atom result_atom;

    // reg_exp has no "<"  ">" surrounded
    if (!peek_has_type_of(Tag::REGEX_EXPR)) scan();
    //
    if (DEBUG) {
        cout << peek_.toString() << peek_.tag_ << endl;
    }
    if (peek_has_type_of(Tag::ID)) {
        step_back();
        result_atom.type_ = Tag::ID;
        vector<CodeToken> view_col_pair = analyse_colomn();
        result_atom.view_alias_ = view_col_pair[0].toString(); 
        result_atom.col_name_ = view_col_pair[1].toString();
        assert_next_peek_is_match(">");
    } else if (peek_has_type_of(Tag::TOKEN)) {
        result_atom.type_ = Tag::TOKEN;
        assert_next_peek_is_match(">");
    } else if (peek_has_type_of(Tag::REGEX_EXPR)) {
        result_atom.type_ = Tag::REGEX_EXPR;
        result_atom.regex_expr_ = peek_.toString();
    } else {
        error("unrecognised atom");
    }
    return result_atom;
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
    vector<CodeToken> column_ids = analyse_colomn(); 

    //analyse name_spec, and remember
    vector<GroupRecord> group_records = analyse_name_spec();

    //really extract here
    //get the exact col and view first
    View source_view = get_view_by_alias(column_ids[0].toString());
    Column source_column = source_view.get_column_by_name(column_ids[1].toString());

    int group_length = group_records.size();
    for (int group_seq = 0; group_seq < group_length; group_seq++) {
        Column single_column;
        single_column.set_name(group_records[group_seq].column_id_);
        //cout << group_records[group_seq].column_id_ << endl;

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
                for (j = result_from_engine[i][2*group_seq]; j < result_from_engine[i][1+2*group_seq]; j++) {
                    match_span += str[j];
                }
                //cout << match_span << result_from_engine[i][0] << j << endl;
                if (match_span.length() > 0) {
                    Span tmp_span(result_from_engine[i][2*group_seq], j, match_span);
                    single_column.add_span(tmp_span);
                } 
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
    //cout << record.group_num_ << endl;
    if (!(scan() && peek_is_match("as"))) {
        error("analyse_single_group()");
    }
    scan();
    record.column_id_ = peek_.toString();
    //cout << record.column_id_ << endl;
    return record;
}

/*
*column → ID . ID
*
*/
vector<CodeToken> Parser::analyse_colomn() {

    vector<CodeToken> column_ids;

    if (!(scan() && peek_has_type_of(Tag::ID))) {
        error("analyse_colomn()1");
    }

    column_ids.push_back(peek_);

    if (!(scan() && peek_is_match("."))) {
        error("analyse_colomn()2");
    }

    if (!(scan() && peek_has_type_of(Tag::ID))) {
        error("analyse_colomn()3");
    }

    column_ids.push_back(peek_);
    return column_ids;
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

void Parser::step_back() {
    if (!peek_pos_ <= 0) {
        peek_pos_--;
    }
    peek_ = lexer_list_[peek_pos_];
}

//announce error
void Parser::error(string func) {
    if (!is_end_) {
        cout << "Syntac Error in line: " << peek_.line_ << endl
            << " the Token is: " << peek_.toString() << endl
            << "call by " << func << endl;
        exit(0);
    }
}

// move peek_ forward and used as an assertion
// first used in pattern match 
bool Parser::assert_next_peek_is_match(const char*reserve) {
    string cmp(reserve);
    if (scan() && peek_.toString() == cmp) {
        return true;
    } else {
        error("assertion not match");
        return false;
    }
}

bool Parser::assert_next_peek_has_type(int tag) {
    if (scan() && peek_.tag_ == tag) {
        return true;
    } else {
        error("assertion not match");
        return false;
    }
}

Column Parser::get_column_from_tokenizer() {
    Column res_col;
    for (unsigned int i = 0; i < doc_token_list_.size();i++) {
        Span tmp(doc_token_list_[i].start_pos_, doc_token_list_[i].end_pos_, doc_token_list_[i].content_);
        res_col.add_span(tmp);
    }
    res_col.set_name("Token_Column");
    return res_col;
}

Column Parser::get_column_as_regex(string reg) {
    Column single_column;
    View source_view = get_view_by_name("Document");
    Column source_column = source_view.get_column_by_name("text");
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
            if (match_span.length() > 0) {
                Span tmp_span(result_from_engine[i][0], j, match_span);
                single_column.add_span(tmp_span);
            }
        }
    }
    single_column.set_name("REGEX_COLUMN");
    return single_column;
}

void Parser::calculate_span_word_seq(Column &content_col) {
    vector<Span> tmp_list = content_col.get_span_list();
    for (unsigned int i = 0; i < tmp_list.size(); i++) {
        for (unsigned j = 0; j < standard_word_span_list_.size(); j++) {
            if (tmp_list[i].start_pos_ >= standard_word_span_list_[j].start_pos_ &&
                tmp_list[i].start_pos_ < standard_word_span_list_[j].end_pos_) {
                tmp_list[i].word_start_pos_ = standard_word_span_list_[j].word_start_pos_;
            }

            if (tmp_list[i].end_pos_-1 >= standard_word_span_list_[j].start_pos_ &&
                tmp_list[i].end_pos_ <= standard_word_span_list_[j].end_pos_) {
                tmp_list[i].word_end_pos_ = standard_word_span_list_[j].word_start_pos_;
            }
        }
    }
    content_col.set_list(tmp_list);
}

vector<Span> Parser::get_standard_word_span_list_() {
    vector<Span> standard_list;
    for (unsigned int i = 0; i < doc_token_list_.size();i++) {
        Span tmp(doc_token_list_[i].start_pos_, doc_token_list_[i].end_pos_, doc_token_list_[i].content_);
        tmp.word_start_pos_ = doc_token_list_[i].word_num_;
        tmp.word_end_pos_ = doc_token_list_[i].word_num_;
        standard_list.push_back(tmp);
    }
    return standard_list;
}
