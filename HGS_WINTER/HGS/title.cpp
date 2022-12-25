//==========================================================
//
//タイトル描画処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include <stdlib.h>
#include "fade.h"
#include "sound.h"

//===================================================
//マクロ定義
//===================================================
#define NUM_TITLE		(3)		//タイトルの種類
#define TEXTUREBG_DATA		"data\\TEXTURE\\bg00.png"		//テクスチャデータ(タイトル背景)
#define TEXTURETITLE_DATA	"data\\TEXTURE\\title_logo.png"		//テクスチャデータ(ゲームタイトル)
#define TEXTUREENTER_DATA	"data\\TEXTURE\\title_enter.png"	//テクスチャデータ(タイトルEnter)
#define TITLE_X			(640.0f)								//タイトルX座標
#define TITLE_Y			(240.0f)								//タイトルY座標
#define SET_TITLE_X		(-200.0f)								//タイトルX座標(初期位置)
#define SET_TITLE_Y		(-200.0f)								//タイトルY座標(初期位置)
#define TITLE_WIDTH		(480.0f)								//タイトル幅
#define TITLE_HEIGHT	(180.0f)								//タイトル高さ
#define ENTER_X			(640.0f)								//EnterX座標
#define ENTER_Y			(550.0f)								//EnterY座標
#define ENTER_WIDTH		(280.0f)								//Enter幅
#define ENTER_HEIGHT	(60.0f)									//Enter高さ
#define TITLE_TIMER		(20)									//タイトルから移動するまでの時間
#define ENTER_TIMER		(3)										//Enterチカチカタイマー
#define SCREEN_RESULT	(600)									//リザルトとタイトルの自動タイマー

//===================================================
//列挙型
//===================================================
typedef enum
{
	TITLE_BG = 0,
	TITLE_TITLE,
	TITLE_ENTER,
	TITLE_BALLOON,
	TITLE_MAX
}TITLE;

//===================================================
//グローバル変数宣言
//===================================================
TITLESTATE g_TitleState = TITLESTATE_NONE;				//タイトルの状態
int g_nCounterTitleState;								//状態管理カウンター
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TITLE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_Titlepos[NUM_TITLE];						//タイトル分の座標
D3DXCOLOR g_Titlecol[NUM_TITLE];						//タイトル分のカラー
float g_TexU;											//テクスチャ座標の開始位置(U値)
float g_fCol;											//Enter透明度
int g_nTitleTimer;										//自動画面移動タイマー

//===================================================
//タイトルの初期化処理
//===================================================
void InitTitle(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREBG_DATA, &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURETITLE_DATA, &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREENTER_DATA, &g_pTextureTitle[2]);

	g_TitleState = TITLESTATE_NORMAL;	//通常状態にセット
	g_nCounterTitleState = 0;			//カウンターを初期化
	g_TexU = 0.0f;						//背景移動量を初期化
	g_nTitleTimer = 0;					//自動移動タイマー初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_BG:
			//背景初期化
			InitBgTitle(nCntTitle);
			break;
		case TITLE_TITLE:
			//ゲームタイトル初期化
			InitGameTitle(nCntTitle);
			break;
		case TITLE_ENTER:
			//Enter初期化
			InitEnterTitle(nCntTitle);
			break;
		}
	}

	//bgmの再生
	PlaySound(SOUND_LABEL_BGM000);
}

