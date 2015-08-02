#include "common.h"
#include "MikanInstaller.h"

extern struct SYSTEM sys;
extern char PageTitle[ PageMax ][ 50 ];

int CAN_INPUT[] =
{
  K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
  K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I, K_J, K_K, K_L, K_M, K_N, K_O, K_P, K_Q, K_R, K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,
  0
};

int CHAR_INPUT[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  0
};

int CHAR_INPUT_SHIFT[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  0
};

int BackButton( int x, int y )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	if ( sys.MODE > 0 && x <= mx && mx < x + 120 && y <= my && my < y + 30 )
	{
		MikanDraw->DrawTexture( 0, x, y, 120, 480, 120, 30 );
		if ( MikanInput->GetMouseNum( 0 ) == -1 )
		{
			MikanWindow->SetWindowName( PageTitle[ --sys.MODE ] );
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, x, y, 0, 480, 120, 30 );
	}
	return 0;
}

int NextButton( int x, int y )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	if ( sys.MODE < PageMax - 1 && x <= mx && mx < x + 120 && y <= my && my < y + 30 )
	{
		MikanDraw->DrawTexture( 0, x, y, 360, 480, 120, 30 );
		if ( MikanInput->GetMouseNum( 0 ) == -1 )
		{
			MikanWindow->SetWindowName( PageTitle[ ++sys.MODE ] );
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, x, y, 240, 480, 120, 30 );
	}
	return 0;
}

int CancelButton( int x, int y )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	if ( sys.MODE < PageMax - 1 && x <= mx && mx < x + 120 && y <= my && my < y + 30 )
	{
		MikanDraw->DrawTexture( 0, x, y, 600, 480, 120, 30 );
		if ( MikanInput->GetMouseNum( 0 ) == -1 )
		{
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, x, y, 480, 480, 120, 30 );
	}
	return 0;
}

int FinishButton( int x, int y )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	if ( x <= mx && mx < x + 120 && y <= my && my < y + 30 )
	{
		MikanDraw->DrawTexture( 0, x, y, 840, 480, 120, 30 );
		if ( MikanInput->GetMouseNum( 0 ) == -1 )
		{
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, x, y, 720, 480, 120, 30 );
	}
	return 0;
}

int MakeButton( int x, int y )
{
	int mx, my;
	mx = MikanInput->GetMousePosX();
	my = MikanInput->GetMousePosY();

	if ( x <= mx && mx < x + 45 && y <= my && my < y + 20 )
	{
		MikanDraw->DrawTexture( 0, x, y, 680, 60, 45, 20 );
		if ( MikanInput->GetMouseNum( 0 ) == -1 )
		{
			return 1;
		}
	} else
	{
		MikanDraw->DrawTexture( 0, x, y, 680, 40, 45, 20 );
	}
	return 0;
}

