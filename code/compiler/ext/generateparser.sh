#!/bin/sh

java -jar antlr4/antlr4.jar -Dlanguage=Cpp -Xexact-output-dir -o ./../parser4/ antlr4/grammar/GPULang.g4 
