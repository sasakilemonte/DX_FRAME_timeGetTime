//�t���[���A�j���[�V�����̃x�[�X�v���O�����ł��B
//���Ԃ̎擾��timeGetTime()���g�p���Ă��邽�߁A
//���̃L�[�t���[���A�j���[�V�����̂ЂȌ`���g���ɂ�
//�v���W�F�N�g�̃v���p�e�B�̍��ځA
//�\���v���p�e�B�̃����J�[�̒ǉ��̈ˑ��t�@�C����winmm.lib��ǉ��̂���
//(C)2013 lemonte.net/���X�؏���

//[DX���C�u�����u����]http://homepage2.nifty.com/natupaji/DxLib/
//[VisualStudio2012�ł�DX���C�u�����̎g����]http://homepage2.nifty.com/natupaji/DxLib/dxuse_vc2012.html
#include "DxLib.h"

//------------------------------------------------
//�Q�[���̓���p�O���[�o���ϐ��������ɋL�q���܂�
//------------------------------------------------







//---------------------------------------------------------
//�Q�[���̓���p�@�֐��v���g�^�C�v�錾�������ɋL�q���܂�
//---------------------------------------------------------







//----------------------------------------------------------
//�t���[�����[�g�ݒ�(16.66666��1000�~���b=60�t���[���̈Ӗ�)
#define Framerate 16.6666
//----------------------------------------------

//--------------------------------------
//�f�o�b�O�p�R���\�[���o��
//#define console_debug
//--------------------------------------

//-----------------------------------------------
//�T���v���̃}�E�X���W�ƍ��{�^���̌��o�\���p�ł�
//�v��Ȃ��ꍇ�͍폜���Ă��ǂ��ł��B

//�}�E�X���{�^����������Ă��邩�H
bool mouseDown=false;
//�}�E�XX���W
int mouseX   =0;
//�}�E�XY���W
int mouseY   =0;
//�}�E�X���{�^����������Ă���Ƃ��̃J�E���g�A�b�v�p�ł��B
int mouseCnt=0;
//-----------------------------------------------

//--------------------------------------
//�T���v���̃t���[���J�E���g�ł�
//�v��Ȃ��ꍇ�͍폜���Ă��ǂ��ł��B
int frame_count=0;
//--------------------------------------

//�v���g�^�C�v�錾
void task_init();
void task_content_load();
void task_update();
void task_draw();

