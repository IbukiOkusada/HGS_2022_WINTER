//==========================================================
//
//カメラ処理
//Author 奥定伊吹
//
//==========================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"

//==========================================================
//マクロ定義
//==========================================================
#define CAMERA_MOVESPEED	(1.0f)			//移動量
#define CAMERA_LENGTHMOVE	(1.0f)			//カメラ距離移動量
#define ROTATE_SPEED	(0.008f)			//カメラの回転速度
#define CAMERA_MAXLENGTH	(600.0f)		//カメラ最大距離
#define CAMERA_MINLENGTH	(100.0f)		//カメラ最小距離
#define MAX_CAMERA_ROTZ (D3DX_PI * 0.9f)	//カメラ最大角度
#define MIN_CAMERA_ROTZ (D3DX_PI * 0.1f)	//カメラ最大角度
#define MOUSE_MOVESPEED		(0.1f)			//マウス移動速度
#define MOUSE_ROTATESPEED_X	(0.005f)		//マウス回転速度x軸
#define MOUSE_ROTATESPEED_Z	(0.005f)		//マウス回転速度z軸
#define MOUSE_WHEELSPEED	(0.1f)			//マウスホイール回転速度
#define CAMERA_DEFAULTROT	(-D3DX_PI * 0.3f)	//デフォルト斜め角度
#define CAMERA_PLAYERLENGTH	(145.0f)		//注視点のプレイヤーからの距離
#define PLUSROT_SPEED		(0.1f)			//旋回加算回転速度
#define TITLE_ROTATESPEED	(0.001f)		//タイトル旋回速度
#define TITLE_LENGTH		(3000.0f)		//タイトルカメラ長さ

//==========================================================
//プロトタイプ宣言
//==========================================================
void TitleRotateCamera(void);


//==========================================================
//グローバル変数
//==========================================================
Camera g_camera;	//カメラの情報
int g_nStoker = 1;	//追従のオンオフ
int g_nAuto = 1;	//旋回のオンオフ

