#!/bin/bash

cp ../../materials/linters/*.{py,cfg} ../
python3 ../cpplint.py --extensions=c *.[ch] 2>&1 1>/dev/null | awk -F' {2}' '{printf "%-20s %s %s\n", $1, $2, $3}' | sort -k3 | nl
rm -f ../*.{py,cfg}
