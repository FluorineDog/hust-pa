#include <regex>
#include "nemu.h"
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <memory>
#include "monitor/expr.h"

using std::cout;
using std::unique_ptr;
using std::cerr;
using std::endl;
using std::pair;
using std::string;
using std::vector;
using std::make_unique;

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <sys/types.h>

#define FORWARD(t) do{ if(t == nullptr)return nullptr; }while(0)

typedef enum {
    TK_END = 0,
    TK_ = 1,
    TK_NOTYPE = 256,
    TK_DEC_NUM,
    TK_HEX_NUM,
    TK_REG,
    BARI_OP_BEG,
    TK_MUL_STAR,
    TK_DIV_MOD,
    TK_PLUS,
    TK_PLUS_MINUS,
    TK_CMP_OP1,
    TK_CMP_OP2,
    TK_AND,
    TK_OR,
    TK_BINARY1,
    TK_BINARY2,
    BARI_OP_END,
    TK_UNARY,
    TK_LEFT_PARAM,
    TK_RIGHT_PARAM,
    /* TODO: Add more token types */
} TOKEN_ID;

static struct rule {
    const char *regex;
    TOKEN_ID token_type;
} rules[] = {
        {R"($)",             TK_END},
        {R"( +)",            TK_NOTYPE},
        {R"(0x\d+)",         TK_HEX_NUM},
        {R"(\d+)",           TK_DEC_NUM},
        {R"(\$\w+)",         TK_REG},
        {R"(&&)",            TK_AND},
        {R"(\|\|)",          TK_OR},
        {R"((<<|>>))",       TK_BINARY2},
        {R"((<=|>=|==|!=))", TK_CMP_OP2},
        {R"(\*)",            TK_MUL_STAR},
        {R"((/|%))",         TK_DIV_MOD},
        {R"((\+|-))",        TK_PLUS_MINUS},
        {R"((\||\&))",       TK_BINARY1},
        {R"((<|>))",         TK_CMP_OP1},
        {R"((!|~))",         TK_UNARY},
        {R"(\()",            TK_LEFT_PARAM},
        {R"(\))",            TK_RIGHT_PARAM},
};
static vector<pair<std::regex, TOKEN_ID>> engine_holder;

static std::map<string, Handler> handler_holder;
static std::map<string, std::pair<int, int>> pred;

static void test_regex();


using Token = std::pair<TOKEN_ID, string>;
using VEC = vector<Token>;

static vector<Token> tokenize(const string &raw) {
    vector<std::pair<TOKEN_ID, string>> result;
    auto iter = raw.cbegin();
    while (true) {
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
            auto tk_str = string(m[0].first, m[0].second);
            result.emplace_back(tk, tk_str);
        }
        iter = m[0].second;
        if (tk == TK_END) {
            break;
        }
    }
    return result;
}


static bool is_operator(const Token &a) {
    return BARI_OP_BEG <= a.first && a.first < BARI_OP_END;
}

static bool pred_cmp_less(const Token &a, const Token &b) {
    assert(is_operator(a));
    assert(is_operator(b));
    return pred[a.second].first < pred[b.second].second;
}


Handler get_leaf_handler(const string &str) {
    return handler_holder[str];
}

Handler get_op_handler(const Token &tok, bool special = false) {
    if (special) {
        return handler_holder[tok.second + "s"];
    }
    return handler_holder[tok.second];
}

void init_handler() {
    handler_holder["reg"] = [](Tree *t) -> int { return 0; /* TODO*/ };
    handler_holder["num"] = [](Tree *t) -> int { return t->value; };

    handler_holder["*s"] = [](Tree *t) -> int { return 0; /* TODO*/ };
    handler_holder["-s"] = [](Tree *t) -> int { return -tree_eval(t->left); };
    handler_holder["+s"] = [](Tree *t) -> int { return +tree_eval(t->left); };
    handler_holder["~s"] = [](Tree *t) -> int { return ~tree_eval(t->left); };
    handler_holder["!s"] = [](Tree *t) -> int { return !tree_eval(t->left); };

    handler_holder["||"] = [](Tree *t) -> int { return tree_eval(t->left) || tree_eval(t->right); };

    handler_holder["&&"] = [](Tree *t) -> int { return tree_eval(t->left) && tree_eval(t->right); };

    handler_holder["|"] = [](Tree *t) -> int { return tree_eval(t->left) | tree_eval(t->right); };

    handler_holder["&"] = [](Tree *t) -> int { return tree_eval(t->left) & tree_eval(t->right); };

    handler_holder["=="] = [](Tree *t) -> int { return tree_eval(t->left) == tree_eval(t->right); };
    handler_holder["!="] = [](Tree *t) -> int { return tree_eval(t->left) != tree_eval(t->right); };

    handler_holder["<="] = [](Tree *t) -> int { return tree_eval(t->left) <= tree_eval(t->right); };
    handler_holder[">="] = [](Tree *t) -> int { return tree_eval(t->left) >= tree_eval(t->right); };
    handler_holder[">"] = [](Tree *t) -> int { return tree_eval(t->left) > tree_eval(t->right); };
    handler_holder["<"] = [](Tree *t) -> int { return tree_eval(t->left) < tree_eval(t->right); };

    handler_holder["<<"] = [](Tree *t) -> int { return tree_eval(t->left) << tree_eval(t->right); };
    handler_holder[">>"] = [](Tree *t) -> int { return tree_eval(t->left) >> tree_eval(t->right); };


    handler_holder["+"] = [](Tree *t) -> int { return tree_eval(t->left) + tree_eval(t->right); };
    handler_holder["-"] = [](Tree *t) -> int { return tree_eval(t->left) - tree_eval(t->right); };
    handler_holder["*"] = [](Tree *t) -> int { return tree_eval(t->left) * tree_eval(t->right); };
    handler_holder["/"] = [](Tree *t) -> int { return tree_eval(t->left) / tree_eval(t->right); };
    handler_holder["%"] = [](Tree *t) -> int { return tree_eval(t->left) % tree_eval(t->right); };

    pred["nope"] = {-1, -1,};
    pred["||"] = {2, 1,};

    pred["&&"] = {4, 3,};

    pred["|"] = {6, 5,};

    pred["&"] = {8, 7,};

    pred["=="] = {10, 9,};
    pred["!="] = {10, 9,};

    pred["<="] = {12, 11,};
    pred[">="] = {12, 11,};
    pred[">"] = {12, 11,};
    pred["<"] = {12, 11,};

    pred["<<"] = {14, 13,};
    pred[">>"] = {14, 13,};

    pred["+"] = {16, 15,};
    pred["-"] = {16, 15,};

    pred["*"] = {18, 17,};
    pred["/"] = {18, 17,};
    pred["%"] = {18, 17,};
}