//===================================================
//背景初期化処理
//===================================================
void InitBgTitle(int nCntTitle)
{

	g_Titlepos[nCntTitle] = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(g_TexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_TexU + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_TexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_TexU + 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//ゲームタイトル初期化処理
//===================================================
void InitGameTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ TITLE_X, SET_TITLE_Y, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//Enter初期化処理
//===================================================
void InitEnterTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ ENTER_X, ENTER_Y, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	VERTEX_2D *pVtx;
	g_fCol = 0.01f;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//タイトルの終了処理
//===================================================
void UninitTitle(void)
{
	//bgmの停止
	StopSound();

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//テクスチャの廃棄
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//===================================================
//タイトルの更新処理
//===================================================
void UpdateTitle(void)
{
	//状態確認
	switch (g_TitleState)
	{
	case TITLESTATE_NONE:	//何もしていない状態
		break;
	case TITLESTATE_NORMAL:	//通常状態

		break;
	case TITLESTATE_END:	//終了状態
		g_nCounterTitleState--;

		if (g_nCounterTitleState <= 0)
		{//カウンターが0になったとき
			g_TitleState = TITLESTATE_NONE;	//何もしていない状態に設定
			SetFade(MODE_TUTORIAL);
		}
		break;
	}
	if (g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetKeyboardTrigger(DIK_RETURN) == true
		|| g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetJoyPadButtonTrigger(KEY_START, 0) == true
		|| g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetJoyPadButtonTrigger(KEY_A, 0) == true)
	{//右キーが入力されたとき
		if (g_TitleState == TITLESTATE_NORMAL)
		{
			SetTitleState(TITLESTATE_END, TITLE_TIMER);
		}
	}

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_BG:
			//背景更新
			UpdateBgTitle(nCntTitle);
			break;
		case TITLE_TITLE:
			//ゲームタイトル更新
			UpdateGameTitle(nCntTitle);
			break;
		case TITLE_ENTER:
			//Enter更新
			UpdateEnterTitle(nCntTitle);
			break;
		}
	}
}

//===================================================
//背景更新
//===================================================
void UpdateBgTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	g_TexU += 0.0003f;
	if (g_TexU >= 1.0f)
	{
		g_TexU = 0.0f;
	}
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(g_TexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_TexU + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_TexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_TexU + 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//タイトル更新
//===================================================
void UpdateGameTitle(int nCntTitle)
{
	if (g_Titlepos[nCntTitle].y <= TITLE_Y)
	{
		g_Titlepos[nCntTitle].y += 2.0f;
	}
	if (g_Titlecol[nCntTitle].a < 1.0f)
	{
		g_Titlecol[nCntTitle].a += 0.005f;
	}

	if (g_Titlepos[nCntTitle].y < TITLE_Y && GetKeyboardTrigger(DIK_RETURN) == true 
		|| g_Titlepos[nCntTitle].y < TITLE_Y && GetJoyPadButtonTrigger(KEY_START,0) == true 
		|| g_Titlepos[nCntTitle].y < TITLE_Y && GetJoyPadButtonTrigger(KEY_A, 0) == true)
	{
		g_Titlepos[nCntTitle].y = TITLE_Y;
		g_Titlecol[nCntTitle].a = 1.0f;
	}



	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//Enter更新
//===================================================
void UpdateEnterTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ ENTER_X, ENTER_Y, 0.0f };

	VERTEX_2D *pVtx;

	if (g_TitleState == TITLESTATE_NORMAL)
	{//終了状態ではない場合
		if (g_Titlepos[TITLE_TITLE].y >= TITLE_Y)
		{//ゲームタイトルが定位置についた後
			g_Titlecol[nCntTitle].a += g_fCol;

			if (g_Titlecol[nCntTitle].a >= 1.0f || g_Titlecol[nCntTitle].a <= 0.0f)
			{
				g_fCol *= -1.0f;
			}
		}
	}
	else if (g_TitleState != TITLESTATE_NORMAL)
	{//終了状態の場合
		static bool bEndUse = false;	//初回透明度変更
		static float fEndCol = 0.1f;	//終了設定後のカラー変更
		static int nCounter = 0;		//カウンター
		nCounter++;

		if (nCounter >= ENTER_TIMER)
		{//カウンターがたまったとき
			nCounter = 0;
			switch (bEndUse)
			{
			case true:	//二度以降
				g_Titlecol[nCntTitle].a = 0.0f;	//半透明分プラス
				bEndUse = false;	//二度目以降に変更
				break;
			case false:	//初回
				bEndUse = true;	//二度目以降に変更
				g_Titlecol[nCntTitle].a = 1.0f;	//初期カラーを完全不透明に設定
				break;
			}

			//g_Titlecol[nCntTitle].a = fEndCol;	//半透明分プラス
			fEndCol *= -1.0f;	//+-を逆にする

		}

	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//タイトルの描画処理
//===================================================
void DrawTitle(void)
{

	PDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
		//タイトルの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntTitle * 4,
			2	//頂点情報構造体のサイズ
		);

	}
}

//===================================================
//ゲーム状態設定
//===================================================
void SetTitleState(TITLESTATE state, int nCounter)
{
	if (g_TitleState != state)
	{
		g_TitleState = state;
		g_nCounterTitleState = nCounter;
	}
}

//===================================================
//ゲーム状態取得
//===================================================
TITLESTATE GetTitleState(void)
{
	//aaaa
	return g_TitleState;
}
