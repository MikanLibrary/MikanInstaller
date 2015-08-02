#ifndef MIKAN_DEFAULT
#define MIKAN_DEFAULT "C:\\lib\\Mikan"

#define ZIP_DIRECTORY "data/*"
#define ZIP_TEMPORARY "temp/"

#define MIKAN_LIBRARY_BASENAME  "Mikan"
#define MIKAN_TEMPLATE_BASENAME "MikanProject"

#define PageMax 5

struct SYSTEM
{
	int MODE;
	unsigned int timer;
	int select[ 2 ];
	int install[ 2 ];
	int installed;
	int progress[ 4 ];
	int click;
};

struct TEXTFIELD
{
	int flag;
	unsigned int pos;
	unsigned int timer;
	unsigned int first;
	unsigned long color[ 2 ];
	char buf[ 1024 ];
	char text[ 1024 ];
};

int BackButton( int x, int y );

int NextButton( int x, int y );

int CancelButton( int x, int y );

int FinishButton( int x, int y );

int MakeButton( int x, int y );

int TextField( int x, int y, unsigned int size, unsigned int buflen, struct TEXTFIELD *tf );

#endif
