//==========================================================
//
//�t�F�[�h����
//Author ����ɐ�
//
//==========================================================
#include "fade.h"

//�O���[�o���錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//���_�o�b�t�@�ւ̃|�C���^
FADE g_fade;									//�t�F�[�h�̏��
MODE g_modeNext;								//���̉��(���[�h)
D3DXCOLOR g_fadeCol;							//�t�F�[�h�J���[

//�t�F�[�h�̏���������
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	g_fade = FADE_IN;	//�t�F�[�h�C����Ԃ�
	g_modeNext = modeNext;	//���̉�ʂ�ݒ�
	g_fadeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S���ɂ��Ă���

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
		);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[1].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[2].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[3].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);
		pVtx[0].col = D3DXCOLOR(g_fadeCol.r, g_fadeCol.g, g_fadeCol.b, g_fadeCol.a);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//�t�F�[�h�̏I������
void UninitFade(void)
{
	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//�t�F�[�h�̍X�V����
void UpdateFade(void)
{

	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����
			g_fadeCol.a -= 1.0f / 30;	//�����ɋ߂Â��Ă���

			if (g_fadeCol.a <= 0.0f)
			{//���S�ɓ����ɂȂ����ꍇ
				g_fadeCol.a = 0.0f;	//�����x�𓧖���
				g_fade = FADE_NONE;	//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���

			g_fadeCol.a += 1.0f / 30;	//�s�����ɋ߂Â��Ă���

			if (g_fadeCol.a >= 1.0f)
			{//���S�ɕs�����ɂȂ����ꍇ
				g_fadeCol.a = 1.0f;	//�����x��s������

				//���[�h�̐ݒ�(���̉�ʂɈڍs)
				SetMode(g_modeNext);

				g_fade = FADE_IN;	//�������Ă��Ȃ���Ԃ�
				
				
			}
		}
	}

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	//���_�J���[�̐ݒ�
	pVtx[1].col = g_fadeCol;
	pVtx[2].col = g_fadeCol;
	pVtx[3].col = g_fadeCol;
	pVtx[0].col = g_fadeCol;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//�t�F�[�h�̕`�揈��
void DrawFade(void)
{
	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO�̏ꍇ
		PDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

		//�f�o�C�X�̎擾
		pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffFade,
			0,
			sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�t�F�[�h�̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,
			2	//���_���\���̂̃T�C�Y
		);
	}
}

//�t�F�[�h�̓����x�擾
FADE GetFade(void)
{
	//�����x��Ԃ�
	return g_fade;
}

//�t�F�[�h�̏�Ԑݒ�
void SetFade(MODE modeNext)
{
	if (g_fade != FADE_OUT)
	{
		g_fade = FADE_OUT;	//�t�F�[�h�A�E�g��Ԃ�
		g_modeNext = modeNext;	//���̉��(���[�h)��
		g_fadeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�����ɐݒ�
	}
}
