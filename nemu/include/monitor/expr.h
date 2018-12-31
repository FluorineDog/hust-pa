#pragma once
#include "common.h"
#include <memory>
#include <string>

using Handler = int (*)(struct Tree *);
struct Tree {
  private:
    Handler fn;

  public:
    int value;
    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;
    Tree(Handler fn, std::unique_ptr<Tree> l, std::unique_ptr<Tree> r = nullptr)
        : fn(std::move(fn)), left(std::move(l)), right(std::move(r)) {}
    Tree(int v, Handler fn) : fn(fn), value(v) {}
    int eval() {
        return fn(this);
    }
};

//inline int tree_eval(std::unique_ptr<Tree> &t) {
//    return t->fn(t.get());
//}

std::unique_ptr<Tree> compile_expr(std::string_view str);
