#!/bin/bash

echo "***********************************"
echo "====> Existing Logs:"
ls -hl ./logs/
echo "====> Starting Log Flushing..."
rm -v ./logs/*
echo "====> Log Flushing Finished!"
echo "***********************************"
