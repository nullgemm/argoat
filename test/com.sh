#!/bin/bash

ok=0
fail=0

functest () {
	var=$($1)

	if [ "$var" != "$2" ]
	then
		echo "the following command failed: \"$1\""
		((fail++))
	else
		((ok++))
	fi
}

summary () {
	echo "> $ok tests passed"
	echo "> $fail tests failed"
}
