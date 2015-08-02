#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "UnzipClass.h"

#ifdef _DEBUG
#pragma comment(lib,"Unzip/zlibD.lib")
#else
#pragma comment(lib,"Unzip/zlib.lib")
#endif

UNZIP::UNZIP( void )
{
	stack = NULL;
}

UNZIP::~UNZIP()
{
	BufferRelease();
}

//Private

void UNZIP::Free( void *r )
{
	if ( r )
	{
		free( r );
	}
	r = NULL;
}

int UNZIP::Push( char *str )
{
	struct RELEASE_STRING *prev = NULL;
	if ( stack == NULL )
	{
		stack = ( struct RELEASE_STRING * )calloc( 1, sizeof( RELEASE_STRING ) );
	} else
	{
		prev = stack;
		stack = ( struct RELEASE_STRING * )calloc( 1, sizeof( RELEASE_STRING ) );
	}
	stack->next = prev;
	stack->str = str;
	return 0;
}

char *UNZIP::Pop( void )
{
	struct RELEASE_STRING *del;
	char *ret = NULL;
	if ( stack )
	{
		del = stack;
		ret = stack->str;
		stack = stack->next;
		Free( del );
	}
	return ret;
}

char *UNZIP::JoinPath( char *a, char *b )
{
	int count = 0, read = 0;
	char *result = (char *)calloc( strlen( a ) + strlen( b ) + 2, sizeof( char ) );

	Push( result );

	while ( a[ read ] != '\0' )
	{
		result[ count++ ] = a[ read++ ];
	}
	read = 0;

	if ( result[ count - 1 ] != '\\' )
	{
		result[ count++ ] = '\\';
	}

	while ( b[ read ] != '\0' )
	{
		result[ count++ ] = b[ read++ ];
	}

	return result;
}

char *UNZIP::CutPath( char *a, int c )
{
	int count = 0, read = 0;
	char *result = (char *)calloc( strlen( a ) + 1, sizeof( char ) );

	Push( result );

	while ( a[ read ] != '\0' )
	{
		result[ count++ ] = a[ read++ ];
		if ( read >= c ) { break; }
	}

	return result;
}

int UNZIP::SetCommonDir( char *d )
{
	int i = 0;
	if ( commondir[ 0 ] == '\n' )
	{
		strcpy( commondir, d );
	}

	while ( d[ i ] != '\0' )
	{
		if ( commondir[ i ] != d[ i ] )
		{
			commondir[ i ] = '\0';
			break;
		}
		++i;
	}
	return 0;
}


//Public

int UNZIP::IsShiftJIS( int x )
{
	return ( (BYTE)( ( x ^ 0x20 ) - 0xA1 ) <= 0x3B );
}

int UNZIP::IsFileExist( char *Filename )
{
	return GetFileAttributes( Filename ) != 0xffffffff;
}

int UNZIP::Substitution( char *str, char before, char after )
{
	int count = 0, i;
	int len = strlen( str );
	for ( i = 0; i < len; ++i )
	{
		if ( str[ i ] == before )
		{
			str[ i ] = after;
			++count;
		}
	}
	return count;
}

int UNZIP::CreateDirectoryReflex( char *Path )
{
	char *p = Path;
	char *SubPath;
	int c = 0;
	for ( ; *p; p += IsShiftJIS( *p ) ? 2 : 1 )
	{
		++c;
		if ( *p == '\\' )
		{
			//      SubPath = JoinPath( Path, p );
			SubPath = CutPath( Path, c );
			if ( !IsFileExist( SubPath ) )
			{
				if ( !CreateDirectory( SubPath, NULL ) )
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int UNZIP::ZipNums( char *ZipFilename )
{
	unzFile hUnzip = unzOpen( ZipFilename );
	//  HANDLE hFile;
	//  unsigned char Buffer[8192];
	//  unsigned long SizeRead;
	char ConFilename[ 512 ];
	int len, i, count = 0;

	if ( !hUnzip )
	{
		return -1;
	}

	do
	{
		unz_file_info fileInfo;
		if ( unzGetCurrentFileInfo( hUnzip, &fileInfo, ConFilename, sizeof( ConFilename ), NULL, 0, NULL, 0 ) != UNZ_OK )
		{
			break;
		}

		len = strlen( ConFilename );
		for ( i = 0; i < len; ++i )
		{
			if ( ConFilename[ i ] == '/' )
			{
				ConFilename[ i ] = '\\';
			}
		}

		// ディレクトリの場合
		if ( len >= 2 && !IsShiftJIS( ConFilename[ len - 2 ] ) && ConFilename[ len - 1 ] == '\\' )
		{
			//      CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
			continue;
		}

		// ファイルの場合
		if ( unzOpenCurrentFile( hUnzip ) != UNZ_OK )
		{
			break;
		}
		//    ++count;
		//    CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
		//    hFile = CreateFile( JoinPath( TargetPath, ConFilename ) , GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		++count;
		//    while( ( SizeRead = unzReadCurrentFile( hUnzip, Buffer, sizeof( Buffer ) ) ) > 0 )
		//    {
		//      ++count;
		//      //WriteFile( hFile, Buffer, SizeRead, &SizeRead, NULL );
		//    }

		//    FlushFileBuffers( hFile );
		//    CloseHandle( hFile );
		unzCloseCurrentFile( hUnzip );

	} while ( unzGoToNextFile( hUnzip ) != UNZ_END_OF_LIST_OF_FILE );

	unzClose( hUnzip );

	return count;
}

int UNZIP::Unzip( char *ZipFilename, char *TargetPath )
{
	unzFile hUnzip = unzOpen( ZipFilename );
	HANDLE hFile;
	unsigned char Buffer[ 8192 ];
	unsigned long SizeRead;
	char ConFilename[ 512 ];
	int len, i, count = 0;

	if ( !hUnzip )
	{
		return -1;
	}
	commondir[ 0 ] = '\n';

	do
	{
		unz_file_info fileInfo;
		if ( unzGetCurrentFileInfo( hUnzip, &fileInfo, ConFilename, sizeof( ConFilename ), NULL, 0, NULL, 0 ) != UNZ_OK )
		{
			break;
		}

		len = strlen( ConFilename );
		SetCommonDir( ConFilename );
		for ( i = 0; i < len; ++i )
		{
			if ( ConFilename[ i ] == '/' )
			{
				ConFilename[ i ] = '\\';
			}
		}

		// ディレクトリの場合
		if ( len >= 2 && !IsShiftJIS( ConFilename[ len - 2 ] ) && ConFilename[ len - 1 ] == '\\' )
		{
			CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
			continue;
		}

		// ファイルの場合
		if ( unzOpenCurrentFile( hUnzip ) != UNZ_OK )
		{
			break;
		}
		++count;
		CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
		hFile = CreateFile( JoinPath( TargetPath, ConFilename ), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );

		while ( ( SizeRead = unzReadCurrentFile( hUnzip, Buffer, sizeof( Buffer ) ) ) > 0 )
		{
			WriteFile( hFile, Buffer, SizeRead, &SizeRead, NULL );
		}

		FlushFileBuffers( hFile );
		CloseHandle( hFile );
		unzCloseCurrentFile( hUnzip );

	} while ( unzGoToNextFile( hUnzip ) != UNZ_END_OF_LIST_OF_FILE );

	unzClose( hUnzip );

	return count;
}

int UNZIP::UnzipCount( char *ZipFilename, char *TargetPath, int *count )
{
	unzFile hUnzip = unzOpen( ZipFilename );
	HANDLE hFile;
	unsigned char Buffer[ 8192 ];
	unsigned long SizeRead;
	char ConFilename[ 512 ];
	int len, i;

	if ( !hUnzip )
	{
		return 1;
	}
	commondir[ 0 ] = '\n';

	do
	{
		unz_file_info fileInfo;
		if ( unzGetCurrentFileInfo( hUnzip, &fileInfo, ConFilename, sizeof( ConFilename ), NULL, 0, NULL, 0 ) != UNZ_OK )
		{
			break;
		}

		len = strlen( ConFilename );
		SetCommonDir( ConFilename );
		for ( i = 0; i < len; ++i )
		{
			if ( ConFilename[ i ] == '/' )
			{
				ConFilename[ i ] = '\\';
			}
		}

		// ディレクトリの場合
		if ( len >= 2 && !IsShiftJIS( ConFilename[ len - 2 ] ) && ConFilename[ len - 1 ] == '\\' )
		{
			CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
			continue;
		}

		// ファイルの場合
		if ( unzOpenCurrentFile( hUnzip ) != UNZ_OK )
		{
			break;
		}
		++( *count );
		CreateDirectoryReflex( JoinPath( TargetPath, ConFilename ) );
		hFile = CreateFile( JoinPath( TargetPath, ConFilename ), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );

		while ( ( SizeRead = unzReadCurrentFile( hUnzip, Buffer, sizeof( Buffer ) ) ) > 0 )
		{
			WriteFile( hFile, Buffer, SizeRead, &SizeRead, NULL );
		}

		FlushFileBuffers( hFile );
		CloseHandle( hFile );
		unzCloseCurrentFile( hUnzip );

	} while ( unzGoToNextFile( hUnzip ) != UNZ_END_OF_LIST_OF_FILE );

	unzClose( hUnzip );

	return 0;
}


int UNZIP::BufferRelease( void )
{
	char *del;
	while ( stack )
	{
		if ( ( del = Pop() ) )
		{
			Free( del );
		}
	}
	return 0;
}

char *UNZIP::GetCommonDir( void )
{
	int i = strlen( commondir );

	while ( i >= 0 && commondir[ i ] != '/' ) { commondir[ i-- ] = '\0'; }

	if ( commondir[ 0 ] == '\0' || commondir[ 0 ] == '\n' ) { return ""; }
	return commondir;
}
