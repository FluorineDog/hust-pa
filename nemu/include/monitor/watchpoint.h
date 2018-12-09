#pragma once

#include "common.h"
#include <map>
#include "expr.h"

struct WatchPoint {
private:
    int value;
    std::string expr_str;
    std::unique_ptr<Tree> tree;
public:
    WatchPoint(std::string_view expr_str, std::unique_ptr<Tree>&& tree)
            : expr_str(expr_str), tree(std::move(tree)) {
        value = tree->eval();
    }
    std::string get_expr_str() {
        return expr_str;
    }

    int get_value() {
        return value;
    }

    bool update() {
        int new_value = tree->eval();
        if (new_value != this->value) {
            this->value = new_value;
            return true;
        } else {
            return false;
        }
    }
};

extern std::map<int, WatchPoint> g_watch_point_pool;
extern int g_watch_count;
