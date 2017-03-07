#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lmcons.h>

#include "MikanInstaller.h"
#include "DirectoryOperations.h"
#include "Unzip/UnzipClass.h"

#include "resource.h"

//後で調べて書き直せるようにする。
#define INSTALL_PATH "C:\\lib\\Mikan2017"

int INSTALL_VERSION = 0;

struct SYSTEM sys = { -1, 0, {},{ 0, 1 }, 0, {}, 0 };

struct TEXTFIELD
	tf[ 2 ] = { { 0, 0, 0, 0, 0xFF000000, 0xFFFFFFFF, "", "" },{ 0, 0, 0, 0, 0xFF000000, 0xFFFFFFFF, "", "" } },
	tfd[ 10 ] = { {-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""},{-1,0,0,0,0,0,"",""} };

class UNZIP Unzip;

unsigned int size[ 4 ];
char *DXSDK_DIR;
char *VSCOMNTOOLS;

int wx, wy, drg = 0;

//char USERHOME[ 1024 ] = "%USERPROFILE%\\Documents\\Visual Studio 2010\\Wizards\\";

char PageTitle[ PageMax ][ 50 ] =
{
	"Mikanライブラリ 簡易インストーラー",
	"Mikanライブラリのインストール場所",
	"インストールの確認",
	"インストール中...",
	"インストール完了"
};

char InstallPath[ 2 ][ 259 ] =
{
	INSTALL_PATH
};

char WIZARDDIR[ 259 ];

class CLASSDIRECTORY *dir = NULL;


int InstallVerSet( int ver )
{
	size[ 1 ] = 1024;
	VSCOMNTOOLS = (char *)calloc(size[1], sizeof(char));
	switch ( ver )
	{
	case 2008:
		INSTALL_VERSION = 2008;
		_dupenv_s( &VSCOMNTOOLS, &( size[ 1 ] ), "VS100COMNTOOLS" );
		strcpy_s( WIZARDDIR, 259, "\\Documents\\Visual Studio 2008\\Wizards\"" );
		break;
	case 2010:
		INSTALL_VERSION = 2010;
		_dupenv_s( &VSCOMNTOOLS, &( size[ 1 ] ), "VS100COMNTOOLS" );
		strcpy_s( WIZARDDIR, 259, "\\Documents\\Visual Studio 2010\\Wizards\"" );
		break;
		/*default:
		size[ 1 ] = 0;
		VSCOMNTOOLS = NULL;
		break;*/
	case 2012:
		INSTALL_VERSION = 2012;
		_dupenv_s( &VSCOMNTOOLS, &( size[ 1 ] ), "VS110COMNTOOLS" );
		strcpy_s( WIZARDDIR, 259, "\\Documents\\Visual Studio 2012\\Wizards\"" );
		break;
	case 2013:
		INSTALL_VERSION = 2013;
		_dupenv_s( &VSCOMNTOOLS, &( size[ 1 ] ), "VS120COMNTOOLS" );
		strcpy_s( WIZARDDIR, 259, "\\Documents\\Visual Studio 2013\\Wizards\"" );
		break;
	case 2015:
		INSTALL_VERSION = 2015;
		_dupenv_s(&VSCOMNTOOLS, &( size[ 1 ] ), "VS140COMNTOOLS");
		strcpy_s(WIZARDDIR, 259, "\\Documents\\Visual Studio 2015\\Wizards\"");
	case 2017:
	default:
		INSTALL_VERSION = 2017;
		_dupenv_s( &VSCOMNTOOLS, &( size[ 1 ] ), "VS140COMNTOOLS" );
		strcpy_s( WIZARDDIR, 259, "\\Documents\\Visual Studio 2017\\Wizards\"" );
		break;
	}

	return 0;
}

