#ifndef EXISTFS_H
#define EXISTFS_H

#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include "../include/Exist/frame/ExistType.h"

namespace exist
{
typedef struct VALUE
{
	DATA_KEY			key;			//key
	bool				protracted;		//数据是持久化，非0是，0否（预留字段，暂不实现持久化功能）
	bool				delMark;		//删除标记
	char				*pData;			//数据
	unsigned int		size;			//长度
	//////////////////////////////////////////////////////////////////////////
	//持久化相关
	bool				idxAble;		//可索引，idxPos有效
	mdk::uint64			idxPos;			//索引开始位置
	VALUE				*pParent;		//父容器
}VALUE;

/*
	idx文件格式
		key长度(2)+过期(1)+key(?)+db文件中开始位置(4)+...+key长度(2)+过期(1)+key(?)+db文件中开始位置(4)
	
	定长数据db文件格式
		值(n)+值(n)+...+值(n)

	变长数据db文件格式
		数据长度(4)+值(?)+数据长度(4)+值(?)+...+数据长度(4)++值(?)
*/
class ExistFS
{
public:
	ExistFS();
	~ExistFS(void);
	void SetRootDir( const char *rootDir );
	const char* CreateTable( void *createParam, int createParamSize, unsigned char *path, int size );//创建表
	const char* CreateData( VALUE *pValue );
	const char* MoveFristTable();//移动到第一个表
	const char* GetTable( unsigned char *createData, short &size );//取得表参数
	const char* ReadTable(exist::VALUE *pTable, std::vector<exist::VALUE*> &data );//读取表数据
	const char* WriteValue( VALUE *pValue );

public:
	class ExistFile
	{
	public:
		ExistFile();
		virtual ~ExistFile();
		bool IsOpend();
		bool Open( const char *filename, const char *mode );
		void Close();
		operator FILE*();
		static bool CreateDir( const char *strDir );
		static bool CreateFile( const char *strFile );

	private:
		FILE		*m_pFile;
	};

private:
	void StringReplace(std::string &strBase, const char *src, const char *des);
	void StringTrim(std::string &strBase, const char c);//压缩连续的重复字符成1个
	void GetOnwerDir( VALUE *pValue, std::string &dir );//取得数据所在文件
	const char* SetDBIndex( const char *idxFileName, VALUE *pValue, mdk::uint64 newDBIndex, 
		mdk::uint64 oldDBIndex, mdk::uint8 newDelMark, mdk::uint8 oldDelMark );//设置索引信息
	const char* GetDBIndex( const char *idxFileName, VALUE *pValue, mdk::uint64 &dbPos, mdk::uint8 &delMark );//取得索引信息
	const char* GetDataSize(const char* dataFileName, mdk::uint64 dbIndex, mdk::uint32 &dataSize);//取得数据长度

private:
	std::string m_rootDir;
	ExistFile m_tableFile;
};

}

#endif //EXISTFS_H