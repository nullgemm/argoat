#!/bin/bash

. "./test/com.sh"
cmd="./bin/test3"

functest "$cmd -t" "tl0"
functest "$cmd --tau" "t 0"
functest "$cmd --text" "t 0"

functest "$cmd -t --tau 3 4 5" "5tl7"
functest "$cmd --tau=3 4 5" "45t 0"
functest "$cmd --text one two" "twot 0one"

functest "$cmd --text= one two" "onetwot 0"

summary