int TextField( int x, int y, unsigned int size, unsigned int buflen, struct TEXTFIELD *tf )
{
	int w = 0, n = 0;
	int mx, my;
	char buf[ 100 ];

	//�g�ɂȂ�B
	MikanDraw->DrawBox( x - 1, y - 1, size * 10 + 3, 22, tf->color[ 0 ] );
	//���̓X�y�[�X�ɂȂ�B
	MikanDraw->DrawBox( x, y, size * 10 + 1, 20, tf->color[ 1 ] );
	if ( strlen( tf->text ) < size )
	{
		strcpy_s( tf->buf, 1024, tf->text );
	} else
	{
		strncpy_s( tf->buf, 1024, tf->text + tf->first, size );
	}
	MikanDraw->Printf( 2, x + 1, y + 1, tf->buf );

	if ( MikanInput->GetMouseNum( 0 ) == 1 )
	{
		mx = MikanInput->GetMousePosX() - x;
		my = MikanInput->GetMousePosY() - y;
		//�N���b�N���ꂽ�̂Ń`�F�b�N
		if ( 0 <= mx && mx <= (int)size * 10 && 0 <= my && my <= 20 )
		{
			//�e�L�X�g�t�B�[���h���N���b�N���ꂽ�̂ŃJ�[�\���ꏊ��ݒ肷��B
			if ( tf->flag != -1 ) { tf->flag = 1; }
			//[ ][ ][ ][ ][ ] �����B
			// |  |  |  |  |  �J�[�\���ʒu����
			tf->pos = ( mx + 5 ) / 10;
			if ( tf->first + tf->pos > strlen( tf->text ) )
			{
				tf->pos = strlen( tf->text );
			}
		} else
		{
			//�̈�O���N���b�N���ꂽ�̂ŃJ�[�\������͂𖳌��ɂ���B
			if ( tf->flag != -1 ) { tf->flag = 0; }
		}
	}

	if ( tf->flag == 1 )
	{
		//�J�[�\���ړ�
		if ( MikanInput->GetKeyNum( K_RIGHT ) % 10 == 1 && ( tf->first + tf->pos ) < strlen( tf->text ) )
		{
			if ( ++tf->pos > size )
			{
				if ( tf->first + tf->pos <= strlen( tf->text ) )
				{
					++tf->first;
				}
				tf->pos = size;
			}
			tf->timer = 0;
		} else if ( MikanInput->GetKeyNum( K_LEFT ) % 10 == 1 )
		{
			if ( tf->pos == 0 )
			{
				if ( tf->first > 0 )
				{
					--tf->first;
				}
			} else
			{
				--tf->pos;
			}
			tf->timer = 0;
		} else
		{
			++tf->timer;
		}

		//�J�[�\���`��
		if ( ( tf->timer / 30 ) % 2 == 0 )
		{
			MikanDraw->DrawBox( x + ( tf->pos < size ? tf->pos : size ) * 10, y - 1, 2, 22, 0xFF000000 );
		}

		/*  MikanDraw->Printf( 0, x, y+25, "%2d,%2d[%c%c]",
			sys.tfirst, sys.tpos,
			(sys.tfirst + sys.tpos)?str[ sys.tfirst + sys.tpos - 1 ]:' ',
			((sys.tfirst + sys.tpos)<strlen(str))?str[ sys.tfirst + sys.tpos ]:' ');*/

			//������̒ǉ�
		do
		{
			//�͋Z�œ��͉\�����𒲂ׂ�
			if ( MikanInput->GetKeyNum( CAN_INPUT[ n ] ) == 1 )
			{
				buf[ w++ ] = ( MikanInput->GetKeyNum( K_LSHIFT ) > 0 || MikanInput->GetKeyNum( K_RSHIFT ) > 0 ) ? CHAR_INPUT_SHIFT[ n ] : CHAR_INPUT[ n ];
				//        buf[ w ] = '\0';
			}
		} while ( CAN_INPUT[ ++n ] != 0 );

		if ( w > 0 )
		{
			n = strlen( tf->text ) - ( tf->first + tf->pos );
			while ( n-- > 0 )
			{
				tf->text[ tf->first + tf->pos + n + w ] = tf->text[ tf->first + tf->pos + n ];
			}
			n = 0;
			while ( w-- > 0 )
			{
				tf->text[ tf->first + tf->pos + w ] = buf[ w ];
				++n;
			}
			tf->text[ tf->first + tf->pos + n ] = '\0';

			tf->pos += n;
			if ( tf->pos > size )
			{
				tf->first += tf->pos - size;
				tf->pos = size;
			}
		}

		//������̍폜
		if ( strlen( tf->text ) > 0 )
		{
			w = tf->first + tf->pos;

			if ( MikanInput->GetKeyNum( K_BACKSPACE ) % 20 == 1 && w > 0 )
			{
				//�o�b�N�X�y�[�X�̏ꍇ
				//����1������O�ɂ��炷
				do
				{
					tf->text[ w - 1 ] = tf->text[ w ];
				} while ( tf->text[ w++ ] != '\0' );

				//�|�W�V�����̒���
				if ( tf->pos == 0 )
				{
					--tf->first;
				} else
				{
					if ( tf->text[ tf->first + tf->pos ] == '\0' && tf->first > 0 )
					{
						--tf->first;
					} else
					{
						--tf->pos;
					}
				}
			} else if ( MikanInput->GetKeyNum( K_DELETE ) % 20 == 1 )
			{
				//�f���[�g�̏ꍇ

				if ( w < (int)strlen( tf->text ) )
				{
					//��������O�ɂ��炷
					do
					{
						tf->text[ w ] = tf->text[ w + 1 ];
					} while ( tf->text[ w++ ] != '\0' );

					//�|�W�V�����̒���
					if ( tf->first == 0 && tf->pos >= strlen( tf->text ) )
					{
						tf->pos = strlen( tf->text );
					} else if ( tf->pos == size )
					{
						--tf->first;
					}

				} else if ( w > 0 && tf->text[ w ] == '\0' )
				{
					//�Ō��1����������
					tf->text[ w - 1 ] = '\0';
					//�|�W�V�����̐ݒ�
					if ( tf->first == 0 )
					{
						--tf->pos;
					} else
					{
						--tf->first;
					}
				}
			}
		}

		//�m��
		if ( MikanInput->GetKeyNum( K_ENTER ) == -1 )
		{
			return 1;
		}

	}//input flag

	return 0;
}
