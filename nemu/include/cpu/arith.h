#define make_rtl_arith_logic(name) \
    void name_concat3(rtl_, name, i)(rtlreg_t * dest, const rtlreg_t *src1, int imm);
void name_concat3(RTL_PREFIX, _rtl_, name)(rtlreg_t *dest, const rtlreg_t *src1,
                                           const rtlreg_t *src2);

#define rtl_accept(XX) \
    XX(add)            \
    XX(sub)            \
    XX(and)            \
    XX(or)             \
    XX(xor)            \
    XX(shl)            \
    XX(shr)            \
    XX(sar)            \
    XX(mul_lo)         \
    XX(mul_hi)         \
    XX(imul_lo)        \
    XX(imul_hi)        \
    XX(div_q)          \
    XX(div_r)          \
    XX(idiv_q)         \
    XX(idiv_r)

#define XX_(name)                                                                     \
    void name_concat3(RTL_PREFIX, _rtl_, name)(rtlreg_t * dest, const rtlreg_t *src1, \
                                               const rtlreg_t *src2);

#define XX(name) XX_(name)
rtl_accept(XX)
#undef XX_
#undef XX

#define XX_(name)                                                                       \
    inline void name_concat(rtl_, name_concat(name, i))(rtlreg_t * dest, const rtlreg_t *src1, \
                                                 int imm) {                             \
        rtlreg_t imm_at;                                                                \
        rtl_li(&imm_at, imm);                                                           \
        name_concat3(RTL_PREFIX, _rtl_, name)(dest, src1, &imm_at);                     \
    }

#define XX(name) XX_(name)
    rtl_accept(XX)
#undef XX_
#undef XX
