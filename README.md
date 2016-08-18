# Splunk-O-Things
Code, Schematics, and Guides for the Splunk O' Things device

## Setup
Follow GitHub's instructions here for downloading: https://help.github.com/articles/cloning-a-repository/

Add a "config.txt" to the SD card on the device.

## "config.txt"
This file is stored on the SD card on the device in the root directory.   
The four items it needs to have are: 
  1. `Port` number to send data to
  2. `Host` IP or DNS address to send data to
  3. `SSID` of the Wi-Fi network the device will connect to
  4. `Password` associated with the `SSID`

This file is formatted in the same order as above. each item is on a new line with no other formatting text.

