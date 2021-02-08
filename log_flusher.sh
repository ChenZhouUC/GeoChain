#!/bin/bash

echo "***********************************"
echo "====> Existing Logs:"
ls -hl ./logs/
echo "====> Starting Log Flushing..."
rm -v ./logs/*
echo "====> Existing Counters:"
ls -hl ./.VSCodeCounter/
echo "====> Starting Counters Flushing..."
rm -rv ./.VSCodeCounter/*
clear
echo "***********************************"
echo "====> Log Flushing Finished!"
echo "***********************************"