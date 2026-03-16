#!/bin/bash

mkdir -p out
for f in $@; do
    root -l _canu_2d_column.C\(\"$f\"\) -q -b
done