void Init()
{
	char ExePath[ MAX_PATH ], *tmp;
	int n, w;
	/*  WNDCLASSEX *wcx;
	wcx = &( _MikanWindow->GetWindowClassEx() );
	wcx->hIcon = LoadIcon( _MikanWindow->GetInstanceHandle(), TEXT( "ICON_32" ) );
	MikanWindow->SetWindowName( "MikanWizardInstaller" );*/

	//環境変数の取得
	size[ 0 ] = 1024;
	tmp = (char *)calloc( size[ 0 ], sizeof( char ) );
	_dupenv_s( &tmp, &( size[ 0 ] ), "DXSDK_DIR" );
	n = 0;
	while ( tmp[ n ] )
	{
		++size[ 0 ];
		if ( tmp[ n ] == '(' || tmp[ n ] == ')' )
		{
			size[ 0 ] += 2;
		}
		++n;
	}
	DXSDK_DIR = (char *)calloc( size[ 0 ] + 1, sizeof( char ) );
	for ( n = w = 0; w < (int)size[ 0 ]; ++n )
	{
		switch ( tmp[ n ] )
		{
		case '(':
			DXSDK_DIR[ w++ ] = '%';
			DXSDK_DIR[ w++ ] = '2';
			DXSDK_DIR[ w++ ] = '8';
			break;
		case ')':
			DXSDK_DIR[ w++ ] = '%';
			DXSDK_DIR[ w++ ] = '2';
			DXSDK_DIR[ w++ ] = '9';
			break;
		default:
			DXSDK_DIR[ w++ ] = tmp[ n ];
			break;
		}
	}
	free( tmp );

	GetModuleFileName( _MikanSystem->GetInstance(), ExePath, MAX_PATH );
	n = strlen( ExePath );
	while ( --n >= 0 && ExePath[ n ] != '\\' )
	{
		if ( '0' <= ExePath[ n ] && ExePath[ n ] <= '9' )
		{
			INSTALL_VERSION *= 10;
			INSTALL_VERSION += ExePath[ n ] - '0';
		}
	}

	InstallVerSet( INSTALL_VERSION );

	dir = new CLASSDIRECTORY( InstallPath[ 0 ] );//MIKAN_DEFAULT);
												 //現在のディレクトリ
	strcpy_s( tf[ 0 ].text, 1024, dir->Current() );
}

void SystemInit( void )
{

	//  _MikanWindow->SetWindow( WS_NORESIZEFULLSCREEN );
	//  _MikanWindow->SetWindowSize( 640, 480 );
	_MikanWindow->SetWindowSize( 640, 480 );
	_MikanWindow->SetWindow( WT_NOFRAME );

	MikanWindow->SetWindowName( "準備中" );
	MikanWindow->SetWindowIcon( "ICON_16" );

	Init();
}

/*const HMODULE GetCurrentModule()
{
	DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
	HMODULE hm = 0;
	::GetModuleHandleEx( flags, reinterpret_cast<LPCTSTR>( GetCurrentModule ), &hm );
	return hm;
} */
void UserInit( void )
{

	//  D3DXIMAGE_INFO image_info;
	//  HMODULE hm;
	/*  GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		reinterpret_cast<LPCTSTR>( UserInit ),
		&hm);*/
		//r = D3DXGetImageInfoFromResource( NULL, "IDB_BITMAP1", &image_info );
	  //  hm = GetModuleHandle(NULL);
	  //  hm = GetCurrentModule();
	  //  hm = GetModuleHandle( "MikanWizardInstaller.exe" );
	  //  r = D3DXGetImageInfoFromResource( hm, "IDB_BITMAP1", &image_info );
	  //  r = _MikanDraw->CreateTexture( 0, hm, "IDB_BITMAP1", TRC_NONE, 0 );
		//hand = (HCURSOR)IDC_HELP;//LoadCursor( NULL, IDC_ARROW );
		//( HCURSOR ) ::LoadImage( NULL, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED );//LoadCursor( NULL, IDC_HAND );
	MikanDraw->CreateTexture( 0, NULL, "UIIMAGE" );//"./data/image.png", TRC_NONE, 0 );//
	MikanDraw->CreateFont( 0, 20, 0xFF000000 );
	MikanDraw->CreateFont( 1, 25, 0xFF000000 );
	MikanDraw->CreateFont( 2, 20, 0xFF000000 );//入力用

}