//==========================================================
//カメラの初期化処理
//==========================================================
void InitCamera(void)
{
	switch (GetMode())
	{
	case MODE_TITLE:
		g_camera.posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		g_camera.fLength = TITLE_LENGTH;
		g_camera.rot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	case MODE_TUTORIAL:
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.posV = D3DXVECTOR3(-300.0f, 150.0f, 0.0f);
		g_camera.fLength = sqrtf((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z));
		g_camera.rot = D3DXVECTOR3(0.0f, atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 1.29f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.fPlusRot = CAMERA_DEFAULTROT;
		break;
	case MODE_GAME:
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.posV = D3DXVECTOR3(-300.0f, 150.0f, 0.0f);
		g_camera.fLength = sqrtf((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z));
		g_camera.rot = D3DXVECTOR3(0.0f, atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 1.29f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.fPlusRot = CAMERA_DEFAULTROT;
		break;
	case MODE_RESULT:
		break;
	}

	//視点設定
	SetCameraV();
}

//==========================================================
//カメラの終了処理
//==========================================================
void UninitCamera(void)
{

}

//==========================================================
//カメラの更新処理
//==========================================================
void UpdateCamera(void)
{

	switch (GetMode())
	{
	case MODE_TITLE:

		//タイトルカメラ旋回
		TitleRotateCamera();
		break;
	case MODE_TUTORIAL:
		if (GetTutorialEdit() == false)
		{
			SetRotCamera();
			RotateCamera();
		}
		else
		{

			MouseCamera();
		}

		break;
	case MODE_GAME:
		if (GetEdit() == false)
		{
			
		}
		break;
	case MODE_RESULT:
		break;
	}
}

//==========================================================
//カメラの設定処理
//==========================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	//プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの生成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float) SCREEN_HEIGHT,
								10.0f,
								8000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==========================================================
//カメラの情報取得
//==========================================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//==========================================================
//カメラの全移動処理
//==========================================================
void MoveCameraVR(void)
{
	Camera *pCamera = GetCamera();

	//視点注視点の同時移動
	if (GetKeyboardPress(DIK_A) == true)
	{//Aキー入力

		//視点の移動
		if (GetKeyboardPress(DIK_W) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキー入力

		//視点の移動
		if (GetKeyboardPress(DIK_W) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}

	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//Wキー入力

		//視点の移動
		pCamera->move.x += -cosf(pCamera->rot.y) * CAMERA_MOVESPEED;
		pCamera->move.z += -sinf(pCamera->rot.y) * CAMERA_MOVESPEED;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキー入力
		//視点の移動
		pCamera->move.x += cosf(pCamera->rot.y) * CAMERA_MOVESPEED;
		pCamera->move.z += sinf(pCamera->rot.y) * CAMERA_MOVESPEED;

	}

	//移動量を加算
	pCamera->posV.x += pCamera->move.x;
	pCamera->posV.z += pCamera->move.z;

	//移動量を初期化
	pCamera->move.x = 0.0f;
	pCamera->move.z = 0.0f;

	//注視点設定
	SetCameraR();
}

//==========================================================
//カメラの視点移動処理
//==========================================================
void MoveCameraV(void)
{
	Camera *pCamera = GetCamera();

	//x軸の移動
	if (GetKeyboardPress(DIK_Z) == true && GetKeyboardPress(DIK_C) != true)
	{//Qキー入力
		pCamera->rot.y += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_C) == true && GetKeyboardPress(DIK_Z) != true)
	{//Eキー入力
		pCamera->rot.y += D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}

	//z軸の移動
	if (GetKeyboardPress(DIK_Y) == true && GetKeyboardPress(DIK_N) != true)
	{//Yキー入力
		//角度の変更
		pCamera->rot.z += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		
	}
	else if (GetKeyboardPress(DIK_N) == true && GetKeyboardPress(DIK_Y) != true)
	{//Nキー入力
		//角度の変更
		pCamera->rot.z += D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}

	}

	//注視点からの距離の操作
	if (GetKeyboardPress(DIK_U) == true && GetKeyboardPress(DIK_M) != true)
	{//Uキー入力
		pCamera->fLength -= CAMERA_LENGTHMOVE;

		if (pCamera->fLength < CAMERA_MINLENGTH)
		{//距離が最小を超えた場合
			pCamera->fLength = CAMERA_MINLENGTH;
		}
		
	}
	else if (GetKeyboardPress(DIK_M) == true && GetKeyboardPress(DIK_U) != true)
	{//Mキー入力
		pCamera->fLength += CAMERA_LENGTHMOVE;

		if (pCamera->fLength > CAMERA_MAXLENGTH)
		{//距離が最大を超えた場合
			pCamera->fLength = CAMERA_MAXLENGTH;
		}
	}

	//視点設定
	SetCameraV();
}

//==========================================================
//カメラの注視点移動処理
//==========================================================
void MoveCameraR(void)
{
	Camera *pCamera = GetCamera();

	//x軸の移動
	if (GetKeyboardPress(DIK_Q) == true && GetKeyboardPress(DIK_E) != true)
	{//Qキー入力

		//角度の変更
		pCamera->rot.y += D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_E) == true && GetKeyboardPress(DIK_Q) != true)
	{//Eキー入力

		//角度の変更
		pCamera->rot.y += -D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}

	//z軸の移動
	if (GetKeyboardPress(DIK_T) == true && GetKeyboardPress(DIK_B) != true)
	{//Tキー入力

		//角度の変更
		pCamera->rot.z += D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.z >  MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}
	}
	else if (GetKeyboardPress(DIK_B) == true && GetKeyboardPress(DIK_T) != true)
	{//Bキー入力
		//角度の変更
		pCamera->rot.z += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
	}

	//注視点設定
	SetCameraR();
}

//==========================================================
//カメラ視点設定処理
//==========================================================
void SetCameraV(void)
{
	Camera *pCamera = GetCamera();

	pCamera->posV.y = pCamera->posR.y + cosf(pCamera->rot.z) * pCamera->fLength;
	pCamera->posV.x = pCamera->posR.x + (sinf(pCamera->rot.z) * cosf(pCamera->rot.y + g_camera.fPlusRot)) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + (sinf(pCamera->rot.z) * sinf(pCamera->rot.y + g_camera.fPlusRot)) * pCamera->fLength;
}

//==========================================================
//カメラ注視点設定処理
//==========================================================
void SetCameraR(void)
{
	Camera *pCamera = GetCamera();

	pCamera->posR.y = pCamera->posV.y - cosf(pCamera->rot.z) * pCamera->fLength;
	pCamera->posR.x = pCamera->posV.x - (sinf(pCamera->rot.z) * cosf(pCamera->rot.y + g_camera.fPlusRot)) * pCamera->fLength;
	pCamera->posR.z = pCamera->posV.z - (sinf(pCamera->rot.z) * sinf(pCamera->rot.y + g_camera.fPlusRot)) * pCamera->fLength;
}

