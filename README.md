# CLI Arguments parser

My own argparser library written on cpp. Works with integers, floats, bools and constant length strings.

### Description

Simple library with one header 'n one cpp file. Works with creating parser struct, which has methods to add, count and list parsing arguments.
Also it counts repeating arguments and return the last value and works with validating functions for args. The parsing arguments have to have name, short flag, and long flag.


In this work I was supposed to write this library without STL (or self-written) containers and strings, templates and classes.


### Structure 

- _bin/_  - example, which uses library to take two arguments and make sum or mul opertaion
- _lib/_ - library realizationl: header and cpp
- _tests/_ - gtest folder with tests

### Tests

In path "../tests/" there are 54 tests for tracing all types and corner cases in input. Also tests coverage the memory liberating.

