//===================================================
//
//ランキング処理
//Author 奥定伊吹
//
//===================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude


//===================================================
//列挙型を定義
//===================================================
//ランキングの状態
typedef enum
{
	RANKINGSTATE_NONE = 0,	//何もない状態
	RANKINGSTATE_NORMAL,	//通常状態
	RANKINGSTATE_END,		//終了状態
	RANKINGSTATE_MAX
}RANKINGSTATE;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

#endif
