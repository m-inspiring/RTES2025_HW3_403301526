#!/usr/bin/env bash

PORT=8080

echo "Listening on port $PORT"
echo

while true; do
  BODY="Hello I,m Meysam Elhambakhsh $(date '+%Y-%m-%d %H:%M:%S')"

  echo -e "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n$BODY" \
    | nc -l -p "$PORT" -q 1 >/dev/null 2>&1

  echo
  echo "$BODY"
done
