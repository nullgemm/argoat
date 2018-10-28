#!/bin/bash

. "./test/com.sh"
cmd="./bin/test1"

functest "$cmd" "t   "

functest "$cmd -l" "tl  "
functest "$cmd -m" "t m "
functest "$cmd -o" "t  o"

functest "$cmd --l" "tl  "
functest "$cmd --long" "t   "

functest "$cmd -lmo" "tlmo"
functest "$cmd -lm -o" "tlmo"
functest "$cmd -l -m -o" "tlmo"

functest "$cmd -l 1 -m 2 -o 3" "tlmo"

functest "$cmd -l - -m" "tlm "
functest "$cmd -l --m 3" "tlm "
functest "$cmd -l --m=3" "tlm "

summary
