#!/bin/bash
clear
echo "***********************************"
echo "====> Existing Logs:"
ls -hl ./logs/
echo "====> Starting Log Flushing..."
rm -v ./logs/*
echo "====> Existing Counters:"
ls -hl ./.VSCodeCounter/
echo "====> Starting Counters Flushing..."
rm -rv ./.VSCodeCounter/*
echo "***********************************"
echo "====> Log Flushing Finished!"
echo "***********************************"