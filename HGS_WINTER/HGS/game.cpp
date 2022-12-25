//===================================================
//
//ゲーム画面処理
//Author 奥定伊吹
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
//プロトタイプ宣言
//===================================================
void InitGameMain(void);
void UpdateGameMain(void);
void DrawGameMain(void);

//===================================================
//グローバル変数宣言
//===================================================
GAMESTATE g_gameState = GAMESTATE_NONE;		//ゲームの状態
int g_nCounterGameState;	//状態管理カウンター
bool g_bPause = false;		//ポーズ状態のON/OFF
bool g_bDebug = false;		//デバッグ表示のON/OFF

//===================================================
//ゲーム画面の初期化処理
//===================================================
void InitGame(void)
{
	//ゲームプレイ画面の初期化処理
	InitGameMain();

	g_gameState = GAMESTATE_NORMAL;	//通常状態に設定
	g_nCounterGameState = 0;		//カウンターを初期化
	g_bPause = false;				//ポーズ状態初期化

}

//===================================================
//ゲームプレイ画面の初期化処理
//===================================================
void InitGameMain(void)
{
	//ポーズの初期化処理
	InitPause();

	//bgmの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//===================================================
//ゲーム画面の終了処理
//===================================================
void UninitGame(void)
{
	//ポーズの終了処理
	UninitPause();

	//Bgmの終了
	StopSound();
}

//===================================================
//ゲーム画面の更新処理
//===================================================
void UpdateGame(void)
{
	//状態ごとに更新
	switch (g_gameState)
	{
	case GAMESTATE_NONE:	//何もしていない状態
		break;
	case GAMESTATE_NORMAL:	//通常状態
		//ゲームプレイ画面の更新
		UpdateGameMain();
		break;
	case GAMESTATE_END:		//終了状態
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;	//何もしていない状態に設定
			//モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//f1が押されたとき
		g_bDebug = g_bDebug ? false : true;
	}

}

//===================================================
//ゲームプレイ画面更新処理
//===================================================
void UpdateGameMain(void)
{
	if (g_bPause == true)
	{//ポーズ状態の時
		//ポーズの更新
		UpdatePause();
	}
	else
	{//それ以外の時

	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadButtonTrigger(KEY_START, 0))
	{//ポーズキーが押された
		g_bPause = g_bPause ? false : true;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//遷移キーが押された
		SetFade(MODE_RESULT);
	}
	PrintDebugProc("ランキング画面へ遷移 【 ENTER 】\n");
#endif

}	

//===================================================
//ゲーム画面描画処理
//===================================================
void DrawGame(void)
{
	
	//メイン描画
	DrawGameMain();

	if (g_bPause == true)
	{//ポーズ状態の時
		//ポーズの描画
		DrawPause();
	}
}

//===================================================
//ゲーム状態設定
//===================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===================================================
//ゲーム状態取得
//===================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===================================================
//ポーズ状態設定
//===================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause ? false: true;
}

//===================================================
//通常描画　
//===================================================
void DrawGameMain(void)
{
	
}
