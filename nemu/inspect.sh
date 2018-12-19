#!/bin/bash
cat *-log.txt | grep "invalid opcode(" | cut -b 33- | sort
echo --------------
for i in `ls *-log.txt`; do cat $i | grep invalid || echo $i; done

