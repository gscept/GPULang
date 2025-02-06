#!/bin/sh

java -jar antlr4/antlr4.jar -Dlanguage=TypeScript antlr4/grammar/GPULang.g4 -o ../parser4 
