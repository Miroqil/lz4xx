lz4xx - A C++ Wrapper for LZ4 algorithm
=======================================

Tsz-Ho Yu (tszhoyu@gmail.com) @ 2015

# Introduction

This is an easy-to-use and independent data compression library based on the LZ4 algorithm. I started this project before I had another side project which needed lossless compression to big model files.

About LZ4 (From Wikipedia): LZ4 is a lossless data compression algorithm that is focused on compression and decompression speed. It belongs to the LZ77 family of byte-oriented compression schemes. The algorithm gives a slightly worse compression ratio than the LZO algorithm - which in turn is worse than algorithms like gzip. However, compression speeds are similar to LZO and several times faster than gzip while decompression speeds can be significantly faster than LZO.

It provides a more intuitive way to compress/decompress d, for example:

### Example 1: Compress a file to another file

    fd_in = fopen("input.data", "r");
    fd_out = fopen("compressed.data.lz4", "w");
    encoder.open(fd_in);
    encoder.encode(fd_out);
    fclose(fd_in);
    fclose(fd_out);

### Example 2: Compress a byte array (e.g. string, image, etc.)

    char input_msg[] = "This is a test string";
    char *compressed_msg = NULL;
    size_t compressed_size = 0;
    encoder.open(input_msg, strlen(input_msg) + 1);     // include end-of-string
    encoder.encode(&compressed_msg, &compressed_size);  // lzBuf is allocated automatically

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

# TODO
* Operator overloading
* Support C++ file s

