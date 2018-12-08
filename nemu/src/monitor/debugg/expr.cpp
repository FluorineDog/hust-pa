#include <regex>
#include "nemu.h"
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <memory>

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
    TK_DIV,
    TK_PLUS,
    TK_MINUS,
    TK_CMP_OP,
    TK_AND,
    TK_OR,
    BARI_OP_END,
    TK_LEFT_PARAM,
    TK_RIGHT_PARAM,
    /* TODO: Add more token types */
} TOKEN_ID;

static struct rule {
    const char *regex;
    TOKEN_ID token_type;
} rules[] = {
        {R"($)",                 TK_END},
        {R"( +)",                TK_NOTYPE},
        {R"(0x\d+)",             TK_HEX_NUM},
        {R"(\d+)",               TK_DEC_NUM},
        {R"(\$\w+)",             TK_REG},
        {R"(\*)",                TK_MUL_STAR}, // 0 1
        {R"(/)",                 TK_DIV}, // 0 1
        {R"(\+)",                TK_PLUS}, // 2 3
        {R"(-)",                 TK_MINUS}, // 2 3
        {R"((<=|<|==|>|>=|!=))", TK_CMP_OP}, // 4 5
        {R"(&&)",                TK_AND}, // 6 7
        {R"(\|\|)",              TK_OR},  // 8 9
        {R"(\()",                TK_LEFT_PARAM},
        {R"(\))",                TK_RIGHT_PARAM},
};
static vector<pair<std::regex, TOKEN_ID>> engine_holder;

using Handler = int (*)(struct Tree *);
static std::map<string, Handler> handler_holder;
static std::map<string, std::pair<int, int>> pred;

void test_regex();


using Token = std::pair<TOKEN_ID, string>;
using VEC = vector<Token>;

vector<Token> tokenize(const string &raw) {
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

struct Tree {
    Handler fn;
    unique_ptr<Tree> left;
    unique_ptr<Tree> right;
    int value;

    Tree(Handler fn, unique_ptr<Tree> l, unique_ptr<Tree> r = nullptr) : fn(std::move(fn)), left(std::move(l)),
                                                                         right(std::move(r)) {}

    Tree(int v, Handler fn) : fn(fn), value(v) {}

};

int get_value(unique_ptr<Tree> &t) {
    return t->fn(t.get());
}

bool is_operator(const Token &a) {
    return BARI_OP_BEG <= a.first && a.first < BARI_OP_END;
}

bool pred_cmp_less(const Token &a, const Token &b) {
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
    handler_holder["-s"] = [](Tree *t) -> int { return -get_value(t->left); };

    handler_holder["||"] = [](Tree *t) -> int { return get_value(t->left) || get_value(t->right); };
    handler_holder["&&"] = [](Tree *t) -> int {return get_value(t->left) && get_value(t->right);};
    handler_holder["+"] = [](Tree *t) -> int { return get_value(t->left) + get_value(t->right); };
    handler_holder["-"] = [](Tree *t) -> int { return get_value(t->left) - get_value(t->right); };
    handler_holder["*"] = [](Tree *t) -> int { return get_value(t->left) * get_value(t->right); };
    handler_holder["/"] = [](Tree *t) -> int { return get_value(t->left) / get_value(t->right); };
    handler_holder["=="] = [](Tree *t) -> int { return get_value(t->left) == get_value(t->right); };
    handler_holder["<="] = [](Tree *t) -> int { return get_value(t->left) <= get_value(t->right); };
    handler_holder[">="] = [](Tree *t) -> int { return get_value(t->left) >= get_value(t->right); };
    handler_holder["!="] = [](Tree *t) -> int { return get_value(t->left) != get_value(t->right); };
    handler_holder[">"] = [](Tree *t) -> int { return get_value(t->left) > get_value(t->right); };
    handler_holder["<"] = [](Tree *t) -> int { return get_value(t->left) < get_value(t->right); };
    pred["nope"] = {-1, -1,};
    pred["||"] = {1, 0,};
    pred["&&"] = {3, 2,};
    pred["=="] = {5, 4,};
    pred["<="] = {5, 4,};
    pred[">="] = {5, 4,};
    pred["!="] = {5, 4,};
    pred[">"] = {5, 4,};
    pred["<"] = {5, 4,};
    pred["+"] = {7, 6,};
    pred["-"] = {7, 6,};
    pred["*"] = {9, 8,};
    pred["/"] = {9, 8,};
}

void init_regex() {
    auto prefix = std::string("^");
    for (auto[rule_str, token] : rules) {
        engine_holder.emplace_back(std::regex(prefix + rule_str), token);
    }
    init_handler();
    test_regex();
}

class TreeGen {
public:
    unique_ptr<Tree> operator()(const string &expr) {
        auto vec = tokenize(expr);
        this->iter = vec.begin();
        Token init{BARI_OP_BEG, "nope"};
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
            case TK_MINUS: {
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

void test_regex() {
    auto str = "1 + 8 * 3 / 2 - 5*-(*( $abc * 4)  - 1) == 8 && $rip <= 0x12ab";
//    auto str = "1+2";
    auto t = TreeGen()(str);
    cout << get_value(t) << endl;
    t = nullptr;
}
