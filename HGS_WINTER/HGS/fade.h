//==========================================================
//
//フェード処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//フェード状態列挙型
typedef enum
{
	FADE_NONE = 0,	//何もしていない状態
	FADE_IN,		//フェードイン状態
	FADE_OUT,		//フェードアウト状態
	FADE_MAX
}FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif
