#!/bin/bash

FIFO_PATH="/tmp/my_fifo"

# Ensure FIFO exists
if [ ! -p "$FIFO_PATH" ]; then
    echo "FIFO does not exist. Start sender first."
    exit 1
fi

# Receive messages from FIFO
while true; do
    if read line < "$FIFO_PATH"; then
        echo "Received: $line"
    fi
done