#!/bin/sh

set -e

PID="$1"
SPEC="$2"

PORT="$(lsof -a -F -i4 -p "$PID" | awk -F: '/^n/ {print $2}')"

printf 'memcached.servers("%s");\n' "$SPEC" |
sed "s/@SERVER@/localhost:$PORT/" >./memcached.servers.vcl
