#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "lz4xx.h"

char test_data[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce eget cursus leo, pellentesque cursus ex. Fusce porttitor mauris velit, vel consequat massa sagittis sed. Donec felis mi, semper vitae lacus et, mattis faucibus odio. In a tempor diam. Phasellus vestibulum cursus felis in blandit. Etiam erat nibh, semper vitae quam non, ullamcorper molestie arcu. Proin cursus magna id mauris dapibus aliquet. Donec nec laoreet nunc.";

int main(int argc, char* argv[])
{

    // check the number of arguments
    if (argc < 2)
    {
        printf("USAGE: %s [test file]\n", argv[0]);
        return 0;
    }

    // set up encoder and decoder
    size_t BLOCKSIZE = 8192;
    LZ4Encoder encoder(BLOCKSIZE);
    LZ4Decoder decoder(BLOCKSIZE);

    // setup file names
    char inPath[256];
    char lzPath[256];
    char outPath[256];
    sprintf(inPath, "%s", argv[1]);
    sprintf(lzPath, "%s.lz4", argv[1]);
    sprintf(outPath, "%s.lz4.out", argv[1]);

    FILE* fd_in;
    FILE* fd_out;

    char* lzBuf;
    size_t lzBufSize;

    char outBuf[BLOCKSIZE];

    // CASE 1:
    // Encode FILE* to FILE*
    fd_in = fopen(inPath, "r");
    fd_out = fopen(lzPath, "w");
    encoder.start(fd_out);
    encoder.write(fd_in);
    encoder.end();
    fclose(fd_in);
    fclose(fd_out);
    // Decode FILE* to FILE*
    fd_in = fopen(lzPath, "r");
    fd_out = fopen(outPath, "w");
    decoder.start(fd_in);
    decoder.read(fd_out);
    decoder.end();
    fclose(fd_in);
    fclose(fd_out);

    // CASE 2:
    // Encode Byte Array to Byte Array
    encoder.start(&lzBuf, &lzBufSize);
    encoder.write(test_data, strlen(test_data));
    encoder.end();
    printf("TEST_DATA: %d bytes compressed to %d bytes\n", (int)strlen(test_data), (int)lzBufSize);
    // Decode ByteArray to Byte Array
    decoder.start(lzBuf, lzBufSize);
    decoder.read(outBuf, BLOCKSIZE);
    decoder.end();
    printf("INPUT:\n%s\n", test_data);
    printf("OUTPUT:\n%s\n", outBuf);
    free(lzBuf);

    return 0;
}
