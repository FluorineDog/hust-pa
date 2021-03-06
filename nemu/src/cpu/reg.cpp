#include "nemu.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <optional>
#include <string_view>
#include <algorithm>
#include <tuple>

using std::string_view;

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};



void reg_test() {
    srand(time(0));
    uint32_t sample[8];
    uint32_t eip_sample = rand();
    cpu.eip = eip_sample;

    int i;
    for (i = R_EAX; i <= R_EDI; i++) {
        sample[i] = rand();
        reg_l(i) = sample[i];
        assert(reg_w(i) == (sample[i] & 0xffff));
    }

    assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
    assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
    assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
    assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
    assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
    assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
    assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
    assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

    assert(sample[R_EAX] == cpu.eax);
    assert(sample[R_ECX] == cpu.ecx);
    assert(sample[R_EDX] == cpu.edx);
    assert(sample[R_EBX] == cpu.ebx);
    assert(sample[R_ESP] == cpu.esp);
    assert(sample[R_EBP] == cpu.ebp);
    assert(sample[R_ESI] == cpu.esi);
    assert(sample[R_EDI] == cpu.edi);

    assert(eip_sample == cpu.eip);
}
std::optional<std::tuple<const char*, size_t>> parse_cpuname(std::string_view str_) {
    assert(str_[0] == '$');
    string_view str = str_.substr(1, 10);
    auto checker = [](std::string_view str, const char *regname[]) {
        auto iter = std::find(regname, regname + 8, str);
        auto loc = iter - regname;
        return loc;
    };
    size_t ret;
    if(str == "eip") {
        return  std::forward_as_tuple("eip", ret);
    }
    ret = checker(str, regsl);
    if (ret != 8) return std::forward_as_tuple("regl", ret);
    ret = checker(str, regsw);
    if (ret != 8) return std::forward_as_tuple("regw", ret);
    ret = checker(str, regsb);
    if (ret != 8) return std::forward_as_tuple("regb", ret);
    return std::nullopt;
}