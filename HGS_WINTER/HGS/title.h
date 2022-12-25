//==========================================================
//
//タイトル画面処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//列挙型
typedef enum
{
	TITLESTATE_NONE = 0,	//何もない状態
	TITLESTATE_NORMAL,	//通常状態
	TITLESTATE_END,		//終了状態
	TITLESTATE_MAX
}TITLESTATE;

//プロトタイプ宣言
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