int RewriteVCProject( char *path )
{
	fpos_t fsize = 0;
	FILE *fp;
	//  char *str = NULL;
	char str[ 1025 ], tag[ 30 ], *newstr;
	char vcinclude[ MAX_PATH * 2 + 2 ], vclibrary[ MAX_PATH * 2 + 2 ];
	int n, size, w;
	__int64 filesize;
	long newfilesize;

	//VCのパス設定
	//includeパス
	//Mikan
	strcpy_s( vcinclude, MAX_PATH * 2 + 2, tf[ 0 ].text );
	vcinclude[ ( size = strlen( vcinclude ) ) - 1 ] = ';';
	//DirectX
  //  strcpy_s( vcinclude + size, MAX_PATH * 2 + 2 - size, DXSDK_DIR );
  //  strcpy_s( vcinclude + strlen( vcinclude ), MAX_PATH * 2 + 2 - size - strlen( vcinclude ), "$(DXSDK_DIR)Include;" );
	strcpy_s( vcinclude + size, MAX_PATH * 2 + 2 - size, "$(DXSDK_DIR)Include;" );
	vcinclude[ strlen( vcinclude ) ] = '\0';

	//ライブラリパス
	//Mikan
	strcpy_s( vclibrary, MAX_PATH * 2 + 2, tf[ 0 ].text );
	vclibrary[ ( size = strlen( vclibrary ) ) - 1 ] = ';';
	//DirectX
  //  strcpy_s( vclibrary + size, MAX_PATH * 2 + 2 - size, DXSDK_DIR );
  //  strcpy_s( vclibrary + strlen( vclibrary ), MAX_PATH * 2 + 2 - size - strlen( vcinclude ), "$(DXSDK_DIR)Lib\\x86;" );
	strcpy_s( vclibrary + size, MAX_PATH * 2 + 2 - size, "$(DXSDK_DIR)Lib\\x86;" );
	vclibrary[ strlen( vclibrary ) ] = '\0';

	//ファイルを読み込みながら加工
	if ( ( w = fopen_s( &fp, path, "r" ) ) == 0 )
	{
		newfilesize = fseek( fp, 0, SEEK_END );
		fgetpos( fp, &( filesize ) );
		fseek( fp, newfilesize, SEEK_SET );

		newfilesize = (int)filesize + MAX_PATH * 4 + 4;
		newstr = (char *)calloc( newfilesize, sizeof( char ) );

		while ( fgets( str, 1024, fp ) != NULL )
		{
			//タグの検索
			size = strlen( str );
			w = 0;
			tag[ 0 ] = '\0';
			for ( n = 0; n < size; ++n )
			{
				if ( str[ n ] == '>' || ( w && str[ n ] == ' ' ) )
				{
					break;
				} else if ( str[ n ] == '<' )
				{
					tag[ w++ ] = str[ ++n ];
					tag[ w ] = '\0';
				} else
				{
					if ( w )
					{
						tag[ w++ ] = str[ n ];
						tag[ w ] = '\0';
					}
				}
			}//end for loop
			if ( strcmp( tag, "IncludePath" ) == 0 )
			{
				//includeのパス設定
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), "    <IncludePath>" );
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), vcinclude );
				for ( ; n < size; ++n )
				{
					if ( str[ n ] == ';' )
					{
						if ( strncmp( str + n + 1, "$(IncludePath)", strlen( "$(IncludePath)" ) ) == 0 )// str[ n + 1 ] == '$' )
						{
							strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), str + n + 1 );
						}
					}
				}
			} else if ( strcmp( tag, "LibraryPath" ) == 0 )
			{
				//ライブラリパス設定
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), "    <LibraryPath>" );
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), vclibrary );
				for ( ; n < size; ++n )
				{
					if ( str[ n ] == ';' )
					{
						if ( strncmp( str + n + 1, "$(LibraryPath)", strlen( "$(LibraryPath)" ) ) == 0 )//str[ n + 1 ] == '$' )
						{
							strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), str + n + 1 );
						}
					}
				}
			} else
			{
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), str );
			}
		}
		fclose( fp );
		if ( ( w = fopen_s( &fp, path, "w" ) ) == 0 )
		{
			fputs( newstr, fp );
			fclose( fp );
		}
		free( newstr );
	} else
	{
		w = 0;
	}

	return 0;
}

