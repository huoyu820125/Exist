// MD5Helper.h: interface for the MD5Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HASH_H__E8D65D13_34AB_4C85_99F0_6A839207769B__INCLUDED_)
#define AFX_HASH_H__E8D65D13_34AB_4C85_99F0_6A839207769B__INCLUDED_

#include <string>

typedef unsigned char *MD5_POINTER;

//@MD5-hash context.
typedef struct MD5_CONTEXT 
{
	/** state (ABCD) */
	unsigned long int state[4];   	      

	/** number of bits, modulo 2^64 (lsb first) */
	unsigned long int count[2];

	/** input buffer */
	unsigned char buffer[64];
}MD5_CONTEXT;

class MD5Helper  
{
public:
	MD5Helper();
	virtual ~MD5Helper();
	std::string HashString( const void* input, int size );//取得字符串形式的hash值
	unsigned int HashValue( const void* input, int size );//取得数值形式的hash值

protected:
	void Init();
	void Update( unsigned char *input, unsigned int inputLen );
	void Final( unsigned char digest[16] );
	std::string HashToString( unsigned char *data );
	unsigned int DJBHash( const unsigned char *buf, int len );

	void Encode( unsigned char *output, unsigned long int *input, unsigned int len );
	void Memset( MD5_POINTER output,int value,unsigned int len );
	void Memcpy( MD5_POINTER output, MD5_POINTER input, unsigned int len );
	void Transform( unsigned long int state[4], unsigned char block[64] );
	void Decode( unsigned long int *output, unsigned char *input, unsigned int len );

	/**
	 * MD5 context
	 */
	MD5_CONTEXT ctx;
};

#endif // !defined(AFX_HASH_H__E8D65D13_34AB_4C85_99F0_6A839207769B__INCLUDED_)
