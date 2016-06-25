#!/bin/bash

g++ josh_parser.cc  josh_builtin.cc josh_main.cc josh_aux.cc -lreadline -std=c++11 -o josh
sudo cp josh /usr/local/bin
rm josh
