#include "monitor/watchpoint.h"
#include "monitor/expr.h"

std::map<int, WatchPoint> g_watch_point_pool;

int g_watch_count = 0;