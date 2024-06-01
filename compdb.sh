#!/bin/sh
sed -e '1s/^/[\n/' -e '$s/,$/\n]/' *.o.json > compile_commands.json