int RewriteVCWizard( char *path )
{
	fpos_t fsize = 0;
	FILE *fp;
	//  char *str = NULL;
	char str[ 1025 ], param[ 30 ], *newstr;
	char templatepath[ MAX_PATH ];
	int n, size, w;
	__int64 filesize;
	long newfilesize;

	//テンプレートのパス設定
	//DirectX
	strcpy_s( templatepath, MAX_PATH, tf[ 0 ].text );
	strcpy_s( templatepath + strlen( templatepath ), MAX_PATH - strlen( templatepath ), "MikanProject" );

	//ファイルを読み込みながら加工
	if ( ( w = fopen_s( &fp, path, "r" ) ) == 0 )
	{
		newfilesize = fseek( fp, 0, SEEK_END );
		fgetpos( fp, &( filesize ) );
		fseek( fp, newfilesize, SEEK_SET );

		newfilesize = (int)filesize + MAX_PATH;
		newstr = (char *)calloc( newfilesize, sizeof( char ) );

		while ( fgets( str, 1024, fp ) != NULL )
		{
			//目的箇所の検索
			size = strlen( str );
			w = 0;
			param[ 0 ] = '\0';
			for ( n = 0; n < size; ++n )
			{
				if ( str[ n ] == '"' )
				{
					param[ w++ ] = str[ ++n ];
					param[ w ] = '\0';
				} else if ( w )
				{
					if ( str[ n ] == ' ' )
					{
						break;
					}
					param[ w++ ] = str[ n ];
					param[ w ] = '\0';
				}
			}
			if ( strncmp( str, "Param", 5 ) == 0 && strcmp( param, "ABSOLUTE_PATH" ) == 0 )
			{
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), "Param=\"ABSOLUTE_PATH = " );
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), templatepath );
				for ( ; n < size; ++n )
				{
					if ( str[ n ] == '"' )
					{
						break;
					}
				}
				for ( ; n < size; ++n )
				{
					newstr[ strlen( newstr ) + 1 ] = '\0';
					newstr[ strlen( newstr ) ] = str[ n ];
				}
				newstr[ strlen( newstr ) ] = '\0';
			} else
			{
				strcpy_s( newstr + strlen( newstr ), newfilesize - strlen( newstr ), str );
			}
		}
		fclose( fp );

		if ( ( w = fopen_s( &fp, path, "w" ) ) == 0 )
		{
			fputs( newstr, fp );
			fclose( fp );
		}
		free( newstr );
	} else
	{
		w = 0;
	}

	return 0;
}
unsigned long WINAPI ThreadInstall( void *vdParam )
{
	//int num = (int)vdParam;
	HANDLE hdir;
	WIN32_FIND_DATA status;
	char path[ MAX_PATH ], command[ MAX_PATH * 2 + 6 ], echo[ MAX_PATH * 2 ] = "echo ";
	int pathsize;
	unsigned long ulen = UNLEN;
	OSVERSIONINFOA OsVersionInfo;
	OsVersionInfo.dwOSVersionInfoSize = sizeof( OsVersionInfo );
	GetVersionExA( &OsVersionInfo );

	//Mikanライブラリを作るフォルダを作成。
	Unzip.CreateDirectoryReflex( tf[ 0 ].text );

	if ( ( hdir = FindFirstFile( ZIP_DIRECTORY, &status ) ) != INVALID_HANDLE_VALUE )
	{
		strcpy_s( path, MAX_PATH, ZIP_TEMPORARY );
		Unzip.Substitution( path, '/', '\\' );
		Unzip.CreateDirectoryReflex( path );
		pathsize = strlen( path );

		do
		{
			if ( ( status.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				if ( strncmp( status.cFileName, MIKAN_TEMPLATE_BASENAME, strlen( MIKAN_TEMPLATE_BASENAME ) ) == 0 )
				{
					//Mikanテンプレート
					if ( sys.install[ 1 ] )
					{
						//インストールする設定なのでZIPを解凍する。
						strcpy_s( path, MAX_PATH, ZIP_DIRECTORY );
						Unzip.Substitution( path, '/', '\\' );
						path[ strlen( path ) - 1 ] = '\0';
						strcpy_s( path + strlen( path ), MAX_PATH - strlen( path ), status.cFileName );
						//ZIP内のファイル総数
						sys.progress[ 2 ] = Unzip.ZipNums( path );
						//ZIPの解凍
						Unzip.UnzipCount( path, ZIP_TEMPORARY, &( sys.progress[ 3 ] ) );

						//プロジェクトファイルの加工
						strcpy_s( path, MAX_PATH, ZIP_TEMPORARY );
						strcpy_s( path + strlen( path ), MAX_PATH - strlen( path ), Unzip.GetCommonDir() );//status.cFileName );
						Unzip.Substitution( path, '/', '\\' );
						pathsize = strlen( path );
						/*while( path[ --pathsize ] != '.' )
						{
						  path[ pathsize ] = '\0';
						}
						path[ pathsize++ ] = '\\';
						path[ pathsize ] = '\0';*/

						//default.vcxproj
						strcpy_s( path + pathsize, MAX_PATH - pathsize, ( INSTALL_VERSION == 2008 ) ? "default.vcproj" : "default.vcxproj" );
						RewriteVCProject( path );
						//MikanProject.vsz
						strcpy_s( path + pathsize, MAX_PATH - pathsize, "MikanProject.vsz" );
						RewriteVCWizard( path );

						//Wizardsフォルダを作る。
						strcpy_s( command, MAX_PATH * 2 + 6, "md \"C:\\Users\\" );
						GetUserName( command + strlen( command ), &ulen );
						if ( OsVersionInfo.dwMajorVersion > 5 )
						{
							//vista or 7 or 8
							strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), WIZARDDIR );
						} else
						{
							//XP
							strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), ( INSTALL_VERSION == 2012 ) ? "\\Visual Studio 2012\\Wizards\"" : ( ( INSTALL_VERSION == 2010 ) ? "\\Visual Studio 2010\\Wizards\"" : "\\Visual Studio 2008\\Wizards\"" ) );
						}
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), " >>log.txt" );
						strcpy_s( echo + 5, MAX_PATH * 2 - 5, command );
						system( echo );
						system( command );

						//コマンドを先に作る。
						path[ pathsize ] = '\0';
						strcpy_s( command, MAX_PATH * 2 + 6, "move /Y " );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), path );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), "MikanProject*" );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), " \"C:\\Users\\" );
						GetUserName( command + strlen( command ), &ulen );
						if ( OsVersionInfo.dwMajorVersion > 5 )
						{
							//vista or 7
							strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), WIZARDDIR );
						} else
						{
							//XP
							strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), ( INSTALL_VERSION == 2012 ) ? "\\Visual Studio 2012\\Wizards\"" : ( ( INSTALL_VERSION == 2010 ) ? "\\Visual Studio 2010\\Wizards\"" : "\\Visual Studio 2008\\Wizards\"" ) );
						}
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), " >>log.txt" );
						strcpy_s( echo + 5, MAX_PATH * 2 - 5, command );
						system( echo );
						system( command );

						strcpy_s( command, MAX_PATH * 2 + 6, "rd /S /Q " );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), tf[ 0 ].text );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), "MikanProject >>log.txt" );
						strcpy_s( echo + 5, MAX_PATH * 2 - 5, command );
						system( echo );
						system( command );

						strcpy_s( command, MAX_PATH * 2 + 6, tf[ 0 ].text );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), "MikanProject" );
						Unzip.CreateDirectoryReflex( command );

						strcpy_s( command, MAX_PATH * 2 + 6, "move /Y " );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), path );
						command[ strlen( command ) - 1 ] = ' ';
						//            strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), "* " );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), tf[ 0 ].text );
						strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), " >>log.txt" );
						strcpy_s( echo + 5, MAX_PATH * 2 - 5, command );
						system( echo );
						system( command );

					}
				} else if ( strncmp( status.cFileName, MIKAN_LIBRARY_BASENAME, strlen( MIKAN_LIBRARY_BASENAME ) ) == 0 )
				{
					strcpy_s( path, MAX_PATH, ZIP_DIRECTORY );
					Unzip.Substitution( path, '/', '\\' );
					Unzip.CreateDirectoryReflex( path );
					path[ strlen( path ) - 1 ] = '\0';
					strcpy_s( path + strlen( path ), MAX_PATH - strlen( path ), status.cFileName );
					//Mikanライブラリ
					//ファイルサイズ
					//size = status.nFileSizeHigh * MAXDWORD + status.nFileSizeLow;
					//ZIP内のファイル総数
					sys.progress[ 0 ] = Unzip.ZipNums( path );
					//ZIPの解凍
					Unzip.UnzipCount( path, ZIP_TEMPORARY, &( sys.progress[ 1 ] ) );
				}
			}//end file check
		} while ( FindNextFile( hdir, &status ) );
		FindClose( hdir );

		//ライブラリの移動
		sprintf_s( command, MAX_PATH * 2 + 6, "move temp\\Mikan%d\\* ", INSTALL_VERSION );
		strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), tf[ 0 ].text );
		strcpy_s( command + strlen( command ), MAX_PATH * 2 + 6 - strlen( command ), ( sys.install[ 1 ] ) ? " >>log.txt" : " >log.txt" );
		strcpy_s( echo + 5, MAX_PATH * 2 - 5, command );
		system( echo );
		system( command );
		sys.installed = 1;

	}

	return 0;
}
//    CreateThread( NULL, 0, ThreadGetGamelist, NULL, 0, &ID );

