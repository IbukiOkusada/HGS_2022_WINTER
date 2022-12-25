//==========================================================
//
//�^�C�g���`�揈��
//Author ����ɐ�
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
//�}�N����`
//===================================================
#define NUM_TITLE		(3)		//�^�C�g���̎��
#define TEXTUREBG_DATA		"data\\TEXTURE\\bg00.png"		//�e�N�X�`���f�[�^(�^�C�g���w�i)
#define TEXTURETITLE_DATA	"data\\TEXTURE\\title_logo.png"		//�e�N�X�`���f�[�^(�Q�[���^�C�g��)
#define TEXTUREENTER_DATA	"data\\TEXTURE\\title_enter.png"	//�e�N�X�`���f�[�^(�^�C�g��Enter)
#define TITLE_X			(640.0f)								//�^�C�g��X���W
#define TITLE_Y			(240.0f)								//�^�C�g��Y���W
#define SET_TITLE_X		(-200.0f)								//�^�C�g��X���W(�����ʒu)
#define SET_TITLE_Y		(-200.0f)								//�^�C�g��Y���W(�����ʒu)
#define TITLE_WIDTH		(480.0f)								//�^�C�g����
#define TITLE_HEIGHT	(180.0f)								//�^�C�g������
#define ENTER_X			(640.0f)								//EnterX���W
#define ENTER_Y			(550.0f)								//EnterY���W
#define ENTER_WIDTH		(280.0f)								//Enter��
#define ENTER_HEIGHT	(60.0f)									//Enter����
#define TITLE_TIMER		(20)									//�^�C�g������ړ�����܂ł̎���
#define ENTER_TIMER		(3)										//Enter�`�J�`�J�^�C�}�[
#define SCREEN_RESULT	(600)									//���U���g�ƃ^�C�g���̎����^�C�}�[

//===================================================
//�񋓌^
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
//�O���[�o���ϐ��錾
//===================================================
TITLESTATE g_TitleState = TITLESTATE_NONE;				//�^�C�g���̏��
int g_nCounterTitleState;								//��ԊǗ��J�E���^�[
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TITLE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_Titlepos[NUM_TITLE];						//�^�C�g�����̍��W
D3DXCOLOR g_Titlecol[NUM_TITLE];						//�^�C�g�����̃J���[
float g_TexU;											//�e�N�X�`�����W�̊J�n�ʒu(U�l)
float g_fCol;											//Enter�����x
int g_nTitleTimer;										//������ʈړ��^�C�}�[

//===================================================
//�^�C�g���̏���������
//===================================================
void InitTitle(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREBG_DATA, &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURETITLE_DATA, &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREENTER_DATA, &g_pTextureTitle[2]);

	g_TitleState = TITLESTATE_NORMAL;	//�ʏ��ԂɃZ�b�g
	g_nCounterTitleState = 0;			//�J�E���^�[��������
	g_TexU = 0.0f;						//�w�i�ړ��ʂ�������
	g_nTitleTimer = 0;					//�����ړ��^�C�}�[������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_BG:
			//�w�i������
			InitBgTitle(nCntTitle);
			break;
		case TITLE_TITLE:
			//�Q�[���^�C�g��������
			InitGameTitle(nCntTitle);
			break;
		case TITLE_ENTER:
			//Enter������
			InitEnterTitle(nCntTitle);
			break;
		}
	}

	//bgm�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//===================================================
