//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM�^�C�g��
	SOUND_LABEL_BGM001,			// BGM�`���[�g���A��
	SOUND_LABEL_BGM002,			// BGM�Q�[��
	SOUND_LABEL_BGM003,			// BGM���U���g
	SOUND_LABEL_SE_ENTER,		// �^�C�g�����艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