int ExitButton( void )
{
	int mx, my;
	mx = MikanInput->GetMousePosX() - ( 640 - 40 + 15 );
	my = MikanInput->GetMousePosY() - ( 10 + 15 );
	if ( mx * mx + my * my <= 15 * 15 )
	{
		MikanDraw->DrawTexture( 0, 640 - 40, 10, 960 + 30, 480, 30, 30 );
		if ( MikanInput->GetMouseNum( 0 ) == 1 )
		{
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, 640 - 40, 10, 960, 480, 30, 30 );
	}
	return 0;
}

int StartPage( void )
{
	int f = 0, ret;
	int mx, my;
	MikanDraw->Printf( 1, 190, 30, "ようこそ" );
	MikanDraw->Printf( 0, 180, 70, "　Mikanライブラリのインストーラーへようこそ。\nこのインストーラーはMikanライブラリやテンプ\nレート等のインストールを行います。\n\n　DirectXSDKとVisual Studioがインストールされてい\nる必要があります。" );

	if ( DXSDK_DIR == NULL || strcmp( DXSDK_DIR, "" ) == 0 )
	{
		MikanDraw->Printf( 0, 180, 250, "・DirectXSDKがインストールされていません。" );
		++f;
	}

	if ( VSCOMNTOOLS == NULL || strcmp( VSCOMNTOOLS, "" ) == 0 )
	{
		MikanDraw->Printf( 0, 180, 250 + f * 20, "・Visual Studioがインストールされていません。" );
		f += 2;
	}

	ret = CancelButton( 180, 420 );

	if ( f )
	{
		mx = MikanInput->GetMousePosX();
		my = MikanInput->GetMousePosY();

		MikanDraw->Printf( 0, 180, 250 + ( ++f ) * 20, 0xFFFF0000, "　Mikanライブラリをインストールしても使えません。\n一度再起動してみてください。\nまたトラブルシューティングを見て下さい。\nエラー : %d", f > 3 ? 2 : 1 );
		if ( 220 < mx && mx < 220 + 180 && 250 + ( f + 2 ) * 20 < my && my < 250 + ( f + 3 ) * 20 )
		{
			MikanDraw->Printf( 0, 216, 250 + ( f + 2 ) * 20, 0xFF0000FF, "トラブルシューティング" );
			MikanDraw->DrawLine( 220, 250 + ( f + 3 ) * 20, 220 + 180, 250 + ( f + 3 ) * 20, 0xFF0000FF );
			if ( MikanInput->GetMouseNum( 0 ) == -1 )
			{
				ShellExecute( _MikanDraw->GetWindowHandle(), "open", "http://mikan.azulite.net/wiki.cgi?page=%A5%C8%A5%E9%A5%D6%A5%EB%A5%B7%A5%E5%A1%BC%A5%C6%A5%A3%A5%F3%A5%B0", NULL, NULL, SW_SHOW );
			}
		} else
		{
			MikanDraw->DrawLine( 220, 250 + ( f + 3 ) * 20, 220 + 180, 250 + ( f + 3 ) * 20, 0xFFFF0000 );
		}
		NextButton( 490, 420 );
	} else
	{
		MikanDraw->Printf( 0, 180, 250, "　どちらもインストールされていることが確認されまし\nた。\n　Mikanライブラリをインストールできます。" );

		NextButton( 490, 420 );
	}

	return ret;
}

