#!/bin/bash

. "./test/com.sh"
cmd="./bin/test2"

functest "$cmd --long" "tl  "
functest "$cmd --mighty" "t m "
functest "$cmd --options" "t  o"

functest "$cmd -l" "t   "
functest "$cmd -long" "t   "

functest "$cmd --long --mighty --options" "tlmo"
functest "$cmd 0 --long 1 --mighty 2 --options 3" "0123tlmo"
functest "$cmd 0 --long=1 --mighty 2 --options 3" "023tlmo"
functest "$cmd 0 --long=1 4 --mighty 2 --options 3" "0423tlmo"

functest "$cmd 0 --long - --mighty -- --options 3" "0---3tlmo"

summary
