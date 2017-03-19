#! /bin/bash

rm -rf ./results/*
make
./simulator input.txt out.svg

