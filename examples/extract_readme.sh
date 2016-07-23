#!/usr/bin/env sh
#
# This script extracts the examples from the README and places them into
# source files in this directory. This allows us to actually compile and
# test them.

cd $(dirname $0)

# Read and discard all lines up to and including the start of a c-code
# fence opening, i.e. '```c'
cut_prose() {
	while read line; do
		if [ "$line" = '```c' ]; then
			return
		fi
	done
}

# Read all lines up to a closing code fence, i.e. '```', and echo all but
# this last line.
copy_file() {
	while read line; do
		if [ "$line" = '```' ]; then
			return
		fi
		printf '%s\n' "$line"
	done
}

cat ../README.md | {
	cut_prose
	copy_file > client.c
	cut_prose
	copy_file > server.c
}
