#pragma once
#define str_temp(x) #x
#define str(x) str_temp(x)

#define name_concat_temp(x, y) x##y
#define name_concat(x, y) name_concat_temp(x, y)
#define name_concat3(x, y, z) name_concat(name_concat(x, y), z)
#define name_concat4(x, y, z, w) name_concat3(name_concat(x, y), z, w)
#define name_concat5(x, y, z, v, w) name_concat4(name_concat(x, y), z, v, w)