//==========================================================
//マウスでのカメラ移動処理
//==========================================================
void MouseCamera(void)
{
	Camera *pCamera = GetCamera();

	if (GetMousePress(MOUSE_LBUTTON) == true && GetMousePress(MOUSE_RBUTTON) == true)
	{//左右同時に押されているとき
		pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * (GetMouseCousorMove().x * MOUSE_MOVESPEED) + -cosf(pCamera->rot.y) * (GetMouseCousorMove().y * MOUSE_MOVESPEED);
		pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * (GetMouseCousorMove().x * MOUSE_MOVESPEED) + -sinf(pCamera->rot.y) * (GetMouseCousorMove().y * MOUSE_MOVESPEED);

		//移動量を加算
		pCamera->posV.x += pCamera->move.x;
		pCamera->posV.z += pCamera->move.z;

		//移動量を初期化
		pCamera->move.x = 0.0f;
		pCamera->move.z = 0.0f;

		//注視点設定
		SetCameraR();
	}
	else if (GetMousePress(MOUSE_LBUTTON) == true)
	{//左キーが押されているとき
		pCamera->rot.y -= GetMouseCousorMove().x * MOUSE_ROTATESPEED_X;
		pCamera->rot.z -= GetMouseCousorMove().y * 0.005f;

		if (pCamera->rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		else if (pCamera->rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		else if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}
	}
	else if (GetMousePress(MOUSE_RBUTTON) == true)
	{//右キーが押されているとき
		pCamera->rot.y -= GetMouseCousorMove().x * MOUSE_ROTATESPEED_X;
		pCamera->rot.z -= GetMouseCousorMove().y * MOUSE_ROTATESPEED_Z;

		if (pCamera->rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		else if (pCamera->rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		else if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}

		//視点設定
		SetCameraR();
	}

	//ホイールの使用量で距離の変更
	pCamera->fLength += GetMouseCousorMove().z * MOUSE_WHEELSPEED;

	if (pCamera->fLength > CAMERA_MAXLENGTH)
	{//距離が最大を超えた場合
		pCamera->fLength = CAMERA_MAXLENGTH;
	}
	else if (pCamera->fLength < CAMERA_MINLENGTH)
	{//距離が最小を超えた場合
		pCamera->fLength = CAMERA_MINLENGTH;
	}

	//視点設定
	SetCameraV();
}

//==========================================================
//カメラの追従設定
//==========================================================
void CameraMove(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報を取得
	Camera *pCamera = GetCamera();	//カメラの情報を取得

	//目的の注視点の座標を取得
	pCamera->posRDest = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH, pPlayer->pos.y + 40.0f, pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH);

	//目的の視点の座標を獲得
	pCamera->posVDest = D3DXVECTOR3(
		pCamera->posRDest.x + (sinf(pCamera->rot.z) * cosf(pCamera->rot.y)) * pCamera->fLength,
		pCamera->posRDest.y + cosf(pCamera->rot.z) * pCamera->fLength,
		pCamera->posRDest.z + (sinf(pCamera->rot.z) * sinf(pCamera->rot.y)) * pCamera->fLength);

	//注視点の補正
	D3DXVECTOR3 RDiff = D3DXVECTOR3(pCamera->posRDest.x - pCamera->posR.x, pCamera->posRDest.y - pCamera->posR.y, pCamera->posRDest.z - pCamera->posR.z);
	pCamera->posR += RDiff * 0.2f;

	//視点の補正
	D3DXVECTOR3 VDiff = D3DXVECTOR3(pCamera->posVDest.x - pCamera->posV.x, pCamera->posVDest.y - pCamera->posV.y, pCamera->posVDest.z - pCamera->posV.z);
	pCamera->posV += VDiff * 0.15f;
}

//==========================================================
//カメラの向きの設定
//==========================================================
void SetRotCamera(void)
{
	if (g_nAuto == 0)
	{
		return;
	}

	Player *pPlayer = GetPlayer();	//プレイヤーの情報を取得
	Camera *pCamera = GetCamera();	//カメラの情報を取得
	float fRotDiff;

	pCamera->rotDest.x = -pPlayer->rot.y + (-D3DX_PI * 1.0f);	//目的の向きを取得

	fRotDiff = pCamera->rotDest.x - pCamera->rot.y;

	if (fRotDiff > D3DX_PI || fRotDiff < -D3DX_PI)
	{//-3.14〜3.14の範囲外の場合
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff += (-D3DX_PI * 2);
		}
		else if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += (D3DX_PI * 2);
		}
	}

	pCamera->rot.y += fRotDiff * 0.08f;

	if (pCamera->rot.y > D3DX_PI || pCamera->rot.y < -D3DX_PI)
	{//-3.14〜3.14の範囲外の場合
		if (pCamera->rot.y > D3DX_PI)
		{
			pCamera->rot.y += (-D3DX_PI * 2);
		}
		else if (pCamera->rot.y  < -D3DX_PI)
		{
			pCamera->rot.y += (D3DX_PI * 2);
		}
	}

	//視点の設定
	SetCameraV();

}

