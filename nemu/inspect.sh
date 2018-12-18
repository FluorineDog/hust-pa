#!/bin/bash
cat *-log.txt | grep "invalid opcode(" | cut -b 33- | sort