//�E�B���h�E�Y���C���֐��ł��B
//Windows�̃v���O�����ɂ����āA��ԍŏ��ɌĂ΂��֐��ł��B
//[WinMain]http://msdn.microsoft.com/ja-jp/library/cc364870.aspx
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 LPSTR lpCmdLine, int nCmdShow )
{
	//�����������̌Ăяo��
	task_init();

	// �^�C�g���� test �ɕύX(�K�{)
	//[SetMainWindowText]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_other.html#R15N13
	SetMainWindowText( "�t���[���A�j���[�V����(timeGetTime�g�p)" ) ;

	//�E�B���h�E���[�h�ŋN��(�K�{)(TRUE�Ȃ�E�B���h�E���[�hfalse�Ȃ�t���X�N���[�����[�h�œ��삵�܂�)
	//[ChangeWindowMode]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_other.html#R11N1
    ChangeWindowMode( TRUE );

	//��ʃ��[�h�̐ݒ�(�K�{)
	//��ʂ̍L��800��600�s�N�Z���ŁA
	//16�r�b�g��DX���C�u�����̕W���F�r�b�g���ŁA65536�F���g���ĉ摜��\�����܂��B
	//[SetGraphMode]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N1
	SetGraphMode( 800 , 600 , 16 ) ;

	// �c�w���C�u��������������(�K�{)
	//[DxLib_Init()]http://homepage2.nifty.com/natupaji/DxLib/dxfunc.html#R1N1
	if( DxLib_Init() == -1 ) return -1;

	//�O���t�B�b�N�A�T�E���h�Ȃǂ̃��\�[�X�ǂݍ��݂ł��B
	task_content_load();

	//�R���\�[���o�͂̃}�N����`�ł��B
	//�\�[�X�R�[�h�̍ŏ��̕��Ł@#define console_debug�@�ƋL�q�����
	//#ifdef~#endif�̊Ԃ̓R���p�C�����ɑ}������Ď��s�t�@�C�����o���オ��܂��B
	#ifdef console_debug
		AllocConsole();
        freopen("CONOUT$", "w", stdout); 
        freopen("CONIN$", "r", stdin);
	#endif

	// �}�E�X��\����Ԃɂ���(TRUE�Ȃ�\���Afalse�Ȃ�\�����Ȃ�)
	//http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N5
	SetMouseDispFlag( TRUE ) ;

	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g(�K�{)(����ɕK�v)
	//[SetDrawScreen]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N6
	SetDrawScreen( DX_SCREEN_BACK ) ;

	//�Q�[���̓���X�V�̍ŏI���Ԃł��B
	//[timeGetTime()]http://msdn.microsoft.com/ja-jp/library/cc428795.aspx
	double lasttime = timeGetTime() ;
	//����X�V�̍ŏI���Ԃ����݂̎��Ԃɐݒ肷��B

	//�t���[���A�j���[�V�����̑�����񐔂̕ϐ��ł��B
	int loop_count=0;

	//-----------------------------------------
	//�t���[���A�j���[�V�����̖������[�v�ł��B
	//-----------------------------------------
	for(;;)
	{
		//�o�ߎ��Ԃɂ��A�A�b�v�f�[�g�����s����B
		//�t���[�����������̏ꍇ�̓A�b�v�f�[�g���J��Ԃ��Ăяo���A
		//���s�������Ƃɂ���B

		//���ݎ��Ԃ̎擾�p�ϐ��ł��B
		//[timeGetTime()]http://msdn.microsoft.com/ja-jp/library/cc428795.aspx
		double curtime=timeGetTime();
		//���ݎ��Ԃɐݒ�

		//���݌ɉ�ʍX�V���Ԃ���O��̉�ʍX�V���Ԃ��������l��
		//���[�v�^�C���Ƃ��܂��B
		double looptime=curtime-lasttime;

		//���[�v�^�C����Framerate�𒴂����ꍇ�A�X�V�������s���܂��B
		//���̂Ƃ��A����16.6666�t���[���ōX�V�̂Ƃ�
		//16�̂Ƃ����[�v�J�E���g��0��
		//18�̂Ƃ����[�v�J�E���g��1��
		//35�̂Ƃ��̓��[�v�J�E���g2�ɂȂ�悤
		//���삷��v���O�����ɂ������B
		if (looptime>Framerate)
		{
			loop_count=(int)(looptime/Framerate);	//���[�v�񐔂����߂�
			double amaritime=looptime-(loop_count*Framerate);	//���[�v�񐔂̗]�莞�Ԃ����߂�
			lasttime=curtime-amaritime;

			//�Q�[���̍X�V�����ł��B�t���[���������N�����Ă���ꍇ�A
			//��������s(������)���ăA�j���[�V�����t���[����i�܂��܂��B
			int l=0;	//�X�V�񐔂�0�̏ꍇ�͓���X�V�������s���܂���B
			while(l<loop_count)
			{
				task_update();	//����X�V�����Ăяo��
				l++;
			}
		}
		//�A�b�v�f�[�g�����I��--------------------------------------------

		//�`��X�V�����̌Ăяo��(�Q�[���̍X�V�������s�����Ƃ��ɕ`��X�V���s��)
		if (loop_count>0)
		{
			task_draw();
		}

		// Windows ���L�̖ʓ|�ȏ������c�w���C�u�����ɂ�点��
		// -1 ���Ԃ��Ă����烋�[�v�𔲂���
		if( ProcessMessage() < 0 ) break ;

		// �����d�r�b�L�[��������Ă����烋�[�v���甲����
		//[CheckHitKey]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N2
		if( CheckHitKey( KEY_INPUT_ESCAPE ) ) break ;

	}	//(for(;;)�@�t���[���p�������[�v�̒����ʂł�(�폜�֎~)
	//--------------------------------------------------------------------------------------





	//�R���\�[���o�͉���̃}�N����`�ł��B
	//�\�[�X�R�[�h�̍ŏ��̕��Ł@#define console_debug�@�ƋL�q�����
	//#ifdef~#endif�̊Ԃ̓R���p�C�����ɑ}������Ď��s�t�@�C�����o���オ��܂��B
	#ifdef console_debug
        //�R���\�[�����
        FreeConsole();
	#endif
	//----------------------------------------------------------------------------------------

	// �c�w���C�u�����g�p�̏I������(�K�{)
	//[DxLib_End]http://homepage2.nifty.com/natupaji/DxLib/dxfunc.html#R1N2
	DxLib_End() ;				
	//----------------------------------------------------------------------------------------

	// �\�t�g�̏I��
	return 0 ;
}




//�ŏ��̏����������̕���
//----------------------------------------------
//��ԍŏ��Ɏ��s���鏉�����̏����ł��B
//�ϐ��̏�������f�[�^�̃��[�h�Ȃǂ��s���܂��B
//----------------------------------------------
void task_init()
{






}

//�ŏ��̃O���t�B�b�N�A�T�E���h�̓ǂݍ��ݕ���
//------------------------------------------------------
//DX���C�u�����̏��������ł��Ă��Ȃ��ƁA�O���t�B�b�N
//�T�E���h�Ȃǂ̃f�[�^��ǂݍ��ނ��Ƃ��ł��܂���B
//------------------------------------------------------
void task_content_load()
{





}

//����X�V�̕���
//-----------------------------------------
//���R�ɃQ�[���̓��쏈���������Ƃ���ł��B
//�ݒ肵���l�ł̓���X�V���s���܂��B
//-----------------------------------------
void task_update()
{
	//---------------------------------------
	//�Q�[���̓��쏈���������ɏ����܂�
	//---------------------------------------



















	// �}�E�X�̈ʒu���擾(�g��Ȃ��ꍇ�͕s�v)
	//[GetMousePoint]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N6
	GetMousePoint( &mouseX , &mouseY ) ;
	//----------------------------------------------------------------------------------------

	//�}�E�X�̃N���b�N����(�g��Ȃ��ꍇ�͕s�v)
	//[GetMouseInput]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N8
	if( ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 )
	{
		mouseCnt++;
		if (mouseCnt>24) mouseCnt=24;
	}else mouseCnt=0;
	//----------------------------------------------------------------------------------------

	//�t���[���J�E���g�A�b�v(�g��Ȃ��ꍇ�͕s�v)
	frame_count++;
	//----------------------------------------------------------------------------------------

}

//�`��X�V�̕���
//-----------------------------------------------
//�`��̏����������Ƃ���ł��B
//�t���[�������Ȃǂ̗v���ŕs����Ŏ��s�����̂�
//�����ł͓���̋L�q���s���Ă͂����܂���
//-----------------------------------------------
void task_draw()
{
	// ��ʂ�������(�^�����ɂ���)(�g��Ȃ��ꍇ�͕s�v)
	//[ClearDrawScreen()]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N4
	ClearDrawScreen() ;
	//----------------------------------------------------------------------------------------

	//---------------------------------------
	//�Q�[���̕`�揈���������ɏ����܂�
	//---------------------------------------










	//�����F�𔒂�(�g��Ȃ��ꍇ�͕s�v)
	//[GetColor]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N5
	int Cr = GetColor( 255 , 255 , 255 ) ;
	//----------------------------------------------------------------------------------------

	//�O���t�B�b�N�̕\��(�g��Ȃ��ꍇ�͕s�v)
	//[DrawFormatString]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph2.html#R17N20
	DrawFormatString(400, 10, Cr, "MOUSE  X=%d Y=%d Cnt=%d \n", mouseX,mouseY,mouseCnt) ;
	//----------------------------------------------------------------------------------------

	//�t���[���J�E���g�\��(�g��Ȃ��ꍇ�͕s�v)
	DrawFormatString(100, 10, Cr, "frame count=%d",frame_count/60);
	//----------------------------------------------------------------------------------------

	// ����ʂ̓��e��\��ʂɃR�s�[����(�K�{)
	//�_�u���o�b�t�@�̃t���b�v(�X�V)�����̂��߁A���̌�ɕ`�揈���������Ă͂����܂���
	//[ScreenFlip()]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N7
	ScreenFlip() ;
	//----------------------------------------------------------------------------------------
}



//---------------------------------------------------------
//�Q�[���̓���p�@�֐�����������L�q���܂�
//---------------------------------------------------------

