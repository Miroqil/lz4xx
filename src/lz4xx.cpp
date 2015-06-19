#include "lz4xx.h"

LZ4Encoder::LZ4Encoder()
{
    __init(8192);
}

LZ4Encoder::LZ4Encoder(const size_t blockSize)
{
    __init(blockSize);
}

void LZ4Encoder::__init(const size_t blockSize)
{
    ready = false;
    mBlockSize = blockSize;
    mOutPtr = NULL;
    mBlock = NULL;
}

int LZ4Encoder::start(FILE* out)
{
    // set file
    mOutPtr = out;
    // init LZ4
    LZ4_resetStream(&mStream);
    // init block size
    mBlockSpace = mBlockSize;
    mBlock = new char[mBlockSize];
    // set ready
    ready = true;
    memStream = false;
    return 0;
}

int LZ4Encoder::start(char** out, size_t* outlen)
{
    mOutPtr = open_memstream(out, outlen);
    // init LZ4
    LZ4_resetStream(&mStream);
    // init block size
    mBlockSpace = mBlockSize;
    mBlock = new char[mBlockSize];
    // set ready
    ready = true;
    memStream = true;
    return 0;
}

int LZ4Encoder::write(const char* in, const size_t inlen)
{
    return __write((void*)in, inlen, false);
}

int LZ4Encoder::write(FILE* in, const size_t inlen)
{
    return __write((void*)in, inlen, true);
}

int LZ4Encoder::__write(void* in, const size_t inlen, bool isFile)
{
    int remain = ((inlen == 0 && isFile)? -1: (int)inlen);
    char* inPtr = (char*)in;
    while (remain > 0 || remain == -1)
    {
        // determine write size
        size_t writeSize = (remain > (int)mBlockSpace || remain == -1) ? (mBlockSpace) : (remain);
        char* bufPtr = mBlock + mBlockSize - mBlockSpace;
        // copy to buffer
        if (isFile)
        {
            writeSize = fread(bufPtr, 1, writeSize, (FILE*)in);
            if (writeSize == 0)
            {
                break;
            }
        }
        else
        {
            memcpy(bufPtr, inPtr, writeSize);
            inPtr += writeSize;
        }
        // update available space and pointer
        mBlockSpace -= writeSize;
        bufPtr += writeSize;
        remain = remain - (remain == -1? 0 : writeSize);
        // write when pointer full
        if (mBlockSpace == 0){
            int status = __flush();
            if (status == -1)
            {
                return -1;
            }
        }
    }
    return 0;
}


int LZ4Encoder::end()
{
    if (mBlockSpace != mBlockSize)
    {
        __flush();
    }
    int zero = 0;
    fwrite(&zero, sizeof(zero), 1, mOutPtr);
    if (mBlock != NULL)
    {
        delete[] mBlock;
    }
    if (memStream)
    {
        fclose(mOutPtr);
    }
    memStream = false;
    ready = false;
    return 0;
}

int LZ4Encoder::__flush()
{
    // current block size
    size_t rawBlockSize = mBlockSize - mBlockSpace;
    char cBlock[LZ4_COMPRESSBOUND(rawBlockSize)];
    int cBlockSize = LZ4_compress_fast_continue(&mStream, mBlock, cBlock, rawBlockSize, sizeof(cBlock), 1);
    if (cBlockSize <= 0)
    {
        return -1; // if compression fails
    }
    printf("%6d bytes -> %6d bytes, compression rate %10.2f%%\n", (int) rawBlockSize, (int) cBlockSize, (float)(100.0 * cBlockSize)/rawBlockSize);
    // write to mOutPtr
    fwrite(&cBlockSize, sizeof(cBlockSize), 1, mOutPtr);
    fwrite(&cBlock, 1, (size_t) cBlockSize, mOutPtr);
    // reset block
    mBlockSpace = mBlockSize;
    return 0;
}

LZ4Decoder::LZ4Decoder()
{
    __init(8192);
}

LZ4Decoder::LZ4Decoder(const size_t blockSize)
{
    __init(blockSize);
}

void LZ4Decoder::__init(const size_t blockSize)
{
    ready = false;
    mBlockSize = blockSize;
    mInPtr = NULL;
}

int LZ4Decoder::start(FILE* in)
{
    mInPtr = in;

    LZ4_setStreamDecode(&mStreamDecode, NULL, 0);

    ready = true;

    return 0;
}

int LZ4Decoder::start(const char* in, size_t inlen)
{
    mInPtr = fmemopen((void*)in, inlen, "r");
    if (mInPtr == NULL)
    {
        return -1;
    }
    LZ4_setStreamDecode(&mStreamDecode, NULL, 0);
    ready = true;
    return 0;
}

size_t LZ4Decoder::read(FILE* out, const size_t outlen)
{
    char outBuf[mBlockSize];
    size_t totalRead = 0;
    while (1)
    {
        size_t readSize = read(outBuf, mBlockSize);
        if (readSize == 0)
        {
            break;
        }
        fwrite(outBuf, 1, readSize, out);
        totalRead += readSize;
        if ((outlen - totalRead) < mBlockSize && outlen != 0)
        {
            break;
        }
    }
    return totalRead;
}

size_t LZ4Decoder::read(char* out, const size_t outlen)
{
    int inBlockSize;
    char cmpBuf[LZ4_COMPRESSBOUND(mBlockSize)];
    char* outPtr = out;
    while (1)
    {
        // read from input
        size_t readCount = fread(&inBlockSize, sizeof(inBlockSize), 1, mInPtr);
        if (inBlockSize <= 0 || readCount != 1)
        {
            return 0;
        }
        size_t readCountBuf = fread(cmpBuf, 1, inBlockSize, mInPtr);
        if ((int)readCountBuf != inBlockSize)
        {
            break;
        }

        // decode
        const int outBlockSize = LZ4_decompress_safe_continue(&mStreamDecode, cmpBuf, outPtr, inBlockSize, mBlockSize);
        outPtr += outBlockSize;
        if ((int)(out + outlen - outPtr) < (int)mBlockSize)
        {
            break;
        }
    }
    return outPtr - out;
}

int LZ4Decoder::end()
{
    ready = false;
    return 0;
}

