#!/bin/bash

root macros/img1.C -b -q
root macros/img2.C -b -q
root macros/img3.C -b -q
root macros/img4.C -b -q

for journal in {nima,prc,epjc}; do
    (
        cd papers/${journal}
        ln -s ../../../include/canu/canu.hpp .
        mkdir -p out
        ./wrapper_canu_page.sh ../generic/img1.C ../generic/img2.C ../generic/img3.C
        ./wrapper_canu_column.sh ../generic/img4.C
        pdflatex ${journal}_generic.tex
        pdflatex ${journal}_canu.tex
    )
done
