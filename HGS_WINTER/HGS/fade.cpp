//==========================================================
//
//フェード処理
//Author 奥定伊吹
//
//==========================================================
#include "fade.h"

//グローバル宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//頂点バッファへのポインタ
FADE g_fade;									//フェードの状態
MODE g_modeNext;								//次の画面(モード)
D3DXCOLOR g_fadeCol;							//フェードカラー

//フェードの初期化処理
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	g_fade = FADE_IN;	//フェードイン状態に
	g_modeNext = modeNext;	//次の画面を設定
	g_fadeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴンにしておく

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
		);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[1].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[2].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[3].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[0].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//フェードの終了処理
void UninitFade(void)
{
	//頂点バッファの廃棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//フェードの更新処理
void UpdateFade(void)
{

	if (g_fade != FADE_NONE)
	{//何もしていない状態以外
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_fadeCol.a -= 1.0f / 30;	//透明に近づけていく

			if (g_fadeCol.a <= 0.0f)
			{//完全に透明になった場合
				g_fadeCol.a = 0.0f;	//透明度を透明に
				g_fade = FADE_NONE;	//何もしていない状態に
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態

			g_fadeCol.a += 1.0f / 30;	//不透明に近づけていく

			if (g_fadeCol.a >= 1.0f)
			{//完全に不透明になった場合
				g_fadeCol.a = 1.0f;	//透明度を不透明に

				//モードの設定(次の画面に移行)
				SetMode(g_modeNext);

				g_fade = FADE_IN;	//何もしていない状態に
				
				
			}
		}
	}

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	//頂点カラーの設定
	pVtx[1].col = g_fadeCol;
	pVtx[2].col = g_fadeCol;
	pVtx[3].col = g_fadeCol;
	pVtx[0].col = g_fadeCol;

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//フェードの描画処理
void DrawFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしていない状態以外の場合
		PDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffFade,
			0,
			sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//フェードの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,
			2	//頂点情報構造体のサイズ
		);
	}
}

//フェードの透明度取得
FADE GetFade(void)
{
	//透明度を返す
	return g_fade;
}

//フェードの状態設定
void SetFade(MODE modeNext)
{
	if (g_fade != FADE_OUT)
	{
		g_fade = FADE_OUT;	//フェードアウト状態に
		g_modeNext = modeNext;	//次の画面(モード)に
		g_fadeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//透明に設定
	}
}
