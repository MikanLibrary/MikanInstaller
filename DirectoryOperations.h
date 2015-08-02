#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#ifndef CLASSDIRECTORY
#define CLASSDIRECTORY DirectoryOperations

class DirectoryOperations
{
private:
	int mode;
	int page;
	int items;
	char current[ MAX_PATH ]; //åªç›ÇÃÉpÉX
	char **dirs;
	char drive[ 33 ];
public:
	DirectoryOperations( void );
	DirectoryOperations( char *path );
	~DirectoryOperations( void );
	int Free( void );
	int Join( char *result, char *p1, char *p2 );
	char * Current( void );
	int Items( void );
	int GetDrive( void );
	int Makelist( HANDLE hdir, WIN32_FIND_DATA *status, int count );
	char * GetDirectoryName( int num );
	int Cd( char *path = NULL );
	int Ls( int x, int y, int num, int page );
};

#endif
