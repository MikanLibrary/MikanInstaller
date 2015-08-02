#ifndef UNZIP_CLASS
#define UNZIP_CLASS
#include "unzip.h"

class UNZIP
{
private:
	struct RELEASE_STRING
	{
		char *str;
		struct RELEASE_STRING *next;
	} *stack;
	char commondir[ MAX_PATH ];
	virtual void Free( void *r );
	virtual int Push( char *str );
	virtual char *Pop( void );
	virtual char *JoinPath( char *a, char *b );
	virtual char *CutPath( char *a, int c );
	virtual int SetCommonDir( char *d );
public:
	UNZIP( void );
	virtual ~UNZIP( void );
	virtual int IsShiftJIS( int x );
	virtual int IsFileExist( char *Filename );
	virtual int Substitution( char *str, char before, char after );
	virtual int CreateDirectoryReflex( char *Path );
	virtual int ZipNums( char *ZipFilename );
	virtual int Unzip( char *ZipFilename, char *TargetPath );
	virtual int UnzipCount( char *ZipFilename, char *TargetPath, int *count );
	virtual int BufferRelease( void );
	virtual char *GetCommonDir( void );
};

#endif
