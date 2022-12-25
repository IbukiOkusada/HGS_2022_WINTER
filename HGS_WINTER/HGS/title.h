//==========================================================
//
//�^�C�g����ʏ���
//Author ����ɐ�
//
//==========================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//�񋓌^
typedef enum
{
	TITLESTATE_NONE = 0,	//�����Ȃ����
	TITLESTATE_NORMAL,	//�ʏ���
	TITLESTATE_END,		//�I�����
	TITLESTATE_MAX
}TITLESTATE;

//�v���g�^�C�v�錾
void InitTitle(void);

void InitBgTitle(int nCntTitle);
void InitGameTitle(int nCntTitle);
void InitEnterTitle(int nCntTitle);
void InitTitleBg3D(void);

void UninitTitle(void);
void UninitTitleBg3D(void);
void UpdateTitle(void);

void UpdateBgTitle(int nCntTitle);
void UpdateGameTitle(int nCntTitle);
void UpdateEnterTitle(int nCntTitle);

void DrawTitle(void);

void SetTitleState(TITLESTATE state, int nCounter);
TITLESTATE GetTitleState(void);

#endif