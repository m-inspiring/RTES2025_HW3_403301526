#!/bin/bash

FIFO_PATH="/tmp/my_fifo"

# Create FIFO if it doesn’t exist
if [ ! -p "$FIFO_PATH" ]; then
    mkfifo "$FIFO_PATH"
    chmod 666 "$FIFO_PATH"
fi

# Send messages to FIFO
while true; do
    read -p "Enter message: " message
    if [ "$message" = "exit" ]; then
        break
    fi
    echo "$message" > "$FIFO_PATH"
done