int License( void )
{
	delete( dir );
	return 0;
}

int InstallWhere( void )
{
	int mx, my, num;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	MikanDraw->Printf( 1, 190, 30, "インストール場所" );
	MikanDraw->Printf( 0, 180, 70, "　Mikanライブラリをインストールする場所を選択\nしてください。\n　デフォルトは%sです。", InstallPath[ 0 ] );//MIKAN_DEFAULT);

	if ( TextField( 179, 138, 38, 1024, tf ) )
	{
		//新規作成と移動。
		//フォルダはここでは作らない。
		dir->Cd( tf[ 0 ].text );
		tf[ 0 ].text[ 0 ] = '\0';
		tf[ 0 ].first = tf[ 0 ].pos = tf[ 0 ].flag = 0;
		tf[ 1 ].text[ 0 ] = '\0';
		tf[ 1 ].first = tf[ 1 ].pos = tf[ 1 ].flag = 0;
		//現在のディレクトリ
		strcpy_s( tf[ 0 ].text, 1024, dir->Current() );
	}

	MikanDraw->DrawBox( 178, 158, 383, 202, 0xFF000000 );
	MikanDraw->DrawBox( 179, 159, 381, 200, 0xFFFFFFFF );
	/*  MikanDraw->DrawBox( 300, 300, 30, 30, 0xFF00FF00 );
	  if( MikanInput->GetMouseNum( 0 ) == -1 && 300 <= mx && mx < 330 && 300 <= my && my < 330 )
	  {
		dir->Cd( "C:\\" );
	  }*/
	num = dir->Items() - sys.select[ 0 ] * 10;
	if ( num >= 10 ) { num = 10; }
	for ( --num; num >= 0; --num )
	{
		if ( 179 <= mx && mx < 179 + 380 && 159 + num * 20 <= my && my < 159 + num * 20 + 20 )
		{
			MikanDraw->DrawTexture( 0, 179, 159 + num * 20, 640, 20, 380, 20 );
			if ( MikanInput->GetMouseNum( 0 ) == -1 )
			{
				if ( sys.timer - sys.click < 20 )
				{
					dir->Cd( dir->GetDirectoryName( 10 * sys.select[ 0 ] + num ) );
					sys.select[ 0 ] = 0;

					tf[ 1 ].text[ 0 ] = '\0';
					tf[ 1 ].first = tf[ 1 ].pos = tf[ 1 ].flag = 0;
					//現在のディレクトリ
					strcpy_s( tf[ 0 ].text, 1024, dir->Current() );

				} else
				{
					sys.click = sys.timer;
				}
			}
		} else
		{
			MikanDraw->DrawTexture( 0, 179, 159 + num * 20, 640, 0, 380, 20 );
		}
		if ( dir->GetDirectoryName( 10 * sys.select[ 0 ] + num ) )
		{
			strcpy_s( tfd[ num ].text, 1024, dir->GetDirectoryName( 10 * sys.select[ 0 ] + num ) );
		}
		TextField( 179, 159 + num * 20, 38, 1024, &( tfd[ num ] ) );
	}

	//戻るボタン
	if ( sys.select[ 0 ] > 0 )
	{
		if ( 180 <= mx && mx < 200 && 360 <= my && my < 380 )
		{
			MikanDraw->DrawTexture( 0, 180, 360, 640, 60, 20, 20 );
			if ( MikanInput->GetMouseNum( 0 ) == 1 )
			{
				--sys.select[ 0 ];
			}
		} else
		{
			MikanDraw->DrawTexture( 0, 180, 360, 640, 40, 20, 20 );
		}
	} else
	{
		//    MikanDraw->DrawTexture( 0, 180, 360, 640, 40, 20, 20 );
	}

	//進むボタン
	if ( 10 * ( sys.select[ 0 ] + 1 ) < dir->Items() )
	{
		if ( 540 <= mx && mx < 560 && 360 <= my && my < 380 )
		{
			MikanDraw->DrawTexture( 0, 540, 360, 660, 60, 20, 20 );
			if ( MikanInput->GetMouseNum( 0 ) == 1 )
			{
				++sys.select[ 0 ];
			}
		} else
		{
			MikanDraw->DrawTexture( 0, 540, 360, 660, 40, 20, 20 );
		}
	} else
	{
		//    MikanDraw->DrawTexture( 0, 540, 360, 660, 40, 20, 20 );
	}

	//新規作成
	MikanDraw->Printf( 2, 200, 360, "新規作成:" );
	if ( ( TextField( 290, 360, 20, 1024, tf + 1 ) || MakeButton( 493, 360 ) ) && strcmp( tf[ 1 ].text, "" ) )
	{
		//新規作成と移動。
		//フォルダはここでは作らない。
		dir->Cd( tf[ 1 ].text );
		tf[ 1 ].text[ 0 ] = '\0';
		tf[ 1 ].first = tf[ 1 ].pos = tf[ 1 ].flag = 0;
		//現在のディレクトリ
		strcpy_s( tf[ 0 ].text, 1024, dir->Current() );
	}

	BackButton( 180, 420 );
	if ( NextButton( 490, 420 ) )
	{
		tf[ 0 ].flag = -1;
	}

	return 0;
}

