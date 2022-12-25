//==========================================================
//
//�t�F�[�h����
//Author ����ɐ�
//
//==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//�t�F�[�h��ԗ񋓌^
typedef enum
{
	FADE_NONE = 0,	//�������Ă��Ȃ����
	FADE_IN,		//�t�F�[�h�C�����
	FADE_OUT,		//�t�F�[�h�A�E�g���
	FADE_MAX
}FADE;

//�v���g�^�C�v�錾
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif
