#ifndef CODE_TOKEN_H
#define CODE_TOKEN_H

#include <string>
#include <cstdlib>
#include <sstream>

// transfer int to string 
#ifndef SSTR
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif

using namespace std;

//Tag 用于区别不同的保留字，标识符，数字
// SINGLE_TOKEN : "," ";"
class Tag {
public:
    const static int
    PATTERN = 275, TOKEN = 274, AND = 273, GROUP = 272,
    RETURN = 273, ON = 272, REGEX = 271, EXTRACT = 270,
    FROM = 269, SELECT = 268, OUTPUT = 267, AS = 266,
    VIEW = 265, CREATE = 264, NUM = 263, ID = 262,
    SINGLE_TOKEN = 261, REGEX_EXPR = 260;
};

//默认lexme是空串,tag=-1,异常

class CodeToken {
public:
    int tag_;
    int line_;
    int num_value_;
    string lexme_;

    CodeToken();

    // for number
    CodeToken(int t);

    // for word
    CodeToken(string s, const int tag);

    // for single special token like ';', '('
    CodeToken(char c);

    //重载＝符号
    void set(CodeToken* p1, CodeToken* p2);
    CodeToken& operator=(const CodeToken &tok);

    //复制构造函数, for STL Map
    CodeToken(const CodeToken &tok);

    string toString();
};


#endif