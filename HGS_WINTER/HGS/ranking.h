//===================================================
//
//�����L���O����
//Author ����ɐ�
//
//===================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include


//===================================================
//�񋓌^���`
//===================================================
//�����L���O�̏��
typedef enum
{
	RANKINGSTATE_NONE = 0,	//�����Ȃ����
	RANKINGSTATE_NORMAL,	//�ʏ���
	RANKINGSTATE_END,		//�I�����
	RANKINGSTATE_MAX
}RANKINGSTATE;

//�v���g�^�C�v�錾
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

#endif