class TreeGen {
public:
    unique_ptr<Tree> operator()(const string &expr) {
        auto vec = tokenize(expr);
        this->iter = vec.begin();
        Token init{BARI_OP_BEG, "nope"};
        for (auto &x: vec) {
            cout << x.first << " " << x.second << std::endl;
        }
        auto tree = parse_expr(init);
        assert(iter->first == TK_END);
        return tree;
    }

private:
    unique_ptr<Tree> parse_expr(const Token &last) {
        auto t1 = parse_elem();
        FORWARD(t1);
        while (true) {
            auto new_token = *iter;
            if (!is_operator(new_token)) {
                return t1;
            }
            if (!pred_cmp_less(last, new_token)) {
                return t1;
            }
            iter++;
            auto fn = get_op_handler(new_token);
            auto t2 = parse_expr(new_token);
            FORWARD(t2);
            auto new_node = make_unique<Tree>(fn, std::move(t1), std::move(t2));
            t1 = std::move(new_node);
            FORWARD(t1);
        }
    }

    unique_ptr<Tree> parse_elem() {
        auto token = *iter;
        switch (token.first) {
            case TK_MUL_STAR:
            case TK_UNARY:
            case TK_PLUS_MINUS: {
                ++iter;
                auto fn = get_op_handler(token, true);
                auto t = parse_elem();
                FORWARD(t);
                return make_unique<Tree>(fn, std::move(t));
            }
            case TK_HEX_NUM: {
                ++iter;
                int value = (int) strtol(token.second.c_str(), nullptr, 16);
                auto fn = get_leaf_handler("num");
                return make_unique<Tree>(value, fn);
            }
            case TK_DEC_NUM: {
                ++iter;
                int value = (int) strtol(token.second.c_str(), nullptr, 10);
                auto fn = get_leaf_handler("num");
                return make_unique<Tree>(value, fn);
            }
            case TK_REG: {
                // TODO
                ++iter;
                int value = 0;
                auto fn = get_leaf_handler("reg");
                return make_unique<Tree>(value, fn);
            }
            case TK_LEFT_PARAM: {
                ++iter;
                auto t = parse_expr(Token{BARI_OP_BEG, "nope"});
                FORWARD(t);
                assert(iter->first == TK_RIGHT_PARAM);
                ++iter;
                return t;
            }
            default:
                return nullptr;
        }
    }

    VEC::const_iterator iter;
};

static void test_regex() {
    auto str = "1 + 8 * 3 / 2 - 5*-(*( $abc * 4)  - 1) == 8 && $rip <= 0x12ab";
    auto validate =
            "1"
            "&& ( 0x10101010 | 0x11001100 & 0x11110000) == 0x11101010"
            "&& 2 != 3 == 1"
            "&& 2 <= 3 == 3 <= 4"
            "&& 2 < 3 != 3 > 4"
            "&& 1 << 3 <= 18 >> 1"
            "&& 2 + 3 << 2 == 20"
            "&& 3 + -1 == 2 && 6 / 2 * 3 == 9"
            "&& *0 == 0 && !0 == 1 && ~2 == -3"
            "&& -123 == 0 - 123 && ++44 == 44"
            "&& 100 % 7 == 2"
            "&& (65 | 33) == 97"
            "&& (65 & 33) == 1"
            "&& (1 << 31) == 0x80000000"
            "&& (-1 >> 28) == -1"
            "&& (1 || 0 && 0)";
    auto t = compile_expr(str);
    auto t2 = compile_expr(validate);
    cout << tree_eval(t) << endl;
    cout << tree_eval(t2) << endl;
    t = nullptr;
}

unique_ptr<Tree> compile_expr(const string &str) {
    return TreeGen()(str);
}

void init_regex() {
    auto prefix = std::string("^");
    for (auto[rule_str, token] : rules) {
        engine_holder.emplace_back(std::regex(prefix + rule_str), token);
    }
    init_handler();
    test_regex();
}
