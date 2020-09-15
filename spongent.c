
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uchar;
//globals
static uint8_t S[16] = { 0xe, 0xd, 0xb, 0x0, 0x2, 0x1, 0x4, 0xf, 0x7, 0xa, 0x8, 0x5, 0x9, 0xc, 0x3, 0x6};
static uint16_t b = 272;
static uint16_t B = 272 / 8;
static uint16_t n = 256;
uchar state[272 / 8];

uint8_t nextValueForLfsr(uint8_t lfsr)
{
    return (lfsr << 1) | (((lfsr >> 1) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 7)) & 1);
}

/*
    option 1, use:
    * 6 ANDs
    * 3 ORs
    * 2 assignments
    * 7 shifts left
    * 7 shifts right
*/
uint8_t reverse(uint8_t b)
{
    b =    (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b =    (b & 0xCC) >> 2 | (b & 0x33) << 2;
    return (b & 0xAA) >> 1 | (b & 0x55) << 1;
}


void pLayer()
{
    uchar tmp[B];
    memset(tmp, 0, B);

    for (unsigned idx = 0; idx < (b - 1); ++idx)
    {
        uchar bit = (state[idx / 8] >> (idx % 8)) & 0x1;
        // the array is initialized to zero, so if the bit isn't set
        // it doesn't matter where it'd go
        if (bit)
        {
            unsigned dest = (idx * b / 4) % (b - 1);
            tmp[dest / 8] |= 1 << dest % 8;
        }
    }
    //the very last bit stays in place since
    // ((b-1)*b/4)mod(b-1) and (0*b/4)mod(b-1) would both be 0
    tmp[B-1] |= state[B - 1] & 0x80;
    memcpy(state, tmp, B);
}


void permute()
{
    uint8_t lfsr = 0x9e;
    do
    {
        state[0] ^= lfsr;
        state[B - 1] ^= reverse(lfsr);

        lfsr = nextValueForLfsr(lfsr);

        for (unsigned idx = 0; idx < B; ++idx)
            state[idx] = S[state[idx] >> 4] << 4 | S[state[idx] & 0xF];

        pLayer();
    } while (lfsr != 0xFF);
}


void spongent(uchar *input, uchar *output)
{
    unsigned idx = 0;

    if (input[idx] != 0)
    {

        while (input[idx + 1] != 0)
        {
            state[0] ^= input[idx];
            state[1] ^= input[idx + 1];

            permute();
            idx += 2;
        }
    }

    //pad remaining data (or make r bits of padding), insert and permute.
    if (input[idx] == 0)
    {
        state[0] ^= 0x80;
    } else {
        state[0] ^= input[idx];
        state[1] ^= 0x80;
    }
    permute();

    //do until n bits of data are extracted:
    for (unsigned idx = 0; idx < n/8; idx += 2)  //one permute per r bits
    {
        //concatenate r bits of STATE with output
        output[idx] = state[0];
        output[idx + 1] = state[1];

        permute();
    }
}
