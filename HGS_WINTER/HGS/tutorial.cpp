//===================================================
//
//�`���[�g���A����ʏ���
//Author ����ɐ�
//
//===================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//===================================================
//�`���[�g���A����ʏ���������
//===================================================
void InitTutorial(void)
{
	
}

//===================================================
//�`���[�g���A����ʏI������
//===================================================
void UninitTutorial(void)
{
	
}

//===================================================
//�`���[�g���A����ʍX�V����
//===================================================
void UpdateTutorial(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) == true 
		|| GetJoyPadButtonTrigger(KEY_START, 0) == true 
		|| GetJoyPadButtonTrigger(KEY_A, 0) == true)
	{
		SetFade(MODE_GAME);
	}

}

//===================================================
//�`���[�g���A����ʕ`�揈��
//===================================================
void DrawTutorial(void)
{
	
}