int Check( void )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	MikanDraw->Printf( 1, 190, 30, "インストールの確認" );
	MikanDraw->Printf( 0, 180, 70, "　以下の場所にMikanライブラリをインストール\nします。" );

	TextField( 199, 138, 38, 1024, tf );

	MikanDraw->Printf( 0, 180, 190, "　追加インストールの選択" );

	MikanDraw->Printf( 0, 220, 220, "□Mikan専用テンプレート" );
	if ( sys.install[ 1 ] )
	{
		MikanDraw->DrawTexture( 0, 220, 220, 640, 80, 20, 20 );
	}
	if ( MikanInput->GetMouseNum( 0 ) == -1 && 220 <= mx && mx < 240 && 220 <= my && my < 240 )
	{
		sys.install[ 1 ] = ( sys.install[ 1 ] + 1 ) % 2;
	}

	if ( BackButton( 180, 420 ) )
	{
		tf[ 0 ].flag = 0;
	}
	if ( NextButton( 490, 420 ) )
	{
		//スレッドを生成し、インストール作業を行う。
		CreateThread( NULL, 0, ThreadInstall, NULL, 0, NULL );
	}

	return 0;
}

int Install( void )
{
	MikanDraw->Printf( 1, 190, 30, "インストール中" );

	if ( sys.installed == 0 )
	{
		MikanDraw->Printf( 0, 180, 70, "　インストールしています。\n途中黒いウィンドウが出てきます。" );
		CancelButton( 180, 420 );
	}
	if ( sys.installed )
	{
		MikanDraw->Printf( 0, 180, 70, "　インストール終了。次に進んでください。" );
		NextButton( 490, 420 );
	}

	return 0;
}

