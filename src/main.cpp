#include <iostream>
#include "tokenizer.h"
#include "code_token.h"
#include "lexer.h"
#include "parser.h"
#include <cstdio>

using namespace std;

int main(int argc, char**argv) {

    Tokenizer test(argv[2]);

    Lexer lexer(argv[1]);
    vector<CodeToken> v = lexer.get_list();

    Parser parser(v, test);
    return 0;
}
