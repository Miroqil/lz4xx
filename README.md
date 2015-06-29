lz4xx - A C++ Wrapper for LZ4 algorithm
=======================================

2015 Tsz-Ho Yu (tszhoyu@gmail.com)

# Introduction

This is an easy-to-use and independent data compression library based on the LZ4 algorithm.

## About LZ4 (From Wikipedia)

LZ4 is a lossless data compression algorithm that is focused on compression and decompression speed. It belongs to the LZ77 family of byte-oriented compression schemes.

The algorithm gives a slightly worse compression ratio than the LZO algorithm - which in turn is worse than algorithms like gzip. However, compression speeds are similar to LZO and several times faster than gzip while decompression speeds can be significantly faster than LZO.

# Installation

The source package is supposed to be used as a library, or included directly in the source tree.

Demo programs can be compiled using CMAKE:

    $ mkdir debug
    $ cd debug
    $ cmake ..
    $ make

# Examples

The source code of two demo programs are included in the "example" folder.

* example1.cpp --- Compress a file, and decompress it again.
* example2.cpp --- Compress a text-message from a byte array, and decompress it again.
