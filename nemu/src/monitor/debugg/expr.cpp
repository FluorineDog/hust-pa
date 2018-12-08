#include <regex>
#include "nemu.h"
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <sys/types.h>

typedef enum {
    TK_END = 0,
    TK_ = 1,
    TK_NOTYPE = 256,
    TK_DEC_NUM,
    TK_HEX_NUM,
    TK_REG,
    TK_MUL_STAR,
    TK_DIV,
    TK_PLUS,
    TK_MINUS,
    TK_CMP_OP,
    TK_LOGICAL_OP,
    TK_LEFT_PARAM,
    TK_RIGHT_PARAM,
    /* TODO: Add more token types */
} TOKEN_ID;

static struct rule {
    const char *regex;
    TOKEN_ID token_type;
} rules[] = {
        {R"($)", TK_END},
        {R"( +)", TK_NOTYPE},
        {R"(0x\d+)", TK_HEX_NUM},
        {R"(\d+)", TK_DEC_NUM},
        {R"(\$\w+)", TK_REG},
        {R"(\*)", TK_MUL_STAR},
        {R"(/)", TK_DIV},
        {R"(\+)", TK_PLUS},
        {R"(-)", TK_MINUS},
        {R"((<=|<|==|>|>=))", TK_CMP_OP},
        {R"((&&|\|\|))", TK_LOGICAL_OP},
        {R"(\()", TK_LEFT_PARAM},
        {R"(\))", TK_RIGHT_PARAM},
};
vector<pair<std::regex, TOKEN_ID>> engine_holder;



void test_regex();
void init_regex() {
    auto prefix = std::string("^");
    for (auto[rule_str, token] : rules) {
        engine_holder.emplace_back(std::regex(prefix + rule_str), token);
    }
    test_regex();
}

vector<std::pair<TOKEN_ID, string>> tokenize(const string &raw) {
    vector<std::pair<TOKEN_ID, string>> result;
    auto iter = raw.cbegin();
    while (iter < raw.cend()) {
        std::smatch m;
        TOKEN_ID tk = TK_END;
        for (auto&[eng, eng_tok] : engine_holder) {
            auto suc = std::regex_search(iter, raw.cend(), m, eng);
            if (suc) {
                tk = eng_tok;
                break;
            }
        }
        if (!m.ready()) {
            panic("invalid args");
            return {};
        }
        if (tk != TK_NOTYPE) {
            auto tk_str =  string(m[0].first, m[0].second);
            result.emplace_back(tk, tk_str)
        }
        iter = m[0].second;
    }
    return result;
}
void test_regex() {
    auto str = "1+2 * 3 / 4 - 5*-*( $abc * 4) == 8 && $rip <= 0x12ab";
    tokenize(str);
}

void expr(){
    
}

