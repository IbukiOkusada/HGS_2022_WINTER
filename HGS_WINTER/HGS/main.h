//==========================================================
//
//���C��
//Author ����ɐ�
//
//==========================================================
#ifndef _MAIN_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _MAIN_H_

//DirectX�̏����ݒ�
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x�����Ώ��p�}�N��
#include "dinput.h"
#include "xaudio2.h"					//�T�E���h�����ɕK�v

//���C�u�����̃����N
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")		//�V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH		(1280)			//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)			//�E�C���h�E�̍���
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g[3D]

//���_���(2D)�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//���_���(3D)�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_MAX
}MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
int GetFPS(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif