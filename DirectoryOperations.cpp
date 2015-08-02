#include "DirectoryOperations.h"
#include "common.h"

DirectoryOperations::DirectoryOperations( void )
{
	mode = 0;
	items = 0;
	dirs = NULL;
	Cd();
}

DirectoryOperations::DirectoryOperations( char *path )
{
	mode = 0;
	items = 0;
	dirs = NULL;
	Cd( path );
}

DirectoryOperations::~DirectoryOperations( void )
{
	Free();
}

int DirectoryOperations::Free( void )
{
	while ( --items >= 0 )
	{
		free( dirs[ items ] );
	}
	if ( dirs ) { free( dirs ); }
	return 0;
}

int DirectoryOperations::Join( char *result, char *p1, char *p2 )
{
	int count = 0, read = 0;

	while ( p1[ read ] != '\0' )
	{
		result[ count++ ] = p1[ read++ ];
	}

	read = 0;
	while ( p2[ read ] != '\0' )
	{
		result[ count++ ] = p2[ read++ ];
	}

	result[ count ] = '\0';
	return 0;
}

char * DirectoryOperations::Current( void )
{
	return current;
}

int DirectoryOperations::Items( void )
{
	return items;
}

int DirectoryOperations::GetDrive( void )
{
	int num = 0, n;
	unsigned long d;

	d = GetLogicalDrives();

	for ( n = 0; n < 32; ++n )
	{
		if ( ( 1 << n ) & d )
		{
			drive[ num++ ] = 'A' + n;
		}
	}

	drive[ num ] = '\0';

	return num;
}

int DirectoryOperations::Makelist( HANDLE hdir, WIN32_FIND_DATA *status, int count )
{
	char filepath[ MAX_PATH ] = "";
	int ret, flag = 0;

	//�f�B���N�g������.(���݃f�B���N�g��)�łȂ���Ε�������R�s�[���Ă����B
	if ( ( status->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 &&
		strcmp( status->cFileName, "." ) != 0 )
	{
		//join( filepath, current, status.cFileName );
		strcpy_s( filepath, MAX_PATH, status->cFileName );//strcpy( filepath, status->cFileName );
		flag = 1;
		//printf("%s\n",status->cFileName);

	}

	//���Ɉړ��B
	if ( FindNextFile( hdir, status ) )
	{
		//��������炵���̂ōċA����B
		ret = Makelist( hdir, status, count + flag );//(filepath[ 0 ] != '\0') );
		if ( flag )//filepath[ 0 ] != '\0' )
		{
			//�f�B���N�g���������̂ŗ̈�ɃR�s�[����B
			dirs[ --ret ] = (char *)calloc( strlen( filepath ) + 1, sizeof( char ) );
			strcpy_s( dirs[ ret ], strlen( filepath ) + 1, filepath );//      strcpy( dirs[ ret-- ], filepath );
			return ret;
		}
	} else
	{
		//�Ō�

		//�f�B���N�g���ꗗ�̉���B
		Free();
		//�V�����̈�����B
	//    dirs = (char **)calloc( (items = count + (filepath[ 0 ] != '\0') + 2 ), sizeof( char * ) );
		count += flag;
		dirs = (char **)calloc( ( items = count + 1 ), sizeof( char * ) );

		if ( strcmp( current + 1, ":\\" ) == 0 )
		{
			dirs[ 0 ] = (char *)calloc( 3, sizeof( char ) );
			strcpy_s( dirs[ 0 ], 3, ".." );
			++count;
		} else
		{
			--items;
		}

		if ( flag )//filepath[ 0 ] != '\0' && strcmp( filepath, "" ) )
		{
			//�f�B���N�g���������̂ŗ̈�ɃR�s�[����B
			--count;
			dirs[ count ] = (char *)calloc( strlen( filepath ) + 1, sizeof( char ) );
			strcpy_s( dirs[ count ], strlen( filepath ) + 1, filepath );//      strcpy( dirs[ count ], filepath );
		}

		//���̃f�B���N�g���ԍ���Ԃ��B
		return count;
	}

	return ret;
}

char * DirectoryOperations::GetDirectoryName( int num )
{
	if ( num < items )
	{
		return dirs[ num ];
	}
	return NULL;
}

int DirectoryOperations::Cd( char *path )
{
	int n;
	HANDLE hdir;
	WIN32_FIND_DATA status;

	if ( path == NULL || path[ 0 ] == '\0' )
	{
		//�R���s���[�^
		Free();
		items = GetDrive();

		dirs = (char **)calloc( items, sizeof( char * ) );

		for ( n = 0; n < items; ++n )
		{
			dirs[ n ] = (char *)calloc( 4, sizeof( char ) );
			dirs[ n ][ 0 ] = drive[ n ];
			dirs[ n ][ 1 ] = ':';
			dirs[ n ][ 2 ] = '\\';
		}
		strcpy_s( current, MAX_PATH, "COMPUTER" );//    strcpy( current, "COMPUTER" );

		return 0;
	} else if ( strcmp( path, ".." ) == 0 )
	{
		//1��Ɉړ��B
		if ( strcmp( current + 1, ":\\" ) == 0 )
		{
			return Cd( NULL );
		} else
		{
			n = strlen( current ) - 1;
			for ( --n; n >= 0; --n )
			{
				if ( current[ n ] == '\\' )
				{
					current[ n + 1 ] = '*';
					current[ n + 2 ] = '\0';
					break;
				}
			}
		}
	} else if ( strcmp( path, "." ) == 0 )
	{
		//�����ꏊ�Ȃ̂ňړ����Ȃ��B
	} else
	{
		if ( path[ 1 ] == ':' && path[ 2 ] == '\\' )
		{
			//��΃p�X�B
			strcpy_s( current, MAX_PATH, path );////      strcpy( current, path );
		} else
		{
			//���΃p�X�Ȃ̂ŒP���R�s�[�B
			strcat_s( current, MAX_PATH, path );//      strcat( current, path );
		}
		n = strlen( current );
		if ( current[ n - 1 ] != '\\' )
		{
			current[ n++ ] = '\\';
		}
		current[ n ] = '*';
		current[ n + 1 ] = '\0';
	}

	if ( ( hdir = FindFirstFile( current, &status ) ) != INVALID_HANDLE_VALUE )
	{
		current[ strlen( current ) - 1 ] = '\0';
		Makelist( hdir, &status, 0 );
		FindClose( hdir );
	} else
	{
		current[ strlen( current ) - 1 ] = '\0';

		//�f�B���N�g���ꗗ�̉���B
		Free();
		//�V�����̈�����B
	//    dirs = (char **)calloc( (items = count + (filepath[ 0 ] != '\0') + 2 ), sizeof( char * ) );
		dirs = (char **)calloc( ( items = 1 ), sizeof( char * ) );
		//1��ɍs�����߂̃p�X�͗p�ӂ��Ă����B
		dirs[ 0 ] = (char *)calloc( 3, sizeof( char ) );
		strcpy_s( dirs[ 0 ], 3, ".." );
	}

	return 0;
}

int DirectoryOperations::Ls( int x, int y, int num, int page )
{
	int n, ct = 0;

	for ( n = page * num; ( n < items ) && ( ct < num ); ++n )
	{
		MikanDraw->Printf( 0, x, y + 20 * ( ct++ ), "%s", dirs[ n ] );
	}

	return items;
}