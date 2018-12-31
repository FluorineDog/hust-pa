/* Shared data between exp, exp2 and pow.
   Copyright (c) 2018 Arm Ltd.  All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the company may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

   THIS SOFTWARE IS PROVIDED BY Arm LTD ``AS IS AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL Arm LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "fdlibm.h"
#if !__OBSOLETE_MATH

#include "math_config.h"

#define N (1 << EXP_TABLE_BITS)

const struct exp_data __exp_data = {
    // N/ln2
    .invln2N = 0x1.71547652b82fep0 * N,
// -ln2/N
#if N == 64
    .negln2hiN = -0x1.62e42fefa0000p-7,
    .negln2loN = -0x1.cf79abc9e3b3ap-46,
#elif N == 128
    .negln2hiN = -0x1.62e42fefa0000p-8,
    .negln2loN = -0x1.cf79abc9e3b3ap-47,
#elif N == 256
    .negln2hiN = -0x1.62e42fefc0000p-9,
    .negln2loN = 0x1.c610ca86c3899p-45,
#endif
// Used for rounding when !TOINT_INTRINSICS
#if EXP_USE_TOINT_NARROW
    .shift = 0x1800000000.8p0,
#else
    .shift = 0x1.8p52,
#endif
    // exp polynomial coefficients.
    .poly =
        {
#if N == 64 && EXP_POLY_ORDER == 5 && !EXP_POLY_WIDE
            // abs error: 1.5543*2^-60
            // ulp error: 0.529 (0.533 without fma)
            // if |x| < ln2/128+eps
            // abs error if |x| < ln2/64: 1.7157*2^-50
            0x1.fffffffffdbcdp-2,
            0x1.555555555444cp-3,
            0x1.555573c6a9f7dp-5,
            0x1.1111266d28935p-7,
#elif N == 64 && EXP_POLY_ORDER == 6 && EXP_POLY_WIDE
            // abs error: 1.6735*2^-64
            // ulp error: 0.518 (0.522 without fma)
            // if |x| < ln2/64
            0x1.5555555548f9ap-3,
            0x1.555555554bf5dp-5,
            0x1.11115b75f0f4dp-7,
            0x1.6c171a6b6303ep-10,
#elif N == 128 && EXP_POLY_ORDER == 5 && !EXP_POLY_WIDE
            // abs error: 1.555*2^-66
            // ulp error: 0.509 (0.511 without fma)
            // if |x| < ln2/256+eps
            // abs error if |x| < ln2/256+0x1p-15: 1.09*2^-65
            // abs error if |x| < ln2/128: 1.7145*2^-56
            0x1.ffffffffffdbdp-2,
            0x1.555555555543cp-3,
            0x1.55555cf172b91p-5,
            0x1.1111167a4d017p-7,
#elif N == 128 && EXP_POLY_ORDER == 5 && EXP_POLY_WIDE
            // abs error: 1.5542*2^-60
            // ulp error: 0.521 (0.523 without fma)
            // if |x| < ln2/128
            0x1.fffffffffdbcep-2,
            0x1.55555555543c2p-3,
            0x1.555573c64f2e3p-5,
            0x1.111126b4eff73p-7,
#elif N == 128 && EXP_POLY_ORDER == 6 && EXP_POLY_WIDE
            // abs error: 1.6861*2^-71
            // ulp error: 0.509 (0.511 without fma)
            // if |x| < ln2/128
            0x1.55555555548fdp-3,
            0x1.555555555658fp-5,
            0x1.111123a859bb6p-7,
            0x1.6c16ba6920cabp-10,
#elif N == 256 && EXP_POLY_ORDER == 4 && !EXP_POLY_WIDE
            // abs error: 1.43*2^-58
            // ulp error: 0.549 (0.550 without fma)
            // if |x| < ln2/512
            0x1p0,    // unused
            0x1.fffffffffffd4p-2,
            0x1.5555571d6ef9p-3,
            0x1.5555576a5adcep-5,
#elif N == 256 && EXP_POLY_ORDER == 5 && EXP_POLY_WIDE
            // abs error: 1.5547*2^-66
            // ulp error: 0.505 (0.506 without fma)
            // if |x| < ln2/256
            0x1.ffffffffffdbdp-2,
            0x1.555555555543cp-3,
            0x1.55555cf16e1edp-5,
            0x1.1111167a4b553p-7,
#endif
        },
    .exp2_shift = 0x1.8p52 / N,
    // exp2 polynomial coefficients.
    .exp2_poly =
        {
#if N == 64 && EXP2_POLY_ORDER == 6 && EXP2_POLY_WIDE
            // abs error: 1.3054*2^-63
            // ulp error: 0.515
            // if |x| < 1/64
            0x1.62e42fefa39efp-1,
            0x1.ebfbdff82c58fp-3,
            0x1.c6b08d7045cf1p-5,
            0x1.3b2ab6fb8fd0ep-7,
            0x1.5d884afec48d7p-10,
            0x1.43097dc684ae1p-13,
#elif N == 128 && EXP2_POLY_ORDER == 5 && !EXP2_POLY_WIDE
            // abs error: 1.2195*2^-65
            // ulp error: 0.507 (0.511 without fma)
            // if |x| < 1/256
            // abs error if |x| < 1/128: 1.9941*2^-56
            0x1.62e42fefa39efp-1,
            0x1.ebfbdff82c424p-3,
            0x1.c6b08d70cf4b5p-5,
            0x1.3b2abd24650ccp-7,
            0x1.5d7e09b4e3a84p-10,
#elif N == 256 && EXP2_POLY_ORDER == 5 && EXP2_POLY_WIDE
            // abs error: 1.2195*2^-65
            // ulp error: 0.504 (0.508 without fma)
            // if |x| < 1/256
            0x1.62e42fefa39efp-1,
            0x1.ebfbdff82c424p-3,
            0x1.c6b08d70cf4b5p-5,
            0x1.3b2abd24650ccp-7,
            0x1.5d7e09b4e3a84p-10,
#endif
        },
    // 2^(k/N) ~= H[k]*(1 + T[k]) for int k in [0,N)
    // tab[2*k] = asuint64(T[k])
    // tab[2*k+1] = asuint64(H[k]) - (k << 52)/N
    .tab =
        {
#if N == 64
            0x0,
            0x3ff0000000000000,
            0xbc7160139cd8dc5d,
            0x3fefec9a3e778061,
            0x3c8cd2523567f613,
            0x3fefd9b0d3158574,
            0x3c60f74e61e6c861,
            0x3fefc74518759bc8,
            0x3c979aa65d837b6d,
            0x3fefb5586cf9890f,
            0x3c3ebe3d702f9cd1,
            0x3fefa3ec32d3d1a2,
            0xbc9556522a2fbd0e,
            0x3fef9301d0125b51,
            0xbc91c923b9d5f416,
            0x3fef829aaea92de0,
            0xbc801b15eaa59348,
            0x3fef72b83c7d517b,
            0x3c8b898c3f1353bf,
            0x3fef635beb6fcb75,
            0x3c9aecf73e3a2f60,
            0x3fef54873168b9aa,
            0x3c8a6f4144a6c38d,
            0x3fef463b88628cd6,
            0x3c968efde3a8a894,
            0x3fef387a6e756238,
            0x3c80472b981fe7f2,
            0x3fef2b4565e27cdd,
            0x3c82f7e16d09ab31,
            0x3fef1e9df51fdee1,
            0x3c8b3782720c0ab4,
            0x3fef1285a6e4030b,
            0x3c834d754db0abb6,
            0x3fef06fe0a31b715,
            0x3c8fdd395dd3f84a,
            0x3feefc08b26416ff,
            0xbc924aedcc4b5068,
            0x3feef1a7373aa9cb,
            0xbc71d1e83e9436d2,
            0x3feee7db34e59ff7,
            0x3c859f48a72a4c6d,
            0x3feedea64c123422,
            0xbc58a78f4817895b,
            0x3feed60a21f72e2a,
            0x3c4363ed60c2ac11,
            0x3feece086061892d,
            0x3c6ecce1daa10379,
            0x3feec6a2b5c13cd0,
            0x3c7690cebb7aafb0,
            0x3feebfdad5362a27,
            0xbc8f94340071a38e,
            0x3feeb9b2769d2ca7,
            0xbc78dec6bd0f385f,
            0x3feeb42b569d4f82,
            0x3c93350518fdd78e,
            0x3feeaf4736b527da,
            0x3c9063e1e21c5409,
            0x3feeab07dd485429,
            0x3c9432e62b64c035,
            0x3feea76f15ad2148,
            0xbc8c33c53bef4da8,
            0x3feea47eb03a5585,
            0xbc93cedd78565858,
            0x3feea23882552225,
            0xbc93b3efbf5e2228,
            0x3feea09e667f3bcd,
            0xbc6367efb86da9ee,
            0x3fee9fb23c651a2f,
            0xbc781f647e5a3ecf,
            0x3fee9f75e8ec5f74,
            0xbc8619321e55e68a,
            0x3fee9feb564267c9,
            0xbc7b32dcb94da51d,
            0x3feea11473eb0187,
            0x3c65ebe1abd66c55,
            0x3feea2f336cf4e62,
            0xbc9369b6f13b3734,
            0x3feea589994cce13,
            0xbc94d450d872576e,
            0x3feea8d99b4492ed,
            0x3c8db72fc1f0eab4,
            0x3feeace5422aa0db,
            0x3c7bf68359f35f44,
            0x3feeb1ae99157736,
            0xbc5da9b88b6c1e29,
            0x3feeb737b0cdc5e5,
            0xbc92434322f4f9aa,
            0x3feebd829fde4e50,
            0x3c71affc2b91ce27,
            0x3feec49182a3f090,
            0xbc87c50422622263,
            0x3feecc667b5de565,
            0xbc91bbd1d3bcbb15,
            0x3feed503b23e255d,
            0x3c8469846e735ab3,
            0x3feede6b5579fdbf,
            0x3c8c1a7792cb3387,
            0x3feee89f995ad3ad,
            0xbc55c3d956dcaeba,
            0x3feef3a2b84f15fb,
            0xbc68d6f438ad9334,
            0x3feeff76f2fb5e47,
            0x3c74ffd70a5fddcd,
            0x3fef0c1e904bc1d2,
            0x3c736eae30af0cb3,
            0x3fef199bdd85529c,
            0x3c84e08fd10959ac,
            0x3fef27f12e57d14b,
            0x3c676b2c6c921968,
            0x3fef3720dcef9069,
            0xbc8fad5d3ffffa6f,
            0x3fef472d4a07897c,
            0x3c74a385a63d07a7,
            0x3fef5818dcfba487,
            0x3c8e5a50d5c192ac,
            0x3fef69e603db3285,
            0xbc82d52107b43e1f,
            0x3fef7c97337b9b5f,
            0x3c74b604603a88d3,
            0x3fef902ee78b3ff6,
            0xbc8ff7128fd391f0,
            0x3fefa4afa2a490da,
            0x3c8ec3bc41aa2008,
            0x3fefba1bee615a27,
            0x3c8a64a931d185ee,
            0x3fefd0765b6e4540,
            0x3c77893b4d91cd9d,
            0x3fefe7c1819e90d8,
#elif N == 128
            0x0,
            0x3ff0000000000000,
            0x3c9b3b4f1a88bf6e,
            0x3feff63da9fb3335,
            0xbc7160139cd8dc5d,
            0x3fefec9a3e778061,
            0xbc905e7a108766d1,
            0x3fefe315e86e7f85,
            0x3c8cd2523567f613,
            0x3fefd9b0d3158574,
            0xbc8bce8023f98efa,
            0x3fefd06b29ddf6de,
            0x3c60f74e61e6c861,
            0x3fefc74518759bc8,
            0x3c90a3e45b33d399,
            0x3fefbe3ecac6f383,
            0x3c979aa65d837b6d,
            0x3fefb5586cf9890f,
            0x3c8eb51a92fdeffc,
            0x3fefac922b7247f7,
            0x3c3ebe3d702f9cd1,
            0x3fefa3ec32d3d1a2,
            0xbc6a033489906e0b,
            0x3fef9b66affed31b,
            0xbc9556522a2fbd0e,
            0x3fef9301d0125b51,
            0xbc5080ef8c4eea55,
            0x3fef8abdc06c31cc,
            0xbc91c923b9d5f416,
            0x3fef829aaea92de0,
            0x3c80d3e3e95c55af,
            0x3fef7a98c8a58e51,
            0xbc801b15eaa59348,
            0x3fef72b83c7d517b,
            0xbc8f1ff055de323d,
            0x3fef6af9388c8dea,
            0x3c8b898c3f1353bf,
            0x3fef635beb6fcb75,
            0xbc96d99c7611eb26,
            0x3fef5be084045cd4,
            0x3c9aecf73e3a2f60,
            0x3fef54873168b9aa,
            0xbc8fe782cb86389d,
            0x3fef4d5022fcd91d,
            0x3c8a6f4144a6c38d,
            0x3fef463b88628cd6,
            0x3c807a05b0e4047d,
            0x3fef3f49917ddc96,
            0x3c968efde3a8a894,
            0x3fef387a6e756238,
            0x3c875e18f274487d,
            0x3fef31ce4fb2a63f,
            0x3c80472b981fe7f2,
            0x3fef2b4565e27cdd,
            0xbc96b87b3f71085e,
            0x3fef24dfe1f56381,
            0x3c82f7e16d09ab31,
            0x3fef1e9df51fdee1,
            0xbc3d219b1a6fbffa,
            0x3fef187fd0dad990,
            0x3c8b3782720c0ab4,
            0x3fef1285a6e4030b,
            0x3c6e149289cecb8f,
            0x3fef0cafa93e2f56,
            0x3c834d754db0abb6,
            0x3fef06fe0a31b715,
            0x3c864201e2ac744c,
            0x3fef0170fc4cd831,
            0x3c8fdd395dd3f84a,
            0x3feefc08b26416ff,
            0xbc86a3803b8e5b04,
            0x3feef6c55f929ff1,
            0xbc924aedcc4b5068,
            0x3feef1a7373aa9cb,
            0xbc9907f81b512d8e,
            0x3feeecae6d05d866,
            0xbc71d1e83e9436d2,
            0x3feee7db34e59ff7,
            0xbc991919b3ce1b15,
            0x3feee32dc313a8e5,
            0x3c859f48a72a4c6d,
            0x3feedea64c123422,
            0xbc9312607a28698a,
            0x3feeda4504ac801c,
            0xbc58a78f4817895b,
            0x3feed60a21f72e2a,
            0xbc7c2c9b67499a1b,
            0x3feed1f5d950a897,
            0x3c4363ed60c2ac11,
            0x3feece086061892d,
            0x3c9666093b0664ef,
            0x3feeca41ed1d0057,
            0x3c6ecce1daa10379,
            0x3feec6a2b5c13cd0,
            0x3c93ff8e3f0f1230,
            0x3feec32af0d7d3de,
            0x3c7690cebb7aafb0,
            0x3feebfdad5362a27,
            0x3c931dbdeb54e077,
            0x3feebcb299fddd0d,
            0xbc8f94340071a38e,
            0x3feeb9b2769d2ca7,
            0xbc87deccdc93a349,
            0x3feeb6daa2cf6642,
            0xbc78dec6bd0f385f,
            0x3feeb42b569d4f82,
            0xbc861246ec7b5cf6,
            0x3feeb1a4ca5d920f,
            0x3c93350518fdd78e,
            0x3feeaf4736b527da,
            0x3c7b98b72f8a9b05,
            0x3feead12d497c7fd,
            0x3c9063e1e21c5409,
            0x3feeab07dd485429,
            0x3c34c7855019c6ea,
            0x3feea9268a5946b7,
            0x3c9432e62b64c035,
            0x3feea76f15ad2148,
            0xbc8ce44a6199769f,
            0x3feea5e1b976dc09,
            0xbc8c33c53bef4da8,
            0x3feea47eb03a5585,
            0xbc845378892be9ae,
            0x3feea34634ccc320,
            0xbc93cedd78565858,
            0x3feea23882552225,
            0x3c5710aa807e1964,
            0x3feea155d44ca973,
            0xbc93b3efbf5e2228,
            0x3feea09e667f3bcd,
            0xbc6a12ad8734b982,
            0x3feea012750bdabf,
            0xbc6367efb86da9ee,
            0x3fee9fb23c651a2f,
            0xbc80dc3d54e08851,
            0x3fee9f7df9519484,
            0xbc781f647e5a3ecf,
            0x3fee9f75e8ec5f74,
            0xbc86ee4ac08b7db0,
            0x3fee9f9a48a58174,
            0xbc8619321e55e68a,
            0x3fee9feb564267c9,
            0x3c909ccb5e09d4d3,
            0x3feea0694fde5d3f,
            0xbc7b32dcb94da51d,
            0x3feea11473eb0187,
            0x3c94ecfd5467c06b,
            0x3feea1ed0130c132,
            0x3c65ebe1abd66c55,
            0x3feea2f336cf4e62,
            0xbc88a1c52fb3cf42,
            0x3feea427543e1a12,
            0xbc9369b6f13b3734,
            0x3feea589994cce13,
            0xbc805e843a19ff1e,
            0x3feea71a4623c7ad,
            0xbc94d450d872576e,
            0x3feea8d99b4492ed,
            0x3c90ad675b0e8a00,
            0x3feeaac7d98a6699,
            0x3c8db72fc1f0eab4,
            0x3feeace5422aa0db,
            0xbc65b6609cc5e7ff,
            0x3feeaf3216b5448c,
            0x3c7bf68359f35f44,
            0x3feeb1ae99157736,
            0xbc93091fa71e3d83,
            0x3feeb45b0b91ffc6,
            0xbc5da9b88b6c1e29,
            0x3feeb737b0cdc5e5,
            0xbc6c23f97c90b959,
            0x3feeba44cbc8520f,
            0xbc92434322f4f9aa,
            0x3feebd829fde4e50,
            0xbc85ca6cd7668e4b,
            0x3feec0f170ca07ba,
            0x3c71affc2b91ce27,
            0x3feec49182a3f090,
            0x3c6dd235e10a73bb,
            0x3feec86319e32323,
            0xbc87c50422622263,
            0x3feecc667b5de565,
            0x3c8b1c86e3e231d5,
            0x3feed09bec4a2d33,
            0xbc91bbd1d3bcbb15,
            0x3feed503b23e255d,
            0x3c90cc319cee31d2,
            0x3feed99e1330b358,
            0x3c8469846e735ab3,
            0x3feede6b5579fdbf,
            0xbc82dfcd978e9db4,
            0x3feee36bbfd3f37a,
            0x3c8c1a7792cb3387,
            0x3feee89f995ad3ad,
            0xbc907b8f4ad1d9fa,
            0x3feeee07298db666,
            0xbc55c3d956dcaeba,
            0x3feef3a2b84f15fb,
            0xbc90a40e3da6f640,
            0x3feef9728de5593a,
            0xbc68d6f438ad9334,
            0x3feeff76f2fb5e47,
            0xbc91eee26b588a35,
            0x3fef05b030a1064a,
            0x3c74ffd70a5fddcd,
            0x3fef0c1e904bc1d2,
            0xbc91bdfbfa9298ac,
            0x3fef12c25bd71e09,
            0x3c736eae30af0cb3,
            0x3fef199bdd85529c,
            0x3c8ee3325c9ffd94,
            0x3fef20ab5fffd07a,
            0x3c84e08fd10959ac,
            0x3fef27f12e57d14b,
            0x3c63cdaf384e1a67,
            0x3fef2f6d9406e7b5,
            0x3c676b2c6c921968,
            0x3fef3720dcef9069,
            0xbc808a1883ccb5d2,
            0x3fef3f0b555dc3fa,
            0xbc8fad5d3ffffa6f,
            0x3fef472d4a07897c,
            0xbc900dae3875a949,
            0x3fef4f87080d89f2,
            0x3c74a385a63d07a7,
            0x3fef5818dcfba487,
            0xbc82919e2040220f,
            0x3fef60e316c98398,
            0x3c8e5a50d5c192ac,
            0x3fef69e603db3285,
            0x3c843a59ac016b4b,
            0x3fef7321f301b460,
            0xbc82d52107b43e1f,
            0x3fef7c97337b9b5f,
            0xbc892ab93b470dc9,
            0x3fef864614f5a129,
            0x3c74b604603a88d3,
            0x3fef902ee78b3ff6,
            0x3c83c5ec519d7271,
            0x3fef9a51fbc74c83,
            0xbc8ff7128fd391f0,
            0x3fefa4afa2a490da,
            0xbc8dae98e223747d,
            0x3fefaf482d8e67f1,
            0x3c8ec3bc41aa2008,
            0x3fefba1bee615a27,
            0x3c842b94c3a9eb32,
            0x3fefc52b376bba97,
            0x3c8a64a931d185ee,
            0x3fefd0765b6e4540,
            0xbc8e37bae43be3ed,
            0x3fefdbfdad9cbe14,
            0x3c77893b4d91cd9d,
            0x3fefe7c1819e90d8,
            0x3c5305c14160cc89,
            0x3feff3c22b8f71f1,
#elif N == 256
            0x0,
            0x3ff0000000000000,
            0xbc84e82fc61851ac,
            0x3feffb1afa5abcbf,
            0x3c9b3b4f1a88bf6e,
            0x3feff63da9fb3335,
            0xbc82985dd8521d32,
            0x3feff168143b0281,
            0xbc7160139cd8dc5d,
            0x3fefec9a3e778061,
            0x3c651e617061bfbd,
            0x3fefe7d42e11bbcc,
            0xbc905e7a108766d1,
            0x3fefe315e86e7f85,
            0x3c845fad437fa426,
            0x3fefde5f72f654b1,
            0x3c8cd2523567f613,
            0x3fefd9b0d3158574,
            0xbc954529642b232f,
            0x3fefd50a0e3c1f89,
            0xbc8bce8023f98efa,
            0x3fefd06b29ddf6de,
            0x3c8293708ef5c32e,
            0x3fefcbd42b72a836,
            0x3c60f74e61e6c861,
            0x3fefc74518759bc8,
            0xbc95b9280905b2a4,
            0x3fefc2bdf66607e0,
            0x3c90a3e45b33d399,
            0x3fefbe3ecac6f383,
            0x3c84f31f32c4b7e7,
            0x3fefb9c79b1f3919,
            0x3c979aa65d837b6d,
            0x3fefb5586cf9890f,
            0x3c9407fb30d06420,
            0x3fefb0f145e46c85,
            0x3c8eb51a92fdeffc,
            0x3fefac922b7247f7,
            0xbc9a5d04b3b9911b,
            0x3fefa83b23395dec,
            0x3c3ebe3d702f9cd1,
            0x3fefa3ec32d3d1a2,
            0xbc937a01f0739546,
            0x3fef9fa55fdfa9c5,
            0xbc6a033489906e0b,
            0x3fef9b66affed31b,
            0x3c8b8268b04ef0a5,
            0x3fef973028d7233e,
            0xbc9556522a2fbd0e,
            0x3fef9301d0125b51,
            0xbc9ac46e44a2ebcc,
            0x3fef8edbab5e2ab6,
            0xbc5080ef8c4eea55,
            0x3fef8abdc06c31cc,
            0xbc65704e90c9f860,
            0x3fef86a814f204ab,
            0xbc91c923b9d5f416,
            0x3fef829aaea92de0,
            0xbc897cea57e46280,
            0x3fef7e95934f312e,
            0x3c80d3e3e95c55af,
            0x3fef7a98c8a58e51,
            0x3c56f01429e2b9d2,
            0x3fef76a45471c3c2,
            0xbc801b15eaa59348,
            0x3fef72b83c7d517b,
            0x3c6e653b2459034b,
            0x3fef6ed48695bbc0,
            0xbc8f1ff055de323d,
            0x3fef6af9388c8dea,
            0x3c92cc7ea345b7dc,
            0x3fef672658375d2f,
            0x3c8b898c3f1353bf,
            0x3fef635beb6fcb75,
            0x3c957bfb2876ea9e,
            0x3fef5f99f8138a1c,
            0xbc96d99c7611eb26,
            0x3fef5be084045cd4,
            0x3c8cdc1873af2155,
            0x3fef582f95281c6b,
            0x3c9aecf73e3a2f60,
            0x3fef54873168b9aa,
            0xbc9493684653a131,
            0x3fef50e75eb44027,
            0xbc8fe782cb86389d,
            0x3fef4d5022fcd91d,
            0xbc98e2899077520a,
            0x3fef49c18438ce4d,
            0x3c8a6f4144a6c38d,
            0x3fef463b88628cd6,
            0x3c9120fcd4f59273,
            0x3fef42be3578a819,
            0x3c807a05b0e4047d,
            0x3fef3f49917ddc96,
            0x3c89b788c188c9b8,
            0x3fef3bdda27912d1,
            0x3c968efde3a8a894,
            0x3fef387a6e756238,
            0x3c877afbca90ef84,
            0x3fef351ffb82140a,
            0x3c875e18f274487d,
            0x3fef31ce4fb2a63f,
            0x3c91512f082876ee,
            0x3fef2e85711ece75,
            0x3c80472b981fe7f2,
            0x3fef2b4565e27cdd,
            0x3c9a02f0c7d75ec6,
            0x3fef280e341ddf29,
            0xbc96b87b3f71085e,
            0x3fef24dfe1f56381,
            0xbc803297e78260bf,
            0x3fef21ba7591bb70,
            0x3c82f7e16d09ab31,
            0x3fef1e9df51fdee1,
            0xbc95b77e5ccd9fbf,
            0x3fef1b8a66d10f13,
            0xbc3d219b1a6fbffa,
            0x3fef187fd0dad990,
            0xbc91e75c40b4251e,
            0x3fef157e39771b2f,
            0x3c8b3782720c0ab4,
            0x3fef1285a6e4030b,
            0x3c98a911f1f7785a,
            0x3fef0f961f641589,
            0x3c6e149289cecb8f,
            0x3fef0cafa93e2f56,
            0xbc61e7c998db7dbb,
            0x3fef09d24abd886b,
            0x3c834d754db0abb6,
            0x3fef06fe0a31b715,
            0x3c85425c11faadf4,
            0x3fef0432edeeb2fd,
            0x3c864201e2ac744c,
            0x3fef0170fc4cd831,
            0xbc979517a03e2847,
            0x3feefeb83ba8ea32,
            0x3c8fdd395dd3f84a,
            0x3feefc08b26416ff,
            0xbc800e2a46da4bee,
            0x3feef96266e3fa2d,
            0xbc86a3803b8e5b04,
            0x3feef6c55f929ff1,
            0xbc87430803972b34,
            0x3feef431a2de883b,
            0xbc924aedcc4b5068,
            0x3feef1a7373aa9cb,
            0xbc954de30ae02d94,
            0x3feeef26231e754a,
            0xbc9907f81b512d8e,
            0x3feeecae6d05d866,
            0xbc94f2487e1c03ec,
            0x3feeea401b7140ef,
            0xbc71d1e83e9436d2,
            0x3feee7db34e59ff7,
            0x3c914a5432fcb2f4,
            0x3feee57fbfec6cf4,
            0xbc991919b3ce1b15,
            0x3feee32dc313a8e5,
            0x3c79c3bba5562a2f,
            0x3feee0e544ede173,
            0x3c859f48a72a4c6d,
            0x3feedea64c123422,
            0xbc85a71612e21658,
            0x3feedc70df1c5175,
            0xbc9312607a28698a,
            0x3feeda4504ac801c,
            0x3c86421f6f1d24d6,
            0x3feed822c367a024,
            0xbc58a78f4817895b,
            0x3feed60a21f72e2a,
            0xbc9348a6815fce65,
            0x3feed3fb2709468a,
            0xbc7c2c9b67499a1b,
            0x3feed1f5d950a897,
            0x3c835c43984d9871,
            0x3feecffa3f84b9d4,
            0x3c4363ed60c2ac11,
            0x3feece086061892d,
            0xbc632afc8d9473a0,
            0x3feecc2042a7d232,
            0x3c9666093b0664ef,
            0x3feeca41ed1d0057,
            0xbc95fc5e44de020e,
            0x3feec86d668b3237,
            0x3c6ecce1daa10379,
            0x3feec6a2b5c13cd0,
            0xbc7ea0148327c42f,
            0x3feec4e1e192aed2,
            0x3c93ff8e3f0f1230,
            0x3feec32af0d7d3de,
            0xbc7a843ad1a88022,
            0x3feec17dea6db7d7,
            0x3c7690cebb7aafb0,
            0x3feebfdad5362a27,
            0x3c892ca3bf144e63,
            0x3feebe41b817c114,
            0x3c931dbdeb54e077,
            0x3feebcb299fddd0d,
            0xbc902c99b04aa8b0,
            0x3feebb2d81d8abff,
            0xbc8f94340071a38e,
            0x3feeb9b2769d2ca7,
            0x3c73e34f67e67118,
            0x3feeb8417f4531ee,
            0xbc87deccdc93a349,
            0x3feeb6daa2cf6642,
            0xbc75a3b1197ba0f0,
            0x3feeb57de83f4eef,
            0xbc78dec6bd0f385f,
            0x3feeb42b569d4f82,
            0x3c81bd2888075068,
            0x3feeb2e2f4f6ad27,
            0xbc861246ec7b5cf6,
            0x3feeb1a4ca5d920f,
            0xbc896be8ae89ef8f,
            0x3feeb070dde910d2,
            0x3c93350518fdd78e,
            0x3feeaf4736b527da,
            0xbc88e6ac90348602,
            0x3feeae27dbe2c4cf,
            0x3c7b98b72f8a9b05,
            0x3feead12d497c7fd,
            0xbc91af7f1365c3ac,
            0x3feeac0827ff07cc,
            0x3c9063e1e21c5409,
            0x3feeab07dd485429,
            0xbc943a3540d1898a,
            0x3feeaa11fba87a03,
            0x3c34c7855019c6ea,
            0x3feea9268a5946b7,
            0xbc951f58ddaa8090,
            0x3feea84590998b93,
            0x3c9432e62b64c035,
            0x3feea76f15ad2148,
            0xbc82e1648e50a17c,
            0x3feea6a320dceb71,
            0xbc8ce44a6199769f,
            0x3feea5e1b976dc09,
            0x3c95f30eda98a575,
            0x3feea52ae6cdf6f4,
            0xbc8c33c53bef4da8,
            0x3feea47eb03a5585,
            0x3c917ecda8a72159,
            0x3feea3dd1d1929fd,
            0xbc845378892be9ae,
            0x3feea34634ccc320,
            0xbc9345f3cee1ae6e,
            0x3feea2b9febc8fb7,
            0xbc93cedd78565858,
            0x3feea23882552225,
            0xbc85c33fdf910406,
            0x3feea1c1c70833f6,
            0x3c5710aa807e1964,
            0x3feea155d44ca973,
            0x3c81079ab5789604,
            0x3feea0f4b19e9538,
            0xbc93b3efbf5e2228,
            0x3feea09e667f3bcd,
            0x3c727df161cd7778,
            0x3feea052fa75173e,
            0xbc6a12ad8734b982,
            0x3feea012750bdabf,
            0x3c93f9924a05b767,
            0x3fee9fdcddd47645,
            0xbc6367efb86da9ee,
            0x3fee9fb23c651a2f,
            0xbc87557939a8b5ef,
            0x3fee9f9298593ae5,
            0xbc80dc3d54e08851,
            0x3fee9f7df9519484,
            0x3c51ed2f56fa9d1a,
            0x3fee9f7466f42e87,
            0xbc781f647e5a3ecf,
            0x3fee9f75e8ec5f74,
            0xbc88e67a9006c909,
            0x3fee9f8286ead08a,
            0xbc86ee4ac08b7db0,
            0x3fee9f9a48a58174,
            0x3c86597566977ac8,
            0x3fee9fbd35d7cbfd,
            0xbc8619321e55e68a,
            0x3fee9feb564267c9,
            0x3c92c0b7028a5c3a,
            0x3feea024b1ab6e09,
            0x3c909ccb5e09d4d3,
            0x3feea0694fde5d3f,
            0x3c8a30faf49cc78c,
            0x3feea0b938ac1cf6,
            0xbc7b32dcb94da51d,
            0x3feea11473eb0187,
            0xbc92dad3519d7b5b,
            0x3feea17b0976cfdb,
            0x3c94ecfd5467c06b,
            0x3feea1ed0130c132,
            0x3c87d51410fd15c2,
            0x3feea26a62ff86f0,
            0x3c65ebe1abd66c55,
            0x3feea2f336cf4e62,
            0xbc760a3629969871,
            0x3feea3878491c491,
            0xbc88a1c52fb3cf42,
            0x3feea427543e1a12,
            0x3c8b18c6e3fdef5d,
            0x3feea4d2add106d9,
            0xbc9369b6f13b3734,
            0x3feea589994cce13,
            0x3c90ec1ddcb1390a,
            0x3feea64c1eb941f7,
            0xbc805e843a19ff1e,
            0x3feea71a4623c7ad,
            0xbc522cea4f3afa1e,
            0x3feea7f4179f5b21,
            0xbc94d450d872576e,
            0x3feea8d99b4492ed,
            0x3c7c88549b958471,
            0x3feea9cad931a436,
            0x3c90ad675b0e8a00,
            0x3feeaac7d98a6699,
            0x3c931143962f7877,
            0x3feeabd0a478580f,
            0x3c8db72fc1f0eab4,
            0x3feeace5422aa0db,
            0x3c93e9e96f112479,
            0x3feeae05bad61778,
            0xbc65b6609cc5e7ff,
            0x3feeaf3216b5448c,
            0xbc8dac42a4a38df0,
            0x3feeb06a5e0866d9,
            0x3c7bf68359f35f44,
            0x3feeb1ae99157736,
            0x3c8b99dd98b1ed84,
            0x3feeb2fed0282c8a,
            0xbc93091fa71e3d83,
            0x3feeb45b0b91ffc6,
            0xbc7885ad50cbb750,
            0x3feeb5c353aa2fe2,
            0xbc5da9b88b6c1e29,
            0x3feeb737b0cdc5e5,
            0xbc82d5e85f3e0301,
            0x3feeb8b82b5f98e5,
            0xbc6c23f97c90b959,
            0x3feeba44cbc8520f,
            0xbc51669428996971,
            0x3feebbdd9a7670b3,
            0xbc92434322f4f9aa,
            0x3feebd829fde4e50,
            0x3c71f2b2c1c4c014,
            0x3feebf33e47a22a2,
            0xbc85ca6cd7668e4b,
            0x3feec0f170ca07ba,
            0xbc9294f304f166b6,
            0x3feec2bb4d53fe0d,
            0x3c71affc2b91ce27,
            0x3feec49182a3f090,
            0xbc8a1e58414c07d3,
            0x3feec674194bb8d5,
            0x3c6dd235e10a73bb,
            0x3feec86319e32323,
            0xbc79740b58a20091,
            0x3feeca5e8d07f29e,
            0xbc87c50422622263,
            0x3feecc667b5de565,
            0x3c9165830a2b96c2,
            0x3feece7aed8eb8bb,
            0x3c8b1c86e3e231d5,
            0x3feed09bec4a2d33,
            0xbc903d5cbe27874b,
            0x3feed2c980460ad8,
            0xbc91bbd1d3bcbb15,
            0x3feed503b23e255d,
            0x3c5986178980fce0,
            0x3feed74a8af46052,
            0x3c90cc319cee31d2,
            0x3feed99e1330b358,
            0xbc89472975b1f2a5,
            0x3feedbfe53c12e59,
            0x3c8469846e735ab3,
            0x3feede6b5579fdbf,
            0x3c7d8157a34b7e7f,
            0x3feee0e521356eba,
            0xbc82dfcd978e9db4,
            0x3feee36bbfd3f37a,
            0x3c8c8a4e231ebb7d,
            0x3feee5ff3a3c2774,
            0x3c8c1a7792cb3387,
            0x3feee89f995ad3ad,
            0xbc888c8d11a142e5,
            0x3feeeb4ce622f2ff,
            0xbc907b8f4ad1d9fa,
            0x3feeee07298db666,
            0x3c889c2ea41433c7,
            0x3feef0ce6c9a8952,
            0xbc55c3d956dcaeba,
            0x3feef3a2b84f15fb,
            0xbc7274aedac8ff80,
            0x3feef68415b749b1,
            0xbc90a40e3da6f640,
            0x3feef9728de5593a,
            0x3c85c620ce76df06,
            0x3feefc6e29f1c52a,
            0xbc68d6f438ad9334,
            0x3feeff76f2fb5e47,
            0xbc8fda52e1b51e41,
            0x3fef028cf22749e4,
            0xbc91eee26b588a35,
            0x3fef05b030a1064a,
            0xbc32141a7b3e2cd8,
            0x3fef08e0b79a6f1f,
            0x3c74ffd70a5fddcd,
            0x3fef0c1e904bc1d2,
            0xbc302899507554e5,
            0x3fef0f69c3f3a207,
            0xbc91bdfbfa9298ac,
            0x3fef12c25bd71e09,
            0xbc80dda2d4c0010c,
            0x3fef16286141b33d,
            0x3c736eae30af0cb3,
            0x3fef199bdd85529c,
            0xbc8a007daadf8d68,
            0x3fef1d1cd9fa652c,
            0x3c8ee3325c9ffd94,
            0x3fef20ab5fffd07a,
            0x3c836909391181d3,
            0x3fef244778fafb22,
            0x3c84e08fd10959ac,
            0x3fef27f12e57d14b,
            0xbc811cd7dbdf9547,
            0x3fef2ba88988c933,
            0x3c63cdaf384e1a67,
            0x3fef2f6d9406e7b5,
            0xbc7ac28b7bef6621,
            0x3fef33405751c4db,
            0x3c676b2c6c921968,
            0x3fef3720dcef9069,
            0xbc7030587207b9e1,
            0x3fef3b0f2e6d1675,
            0xbc808a1883ccb5d2,
            0x3fef3f0b555dc3fa,
            0xbc8cc734592af7fc,
            0x3fef43155b5bab74,
            0xbc8fad5d3ffffa6f,
            0x3fef472d4a07897c,
            0x3c87752a44f587e8,
            0x3fef4b532b08c968,
            0xbc900dae3875a949,
            0x3fef4f87080d89f2,
            0x3c85b66fefeef52e,
            0x3fef53c8eacaa1d6,
            0x3c74a385a63d07a7,
            0x3fef5818dcfba487,
            0x3c5159d9d908a96e,
            0x3fef5c76e862e6d3,
            0xbc82919e2040220f,
            0x3fef60e316c98398,
            0x3c8c254d16117a68,
            0x3fef655d71ff6075,
            0x3c8e5a50d5c192ac,
            0x3fef69e603db3285,
            0xbc8d8c329fbd0e03,
            0x3fef6e7cd63a8315,
            0x3c843a59ac016b4b,
            0x3fef7321f301b460,
            0xbc8ea6e6fbd5f2a6,
            0x3fef77d5641c0658,
            0xbc82d52107b43e1f,
            0x3fef7c97337b9b5f,
            0xbc63e8e3eab2cbb4,
            0x3fef81676b197d17,
            0xbc892ab93b470dc9,
            0x3fef864614f5a129,
            0xbc8b7966cd0d2cd9,
            0x3fef8b333b16ee12,
            0x3c74b604603a88d3,
            0x3fef902ee78b3ff6,
            0xbc776caa4c2ff1cf,
            0x3fef953924676d76,
            0x3c83c5ec519d7271,
            0x3fef9a51fbc74c83,
            0xbc81d5fc525d9940,
            0x3fef9f7977cdb740,
            0xbc8ff7128fd391f0,
            0x3fefa4afa2a490da,
            0x3c855cd8aaea3d21,
            0x3fefa9f4867cca6e,
            0xbc8dae98e223747d,
            0x3fefaf482d8e67f1,
            0x3c8269947c2bed4a,
            0x3fefb4aaa2188510,
            0x3c8ec3bc41aa2008,
            0x3fefba1bee615a27,
            0xbc83b6137e9afe9e,
            0x3fefbf9c1cb6412a,
            0x3c842b94c3a9eb32,
            0x3fefc52b376bba97,
            0xbc69fa74878ba7c7,
            0x3fefcac948dd7274,
            0x3c8a64a931d185ee,
            0x3fefd0765b6e4540,
            0x3c901f3a75ee0efe,
            0x3fefd632798844f8,
            0xbc8e37bae43be3ed,
            0x3fefdbfdad9cbe14,
            0xbc516a9ce6ed84fa,
            0x3fefe1d802243c89,
            0x3c77893b4d91cd9d,
            0x3fefe7c1819e90d8,
            0xbc699c7db2effc76,
            0x3fefedba3692d514,
            0x3c5305c14160cc89,
            0x3feff3c22b8f71f1,
            0x3c64b458677f9840,
            0x3feff9d96b2a23d9,
#endif
        },
};
#endif /* __OBSOLETE_MATH */
