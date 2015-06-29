#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "lz4xx.h"

char test_data[] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce eget cursus leo, pellentesque cursus ex. Fusce porttitor mauris velit, vel consequat massa sagittis sed. Donec felis mi, semper vitae lacus et, mattis faucibus odio. In a tempor diam. Phasellus vestibulum cursus felis in blandit. Etiam erat nibh, semper vitae quam non, ullamcorper molestie arcu. Proin cursus magna id mauris dapibus aliquet. Donec nec laoreet nunc.\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce eget cursus leo, pellentesque cursus ex. Fusce porttitor mauris velit, vel consequat massa sagittis sed. Donec felis mi, semper vitae lacus et, mattis faucibus odio. In a tempor diam. Phasellus vestibulum cursus felis in blandit. Etiam erat nibh, semper vitae quam non, ullamcorper molestie arcu. Proin cursus magna id mauris dapibus aliquet. Donec nec laoreet nunc.\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce eget cursus leo, pellentesque cursus ex. Fusce porttitor mauris velit, vel consequat massa sagittis sed. Donec felis mi, semper vitae lacus et, mattis faucibus odio. In a tempor diam. Phasellus vestibulum cursus felis in blandit. Etiam erat nibh, semper vitae quam non, ullamcorper molestie arcu. Proin cursus magna id mauris dapibus aliquet. Donec nec laoreet nunc.\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce eget cursus leo, pellentesque cursus ex. Fusce porttitor mauris velit, vel consequat massa sagittis sed. Donec felis mi, semper vitae lacus et, mattis faucibus odio. In a tempor diam. Phasellus vestibulum cursus felis in blandit. Etiam erat nibh, semper vitae quam non, ullamcorper molestie arcu. Proin cursus magna id mauris dapibus aliquet. Donec nec laoreet nunc.\
";

int main(int argc, char* argv[])
{

    // set up encoder and decoder
    size_t BLOCKSIZE = 8192;

    // compressed buffer
    char* lzBuf;
    size_t lzBufSize;

    // decomopressed buffer
    char *outBuf;
    size_t outSize;

    // initialize LZ4 encoder and decoder
    LZ4Encoder encoder(BLOCKSIZE);
    LZ4Decoder decoder(BLOCKSIZE);

    lzBufSize = 0; // initialize buffer size

    // encode
    encoder.open(test_data, strlen(test_data) + 1);
    encoder.encode(&lzBuf, &lzBufSize);

    // encode
    decoder.open(lzBuf, lzBufSize);
    decoder.decode(&outBuf, &outSize);

    // display results
    printf("\nINPUT TEXT:\n%s\n", test_data);

    printf("\nOUTPUT TEXT:\n%s\n", outBuf);

    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - COMPRESSION RATE: %.2f%%\n", (int)encoder.getTotalByteRead(), (int)encoder.getTotalByteWritten(), (float)encoder.getTotalByteWritten() * 100.0 / encoder.getTotalByteRead());

    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - DECOMPRESSION RATE: %.2f%%\n", (int)decoder.getTotalByteRead(), (int)decoder.getTotalByteWritten(), (float)decoder.getTotalByteWritten() * 100.0 / decoder.getTotalByteRead());

    free(lzBuf);

    return 0;
}
