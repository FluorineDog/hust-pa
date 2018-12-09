#pragma once

#include "common.h"
#include <map>
#include "expr.h"

struct WatchPoint {
private:
    int value_;
    std::string expr_str_;
    std::unique_ptr<Tree> tree_;
public:
    WatchPoint(std::string_view expr_str, std::unique_ptr<Tree>&& tree)
            : expr_str_(expr_str), tree_(std::move(tree)) {
        value_ = this->tree_->eval();
    }
    std::string get_expr_str() {
        return expr_str_;
    }

    int get_value() {
        return value_;
    }

    bool update() {
        int new_value = tree_->eval();
        if (new_value != this->value_) {
            this->value_ = new_value;
            return true;
        } else {
            return false;
        }
    }
};

extern std::map<int, WatchPoint> g_watch_point_pool;
extern int g_watch_count;
