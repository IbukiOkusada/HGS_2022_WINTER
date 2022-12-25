//===================================================
//
//�Q�[����ʏ���
//Author ����ɐ�
//
//===================================================
#include "game.h"
#include "input.h"
#include <stdio.h>
#include "fade.h"
#include "sound.h"
#include "pause.h"
#include "debugproc.h"

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitGameMain(void);
void UpdateGameMain(void);
void DrawGameMain(void);

//===================================================
//�O���[�o���ϐ��錾
//===================================================
GAMESTATE g_gameState = GAMESTATE_NONE;		//�Q�[���̏��
int g_nCounterGameState;	//��ԊǗ��J�E���^�[
bool g_bPause = false;		//�|�[�Y��Ԃ�ON/OFF
bool g_bDebug = false;		//�f�o�b�O�\����ON/OFF

//===================================================
//�Q�[����ʂ̏���������
//===================================================
void InitGame(void)
{
	//�Q�[���v���C��ʂ̏���������
	InitGameMain();

	g_gameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		//�J�E���^�[��������
	g_bPause = false;				//�|�[�Y��ԏ�����

}

//===================================================
//�Q�[���v���C��ʂ̏���������
//===================================================
void InitGameMain(void)
{
	//�|�[�Y�̏���������
	InitPause();

	//bgm�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//===================================================
//�Q�[����ʂ̏I������
//===================================================
void UninitGame(void)
{
	//�|�[�Y�̏I������
	UninitPause();

	//Bgm�̏I��
	StopSound();
}

//===================================================
//�Q�[����ʂ̍X�V����
//===================================================
void UpdateGame(void)
{
	//��Ԃ��ƂɍX�V
	switch (g_gameState)
	{
	case GAMESTATE_NONE:	//�������Ă��Ȃ����
		break;
	case GAMESTATE_NORMAL:	//�ʏ���
		//�Q�[���v���C��ʂ̍X�V
		UpdateGameMain();
		break;
	case GAMESTATE_END:		//�I�����
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;	//�������Ă��Ȃ���Ԃɐݒ�
			//���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_RESULT);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//f1�������ꂽ�Ƃ�
		g_bDebug = g_bDebug ? false : true;
	}

}

//===================================================
//�Q�[���v���C��ʍX�V����
//===================================================
void UpdateGameMain(void)
{
	if (g_bPause == true)
	{//�|�[�Y��Ԃ̎�
		//�|�[�Y�̍X�V
		UpdatePause();
	}
	else
	{//����ȊO�̎�

	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadButtonTrigger(KEY_START, 0))
	{//�|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//�J�ڃL�[�������ꂽ
		SetFade(MODE_RESULT);
	}
	PrintDebugProc("�����L���O��ʂ֑J�� �y ENTER �z\n");
#endif

}	

//===================================================
//�Q�[����ʕ`�揈��
//===================================================
void DrawGame(void)
{
	
	//���C���`��
	DrawGameMain();

	if (g_bPause == true)
	{//�|�[�Y��Ԃ̎�
		//�|�[�Y�̕`��
		DrawPause();
	}
}

//===================================================
//�Q�[����Ԑݒ�
//===================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===================================================
//�Q�[����Ԏ擾
//===================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===================================================
//�|�[�Y��Ԑݒ�
//===================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause ? false: true;
}

//===================================================
//�ʏ�`��@
//===================================================
void DrawGameMain(void)
{
	
}
