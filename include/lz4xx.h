#include "lz4.h"

#include <cstdio>
#include <cstring>

class LZ4DualBuffer
{
    public:
        size_t size;
        size_t space;
        FILE* fd;
        void* buf;
        void* rdptr;
};

class LZ4Encoder
{
    public:
        LZ4Encoder();
        LZ4Encoder(const size_t blockSize);

        // helper - simple functions, if you have enough memory

        // get blocksize

        // write to a file/char stream
        int start(FILE* out);
        int start(char** out, size_t* outlen);
        int write(const char* in, const size_t inlen);
        int write(FILE* in, const size_t inlen = 0);
        int end();

    protected:
        bool ready;                 // ready to write/read
        bool memStream;             // memstream is used
        size_t mBlockSize;          // block size
        size_t mBlockSpace;         // availalble space in block
        char*  mBlock;              // I/O block
        LZ4_stream_t mStream;       // stream
        FILE* mOutPtr;              // file
        char** mOutBuf;             // output buffer
        size_t mOutBufSize;         // output buffer size
        int __write(void* in, const size_t inlen, bool isFile);
        int __flush();              // internal - write block to file
        void __init(const size_t blockSize = 8192);
};

class LZ4Decoder
{
    public:
        LZ4Decoder();
        LZ4Decoder(const size_t blockSize);

        int start(FILE* in);
        int start(const char* in, size_t inlen);
        size_t read(char* out, const size_t outlen);
        size_t read(FILE* out, const size_t outlen = 0);
        int end();

    protected:
        LZ4_streamDecode_t mStreamDecode;   // decode stream
        bool ready;                         // ready to write/read
        size_t mBlockSize;                  // block size
        FILE* mInPtr;
        void __init(const size_t blockSize = 8192);
};

