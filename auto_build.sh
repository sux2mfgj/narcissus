#!/bin/bash

inotifywait -e move -mrq src | \
    while  read line  ; do
        make test;
    done 