//�w�i����������
//===================================================
void InitBgTitle(int nCntTitle)
{

	g_Titlepos[nCntTitle] = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(g_TexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_TexU + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_TexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_TexU + 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//�Q�[���^�C�g������������
//===================================================
void InitGameTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ TITLE_X, SET_TITLE_Y, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//Enter����������
//===================================================
void InitEnterTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ ENTER_X, ENTER_Y, 0.0f };
	g_Titlecol[nCntTitle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	VERTEX_2D *pVtx;
	g_fCol = 0.01f;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//�^�C�g���̏I������
//===================================================
void UninitTitle(void)
{
	//bgm�̒�~
	StopSound();

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//�e�N�X�`���̔p��
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//===================================================
//�^�C�g���̍X�V����
//===================================================
void UpdateTitle(void)
{
	//��Ԋm�F
	switch (g_TitleState)
	{
	case TITLESTATE_NONE:	//�������Ă��Ȃ����
		break;
	case TITLESTATE_NORMAL:	//�ʏ���

		break;
	case TITLESTATE_END:	//�I�����
		g_nCounterTitleState--;

		if (g_nCounterTitleState <= 0)
		{//�J�E���^�[��0�ɂȂ����Ƃ�
			g_TitleState = TITLESTATE_NONE;	//�������Ă��Ȃ���Ԃɐݒ�
			SetFade(MODE_TUTORIAL);
		}
		break;
	}
	if (g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetKeyboardTrigger(DIK_RETURN) == true
		|| g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetJoyPadButtonTrigger(KEY_START, 0) == true
		|| g_Titlepos[TITLE_TITLE].y >= TITLE_Y && GetJoyPadButtonTrigger(KEY_A, 0) == true)
	{//�E�L�[�����͂��ꂽ�Ƃ�
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
			//�w�i�X�V
			UpdateBgTitle(nCntTitle);
			break;
		case TITLE_TITLE:
			//�Q�[���^�C�g���X�V
			UpdateGameTitle(nCntTitle);
			break;
		case TITLE_ENTER:
			//Enter�X�V
			UpdateEnterTitle(nCntTitle);
			break;
		}
	}
}

//===================================================
//�w�i�X�V
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

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y - g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + g_Titlepos[nCntTitle].x, g_Titlepos[nCntTitle].y + g_Titlepos[nCntTitle].y, g_Titlepos[nCntTitle].z);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(g_TexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_TexU + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_TexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_TexU + 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//�^�C�g���X�V
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

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + TITLE_WIDTH, g_Titlepos[nCntTitle].y + TITLE_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//Enter�X�V
//===================================================
void UpdateEnterTitle(int nCntTitle)
{
	g_Titlepos[nCntTitle] = D3DXVECTOR3{ ENTER_X, ENTER_Y, 0.0f };

	VERTEX_2D *pVtx;

	if (g_TitleState == TITLESTATE_NORMAL)
	{//�I����Ԃł͂Ȃ��ꍇ
		if (g_Titlepos[TITLE_TITLE].y >= TITLE_Y)
		{//�Q�[���^�C�g������ʒu�ɂ�����
			g_Titlecol[nCntTitle].a += g_fCol;

			if (g_Titlecol[nCntTitle].a >= 1.0f || g_Titlecol[nCntTitle].a <= 0.0f)
			{
				g_fCol *= -1.0f;
			}
		}
	}
	else if (g_TitleState != TITLESTATE_NORMAL)
	{//�I����Ԃ̏ꍇ
		static bool bEndUse = false;	//���񓧖��x�ύX
		static float fEndCol = 0.1f;	//�I���ݒ��̃J���[�ύX
		static int nCounter = 0;		//�J�E���^�[
		nCounter++;

		if (nCounter >= ENTER_TIMER)
		{//�J�E���^�[�����܂����Ƃ�
			nCounter = 0;
			switch (bEndUse)
			{
			case true:	//��x�ȍ~
				g_Titlecol[nCntTitle].a = 0.0f;	//���������v���X
				bEndUse = false;	//��x�ڈȍ~�ɕύX
				break;
			case false:	//����
				bEndUse = true;	//��x�ڈȍ~�ɕύX
				g_Titlecol[nCntTitle].a = 1.0f;	//�����J���[�����S�s�����ɐݒ�
				break;
			}

			//g_Titlecol[nCntTitle].a = fEndCol;	//���������v���X
			fEndCol *= -1.0f;	//+-���t�ɂ���

		}

	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y - ENTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x - ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos[nCntTitle].x + ENTER_WIDTH, g_Titlepos[nCntTitle].y + ENTER_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[1].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[2].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);
	pVtx[3].col = D3DXCOLOR(g_Titlecol[nCntTitle].r, g_Titlecol[nCntTitle].g, g_Titlecol[nCntTitle].b, g_Titlecol[nCntTitle].a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//===================================================
//�^�C�g���̕`�揈��
//===================================================
void DrawTitle(void)
{

	PDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
		//�^�C�g���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntTitle * 4,
			2	//���_���\���̂̃T�C�Y
		);

	}
}

//===================================================
//�Q�[����Ԑݒ�
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
//�Q�[����Ԏ擾
//===================================================
TITLESTATE GetTitleState(void)
{
	//aaaa
	return g_TitleState;
}
