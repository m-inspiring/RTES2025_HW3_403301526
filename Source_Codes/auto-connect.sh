#!/bin/bash

# Wi-Fi credentials
SSID="Meysam1"
PASSWORD="HoE@1234"
LOG_FILE="/var/log/auto-connect.log"

# Ensure log file is writable
touch $LOG_FILE
chmod 666 $LOG_FILE

# Check if already connected
if nmcli -t -f NAME connection show --active | grep -q "$SSID"; then
    echo "$(date) - Already connected to $SSID" >> $LOG_FILE
else
    echo "$(date) - Not connected to $SSID. Attempting to connect..." >> $LOG_FILE
    nmcli device wifi connect "$SSID" password "$PASSWORD" >> $LOG_FILE 2>&1
    if [ $? -eq 0 ]; then
        echo "$(date) - Successfully connected to $SSID" >> $LOG_FILE
    else
        echo "$(date) - Failed to connect to $SSID" >> $LOG_FILE
    fi
fi

# Log available networks for debugging
echo "$(date) - Available Wi-Fi networks:" >> $LOG_FILE
nmcli device wifi list >> $LOG_FILE 2>&1