//==========================================================
//カメラの自動旋回
//==========================================================
void RotateCamera(void)
{
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();

	float fDest, fDiff;

	if (GetMode() == MODE_GAME)
	{
		switch (pPlayer->nMoveRot)
		{
		case PLAYERMOVE_RIGHT:
			fDest = CAMERA_DEFAULTROT;
			break;
		case PLAYERMOVE_LEFT:
			fDest = -CAMERA_DEFAULTROT;
			break;
		}

		fDiff = fDest - pCamera->fPlusRot;

		pCamera->fPlusRot += fDiff * PLUSROT_SPEED;

		if (pCamera->fPlusRot > D3DX_PI || pCamera->fPlusRot < -D3DX_PI)
		{//-3.14〜3.14の範囲外の場合
			if (pCamera->fPlusRot > D3DX_PI)
			{
				pCamera->fPlusRot += (-D3DX_PI * 2);
			}
			else if (pCamera->fPlusRot  < -D3DX_PI)
			{
				pCamera->fPlusRot += (D3DX_PI * 2);
			}
		}

		//視点設定
		SetCameraV();
	}
	else
	{
		//fDest = pPlayer->rot.y;

		//fDiff = fDest - pCamera->rot.y;

		//pCamera->rot.y += fDiff * PLUSROT_SPEED;

		//if (pCamera->rot.y  > D3DX_PI || pCamera->rot.y  < -D3DX_PI)
		//{//-3.14〜3.14の範囲外の場合
		//	if (pCamera->rot.y > D3DX_PI)
		//	{
		//		pCamera->rot.y += (-D3DX_PI * 2);
		//	}
		//	else if (pCamera->rot.y   < -D3DX_PI)
		//	{
		//		pCamera->rot.y += (D3DX_PI * 2);
		//	}
		//}

		////視点設定
		//SetCameraV();
	}

	
}

//==========================================================
//カメラ旋回
//==========================================================
void TitleRotateCamera(void)
{
	Camera *pCamera = GetCamera();	//カメラの情報を取得

	pCamera->rot.y += -D3DX_PI * TITLE_ROTATESPEED;
	if (pCamera->rot.y < -D3DX_PI)
	{//角度がΠを超えた場合
		pCamera->rot.y += D3DX_PI * 2;
		
	}

	//視点設定
	SetCameraV();

}

//==========================================================
//カメラ移動
//==========================================================
void SetMoveCamera(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報を取得
	Camera *pCamera = GetCamera();	//カメラの情報を取得

	//目的の注視点の座標を取得
	pCamera->posRDest = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH, pPlayer->pos.y + 40.0f, pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH);

	//目的の視点の座標を獲得
	pCamera->posVDest = D3DXVECTOR3(
		pCamera->posRDest.x + (sinf(pCamera->rot.z) * cosf(pCamera->rot.y)) * pCamera->fLength,
		pCamera->posRDest.y + cosf(pCamera->rot.z) * pCamera->fLength,
		pCamera->posRDest.z + (sinf(pCamera->rot.z) * sinf(pCamera->rot.y)) * pCamera->fLength);

	//注視点の補正
	D3DXVECTOR3 RDiff = D3DXVECTOR3(pCamera->posRDest.x - pCamera->posR.x, pCamera->posRDest.y - pCamera->posR.y, pCamera->posRDest.z - pCamera->posR.z);
	pCamera->posR += RDiff;

	//視点の補正
	D3DXVECTOR3 VDiff = D3DXVECTOR3(pCamera->posVDest.x - pCamera->posV.x, pCamera->posVDest.y - pCamera->posV.y, pCamera->posVDest.z - pCamera->posV.z);
	pCamera->posV += VDiff;
}

//==========================================================
//注視点自動移動
//==========================================================
void SetMoveCameraR(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報を取得
	Camera *pCamera = GetCamera();	//カメラの情報を取得

	//目的の注視点の座標を取得
	pCamera->posRDest = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH, pPlayer->pos.y + 40.0f, pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_PLAYERLENGTH);

	//注視点の補正
	D3DXVECTOR3 RDiff = D3DXVECTOR3(pCamera->posRDest.x - pCamera->posR.x, pCamera->posRDest.y - pCamera->posR.y, pCamera->posRDest.z - pCamera->posR.z);
	pCamera->posR += RDiff * 0.2f;
}
