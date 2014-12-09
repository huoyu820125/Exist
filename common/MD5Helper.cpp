// MD5Helper.cpp: implementation of the MD5Helper class.
//
//////////////////////////////////////////////////////////////////////

#include "MD5Helper.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


MD5Helper::MD5Helper()
{

}

MD5Helper::~MD5Helper()
{

}

std::string MD5Helper::HashString( const void* input, int size )
{
	Init();
	Update((unsigned char *)input, size);
	unsigned char buff[16] = "";	
	Final((unsigned char*)buff);

	//converte the hash to a string and return it
	return HashToString(buff);	
}

unsigned int MD5Helper::HashValue( const void* input, int size )
{
	Init();
	Update((unsigned char *)input, size);
	unsigned char buff[16] = "";	
	Final((unsigned char*)buff);

	//converte the hash to a string and return it
	std::string key = HashToString(buff);	
	return DJBHash( (const unsigned char*)key.c_str(), key.size() );
}

unsigned int MD5Helper::DJBHash(const unsigned char *buf, int len) 
{
    unsigned int hash = 5381;

    while (len--)
	{
        hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
	}

    return hash;
}

/**
 *  @brief 	This internal member-function
 *  		convertes the hash-data to a
 *  		std::string (HEX).
 *
 *  @param 	data The hash-data to covert into HEX
 *  @return	the converted data as std::string
 */  
std::string MD5Helper::HashToString(unsigned char *data)
{
	/*
	 * using a ostringstream to convert the hash in a
	 * hex string
	 */
	std::ostringstream os;
	for(int i=0; i<16; ++i)
	{
		/*
		 * set the width to 2
		 */
		os.width(2);

		/*
		 * fill with 0
		 */
		os.fill('0');

		/*
		 * conv to hex
		 */
		os << std::hex << static_cast<unsigned int>(data[i]);
	}

	/*
	 * return as std::string
	 */
	return os.str();
}

/**
 *  @brief 	Initialization begins an operation,
 *  		writing a new context
 *  @param 	context	The MD5_CONTEXT context to initialize
 */  
void MD5Helper::Init()
{
	  ctx.count[0] = ctx.count[1] = 0;
	  ctx.state[0] = 0x67452301;
	  ctx.state[1] = 0xefcdab89;
	  ctx.state[2] = 0x98badcfe;
	  ctx.state[3] = 0x10325476;
}

/**
 *  @brief 	Block update operation. Continues an md5
 *  		message-digest operation, processing another
 *  		message block, and updating the context.
 *  @param	context The MD5_CONTEXT context to update
 *  @param	input The data to write into the context
 *  @param	inputLen The length of the input data
 */  
void MD5Helper::Update(unsigned char *input, unsigned int inputLen)
{
	  unsigned int i, index, partLen;

	  /* Compute number of bytes mod 64 */
	  index = (unsigned int)((ctx.count[0] >> 3) & 0x3F);

	  /* Update number of bits */
	  if ( (ctx.count[0] += ((unsigned long int)inputLen << 3))
	       < ((unsigned long int)inputLen << 3))
		ctx.count[1]++;

	  ctx.count[1] += ((unsigned long int)inputLen >> 29);
	  partLen = 64 - index;

	  /*
	   * Transform as many times as possible.
	   */
	  if (inputLen >= partLen) 
	  {
		 Memcpy((MD5_POINTER)&ctx.buffer[index], (MD5_POINTER)input, partLen);
		 Transform(ctx.state, ctx.buffer);

		 for (i = partLen; i + 63 < inputLen; i += 64)
		   Transform(ctx.state, &input[i]);

		 index = 0;
	  }
	  else 
	 	i = 0;

	  /* Buffer remaining input */
	  Memcpy((MD5_POINTER)&ctx.buffer[index],
	              (MD5_POINTER)&input[i],
		      inputLen-i);
}

/**
 *  @brief 	Finalization ends the md5 message-digest 
 *  		operation, writing the the message digest and
 *  		zeroizing the context.
 *  @param	digest This is an OUT parameter which contains
 *  		the created hash after the method returns
 *  @param	context The context to finalize
 */  
void MD5Helper::Final(unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	Encode(bits, ctx.count, 8);

	/* 
	 * Pad out to 56 mod 64.
	 */
	index = (unsigned int)((ctx.count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	Update(PADDING, padLen);

	/* Append length (before padding) */
	Update(bits, 8);

	/* Store state in digest */
	Encode(digest, ctx.state, 16);

	/*
	 * Zeroize sensitive information.
	 */
	Memset((MD5_POINTER)&ctx, 0, sizeof (ctx));
}

/**
 *  @brief 	Encodes input data
 *  @param	output Encoded data as OUT parameter
 *  @param	input Input data
 *  @param	len The length of the input assuming it is a
 *  		multiple of 4
 */  
void MD5Helper::Encode(unsigned char *output, unsigned long int *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/**
 *  @brief 	internal memory management
 *  @param 	output OUT parameter where MD5_POINTER is an unsigned
 *  		char*
 *  @param	value Value to fill the memory with
 *  @param	len The length of the data
 *  
 */  
void MD5Helper::Memset(MD5_POINTER output,int value,unsigned int len)
{
	/*
	 * TODO-Note: Replace "for loop" with standard memset if possible.
	 */
	unsigned int i;
	for (i = 0; i < len; i++)
		((char *)output)[i] = (char)value;
}

/**
 *  @brief 	internal memory management
 *  @param	output OUT parameter where MD5_POINTER is a unsigned
 *  		char*
 *  @param	input Data to copy where MD5_POINTER is a unsigned char*
 *  @param	len The length of the data
 */  
void MD5Helper::Memcpy(MD5_POINTER output, MD5_POINTER input, unsigned int len)
{
	/*
	 * TODO-Note: Replace "for loop" with standard memcpy if possible.
	 */
	unsigned int i;

	for (i = 0; i < len; i++)
		output[i] = input[i];
}


#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/*
 * ROTATE_LEFT rotates x left n bits. 
 * cast to unsigned int to guarantee support for 64Bit System
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | (( (unsigned int) x) >> (32-(n))))

/*
FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

/**
 *  @brief 	Basic transformation. Transforms state based on block.
 *  @param	state	state to transform
 *  @param	block	block to transform
 */  
void MD5Helper::Transform(unsigned long int state[4], unsigned char block[64])
{
	unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */

	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* 
	 * Zeroize sensitive information.
	 */
	Memset((MD5_POINTER)x, 0, sizeof (x));
}

/**
 *  @brief 	Decodes input data into output
 *  @param	output Decoded data as OUT parameter
 *  @param	input Input data
 *  @param	len The length of the input assuming it is a
 *  		multiple of 4
 */  
void MD5Helper::Decode(unsigned long int *output, unsigned char *input, unsigned int len)
{
	  unsigned int i, j;

	  for (i = 0, j = 0; j < len; i++, j += 4)
		 output[i] = ((unsigned long int)input[j]) | 
			     (((unsigned long int)input[j+1]) << 8) |
			     (((unsigned long int)input[j+2]) << 16) |
			     (((unsigned long int)input[j+3]) << 24);
}
