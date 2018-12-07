#!python3
import re, sys
eng = re.compile("0x(..)")

lis = []
for line in sys.stdin:
    line = line.split("//")[0]
    res = eng.findall(line)
    lis += res
print(" ".join(lis))