int Finish( void )
{
	MikanDraw->Printf( 1, 190, 30, "インストール完了" );
	MikanDraw->Printf( 0, 180, 70, "　Mikanライブラリのインストールは無事完了しま\nした。" );


	if ( FinishButton( 490, 420 ) )
	{
		return 1;
	}

	return 0;
}

int MoveWindow( void )
{
	if ( MikanInput->GetMouseNum( 0 ) == 1 )
	{
		wx = MikanInput->GetMousePosX();
		wy = MikanInput->GetMousePosY();
		drg = 1;
	} else if ( drg && MikanInput->GetMouseNum( 0 ) == -1 )
	{
		drg = 0;
	} else if ( drg )
	{
		wx = MikanWindow->GetPositionX() - ( wx - MikanInput->GetMousePosX() );
		wy = MikanWindow->GetPositionY() - ( wy - MikanInput->GetMousePosY() );
		MikanWindow->SetPositionXY( wx, wy );
		wx = MikanInput->GetMousePosX();
		wy = MikanInput->GetMousePosY();
	}
	return 0;
}

int MainLoop( void )
{
	int ret = 0;
	//画面クリア。
	MikanDraw->ClearScreen();

	MikanDraw->DrawTexture( 0, 0, 0, 0, 0, 640, 480 );

	//  MikanDraw->Printf( 0, 0, 0, "%s,%d", DXSDK_DIR, size[0] );
	//  MikanDraw->Printf( 0, 0, 10, "%s,%d", VS100COMNTOOLS, size[1] );
	//  MikanDraw->Printf( 0, 0, 20, "%s,%d", VS90COMNTOOLS, size[2] );

	switch ( sys.MODE )
	{
	case 0:
		ret = StartPage();
		if ( sys.timer < 30 ) { MikanWindow->SetWindowName( PageTitle[ ( sys.MODE = 0 ) ] ); }
		break;
	case 1:
		InstallWhere();
		break;
	case 2:
		Check();
		break;
	case 3:
		Install();
		break;
	case 4:
		return Finish();
		break;
	default:
		MikanWindow->SetWindowName( PageTitle[ ( sys.MODE = 0 ) ] );
		break;
	}

	MikanDraw->Printf( 0, 10, 200, "ようこそ\nインストール\n場所の設定\nインストール\nの確認\nインストール" );

	if ( ExitButton() )
	{
		return 1;
	}

	MoveWindow();

	++sys.timer;

	//  return ret;
	return ( MikanInput->GetKeyNum( K_ESC ) == 1 ) || ret;
}

void CleanUp( void )
{
}
