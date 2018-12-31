#include <string>
#include <vector>
using std::string;
using std::vector;
inline vector<string> split_str(const string& str, char delim) {
    int hhd = 0;
    vector<string> result;
    while(true) {
        size_t beg = str.find_first_not_of(delim, hhd);
        if(beg == string::npos) {
            break;
        }
        size_t end = str.find_first_of(delim, beg);
        if(end == string::npos) {
            end = str.size();
        }
        result.emplace_back(str.substr(beg, end - beg));
        if(end == string::npos) {
            break;
        }
    }
    return result;
}
