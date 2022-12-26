//==========================================================
//
//���b�V���t�B�[���h�`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"
#include "texture.h"
#include "fileload.h"
#include <stdio.h>

//==========================================================
//�}�N����`
//==========================================================
#define MAX_MESHFIELD		(256)		//���b�V���t�B�[���h�̍ő吔
#define PARALELLFIELD_SIZE	(200.0f)	//���b�V���t�B�[���h�̃T�C�Y
#define PARALELLFIELD_WIDTH	(150)		//�p��������

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void CreateVertexFieldBuff(void);
void CreateIndexFieldBuff(void);

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MeshField g_aMeshField[MAX_MESHFIELD];					//���b�V���t�B�[���h 
int g_nMeshFieldVertex;									//���_�̎g�p���m�F
int g_nMeshFieldIndex;									//�C���f�b�N�X�̎d�l���m�F

//==========================================================
//�|���S���̏���������
//==========================================================
void InitMeshField(void)
{
	//�e��ϐ��̏�����
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		g_aMeshField[nCntField].Polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntField].Polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntField].Polygon.fHeight = 0.0f;
		g_aMeshField[nCntField].Polygon.fWidth = 0.0f;
		g_aMeshField[nCntField].bUse = false;
	}

	//�p�������p�̒n�ʂ�ݒ�
	if (GetMode() == MODE_GAME)
	{
		SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PARALELLFIELD_SIZE, PARALELLFIELD_SIZE, 1, PARALELLFIELD_WIDTH, 20);
	}

}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitMeshField(void)
{
	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshField != NULL)
	{
		g_pIdexBuffMeshField->Release();
		g_pIdexBuffMeshField = NULL;
	}

}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateMeshField(void)
{
	
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshField[nCntField].Polygon.mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntField].Polygon.rot.y, g_aMeshField[nCntField].Polygon.rot.x, g_aMeshField[nCntField].Polygon.rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].Polygon.mtxWorld, &g_aMeshField[nCntField].Polygon.mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntField].Polygon.pos.x, g_aMeshField[nCntField].Polygon.pos.y, g_aMeshField[nCntField].Polygon.pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].Polygon.mtxWorld, &g_aMeshField[nCntField].Polygon.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntField].Polygon.mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshField,
				0,
				sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdexBuffMeshField);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, *SetTexture(g_aMeshField[nCntField].nTextype));

			if (nCntField != 0)
			{
				//�|���S���̕`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
					0,
					0,
					g_aMeshField[nCntField].nVertex,		//�p�ӂ������_��
					nCntIndex,
					g_aMeshField[nCntField].nIndex - 2	//�`�悷��v���~�e�B�u��
				);
			}

			nCntIndex += g_aMeshField[nCntField].nIndex;
		}
	}
}

//==========================================================
//���b�V���t�B�[���h�̐ݒ菈��
//==========================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType)
{
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aMeshField[nCntField].Polygon.pos = pos;
			g_aMeshField[nCntField].Polygon.rot = rot;
			g_aMeshField[nCntField].Polygon.fHeight = fHeight;
			g_aMeshField[nCntField].Polygon.fWidth = fWidth;
			g_aMeshField[nCntField].nHeightMesh = nHeight;
			g_aMeshField[nCntField].nWidthMesh = nWidth;
			g_aMeshField[nCntField].nVertex = (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nHeightMesh + 1);
			g_aMeshField[nCntField].bUse = true;
			g_aMeshField[nCntField].nIndex = (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nHeightMesh) * 2 + (g_aMeshField[nCntField].nHeightMesh - 1) * 2;
			g_aMeshField[nCntField].nTextype = nType;

			//���_���ƃC���f�b�N�X������
			g_nMeshFieldIndex += g_aMeshField[nCntField].nIndex;
			g_nMeshFieldVertex += g_aMeshField[nCntField].nVertex;
			break;
		}
	}
}

//==========================================================
//���b�V���t�B�[���h�̃C���f�b�N�X�ݒ菈��
//==========================================================
void SetIndexMeshField(int nCntField)
{
	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nMeshFieldIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshField[nCntField].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshField[nCntField].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshField[nCntField].nWidthMesh + 2) / (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nWidthMesh + 2))
			- (nCntIdx / (g_aMeshField[nCntField].nWidthMesh + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshField[nCntField].nWidthMesh + 2) / (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshField[nCntField].nWidthMesh + 2));
		pIdx[nCntIdx * 2] += g_nMeshFieldVertex;
		pIdx[nCntIdx * 2 + 1] += g_nMeshFieldVertex;

	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshField->Unlock();

	g_nMeshFieldIndex += g_aMeshField[nCntField].nIndex;
}

//==========================================================
//���b�V���t�B�[���h�̒��_���W�ݒ菈��
//==========================================================
void SetpVtxMeshField(int nCntField)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nMeshFieldVertex;

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshField[nCntField].nVertex; nCntpVtx++)
	{
		//���_���W
		pVtx[nCntpVtx].pos.x = -(g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh) + (nCntpVtx % (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].Polygon.fWidth * 2));
		pVtx[nCntpVtx].pos.y = 0.0f;
		pVtx[nCntpVtx].pos.z = (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh) + ((nCntpVtx / (g_aMeshField[nCntField].nWidthMesh + 1) * (-g_aMeshField[nCntField].Polygon.fHeight * 2)));
		
		//�@��
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//�F
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshField[nCntField].nWidthMesh + 1)), 1.0f * (nCntpVtx / (g_aMeshField[nCntField].nWidthMesh + 1)));
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	g_nMeshFieldVertex += g_aMeshField[nCntField].nVertex;
}

//==========================================================
//���b�V���t�B�[���h�̒��_�o�b�t�@����
//==========================================================
void CreateVertexFieldBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshFieldIndex,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshField,
		NULL);
}

//==========================================================
//���b�V���t�B�[���h�̃C���f�b�N�X�o�b�t�@����
//==========================================================
void CreateIndexFieldBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nMeshFieldVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);
}

//==========================================================
//���b�V���t�B�[���h�̐ݒu���ǂݍ���
//==========================================================
void LoadMeshFieldData(FILE *pFile)
{
	char aStr[256];	//�]���ȕ��͓ǂݍ��ݗp
	MeshField meshField;

	//������
	memset(&meshField, NULL, sizeof(meshField));

	//�I�������܂œǂݍ���
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//�z�u���m�F
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//�e�N�X�`��
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &meshField.nTextype);	//�e�N�X�`�����ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//���W
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.pos.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.pos.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.pos.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.rot.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.rot.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.rot.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &meshField.nHeightMesh);	//�c���ǂݍ���
			fscanf(pFile, "%d", &meshField.nWidthMesh);		//�����ǂݍ���

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//�T�C�Y
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.fHeight);//�c���ǂݍ���
			fscanf(pFile, "%f", &meshField.Polygon.fWidth);	//�����ǂݍ���
		}

		//�I��
		if (strcmp(&aStr[0], ENDFIELDSET_TXT) == 0)
		{//�I������
			break;
		}
	}

	//�t�B�[���h�̔z�u
	SetMeshField(meshField.Polygon.pos, D3DXToRadian(meshField.Polygon.rot),
		meshField.Polygon.fHeight, meshField.Polygon.fWidth, 
		meshField.nHeightMesh, meshField.nWidthMesh, meshField.nTextype);

}

//==========================================================
//���b�V���t�B�[���h�̐����֘A
//==========================================================
void CreateFieldBuff(void)
{

	//���_�o�b�t�@����
	CreateVertexFieldBuff();

	//�C���f�b�N�X�o�b�t�@����
	CreateIndexFieldBuff();

	//��񏉊���
	g_nMeshFieldVertex = 0;
	g_nMeshFieldIndex = 0;

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//�g�p����Ă���ꍇ
			//�C���f�b�N�X�̐ݒ�
			SetIndexMeshField(nCntField);

			//���_���W�̐ݒ�
			SetpVtxMeshField(nCntField);
		}
	}
}

//==========================================================
//���b�V���t�B�[���h�Ƃ̓����蔻��
//==========================================================
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, MeshField **pMeshField)
{
	bool bLand = false;;
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//�g�p����Ă���ꍇ
			if (g_aMeshField[nCntField].Polygon.rot.y == D3DX_PI || g_aMeshField[nCntField].Polygon.rot.y == 0.0f)
			{//�c����
				if (pPos->x + vtxMax.x >= g_aMeshField[nCntField].Polygon.pos.x - (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh)
					&& pPos->x + vtxMin.x <= g_aMeshField[nCntField].Polygon.pos.x + (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh)
					&& pPos->z + vtxMax.z >= g_aMeshField[nCntField].Polygon.pos.z - (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh)
					&& pPos->z + vtxMin.z <= g_aMeshField[nCntField].Polygon.pos.z + (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh))
				{//�t�B�[���h�͈͓̔��ɂ���ꍇ
					if (pPosOld->y >= g_aMeshField[nCntField].Polygon.pos.y &&
						pPos->y < g_aMeshField[nCntField].Polygon.pos.y)
					{//�߂荞�񂾏ꍇ
						if (g_aMeshField[nCntField].nTextype != 1 && GetMode() == MODE_GAME || GetMode() != MODE_GAME)
						{
							*pMeshField = &g_aMeshField[nCntField];
						}

						pPos->y = g_aMeshField[nCntField].Polygon.pos.y;
						pMove->y = 0.0f;
						bLand = true;
					}
					else if (pPosOld->y + 50.0f <= g_aMeshField[nCntField].Polygon.pos.y &&
						pPos->y + 50.0f > g_aMeshField[nCntField].Polygon.pos.y &&  g_aMeshField[nCntField].Polygon.rot.z == D3DX_PI)
					{
						pMove->y = 0.0f;
						pPos->y = g_aMeshField[nCntField].Polygon.pos.y - 50.0f;
					}
				}
			}
			else if (g_aMeshField[nCntField].Polygon.rot.y == D3DX_PI * 0.5f || g_aMeshField[nCntField].Polygon.rot.y == -D3DX_PI * 0.5f)
			{//������
				if (pPos->x + vtxMax.x >= g_aMeshField[nCntField].Polygon.pos.x - (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh)
					&& pPos->x + vtxMin.x <= g_aMeshField[nCntField].Polygon.pos.x + (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh)
					&& pPos->z + vtxMax.z >= g_aMeshField[nCntField].Polygon.pos.z - (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh)
					&& pPos->z + vtxMin.z <= g_aMeshField[nCntField].Polygon.pos.z + (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh))
				{//�t�B�[���h�͈͓̔��ɂ���ꍇ
					if (pPosOld->y >= g_aMeshField[nCntField].Polygon.pos.y &&
						pPos->y < g_aMeshField[nCntField].Polygon.pos.y)
					{//�߂荞�񂾏ꍇ
						if (g_aMeshField[nCntField].nTextype != 1 && GetMode() == MODE_GAME || GetMode() != MODE_GAME)
						{
							*pMeshField = &g_aMeshField[nCntField];
						}

						pPos->y = g_aMeshField[nCntField].Polygon.pos.y;
						pMove->y = 0.0f;
						bLand = true;
					}
					else if (pPosOld->y + 50.0f <= g_aMeshField[nCntField].Polygon.pos.y &&
						pPos->y + 50.0f > g_aMeshField[nCntField].Polygon.pos.y &&  g_aMeshField[nCntField].Polygon.rot.z == D3DX_PI)
					{
						pMove->y = 0.0f;
						pPos->y = g_aMeshField[nCntField].Polygon.pos.y - 50.0f;
					}
				}
			}

		}
	}

	return bLand;
}

//==========================================================
//�����蔻��m�F
//==========================================================
bool CollisionParalellMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, MeshField **pMeshField)
{
	bool bLand = false;;
	if (g_aMeshField[0].bUse == true)
	{//�g�p����Ă���ꍇ
		if (g_aMeshField[0].Polygon.rot.y == D3DX_PI || g_aMeshField[0].Polygon.rot.y == 0.0f)
		{//�c����
			if (pPos->x + vtxMax.x >= g_aMeshField[0].Polygon.pos.x - (g_aMeshField[0].Polygon.fWidth * g_aMeshField[0].nWidthMesh)
				&& pPos->x + vtxMin.x <= g_aMeshField[0].Polygon.pos.x + (g_aMeshField[0].Polygon.fWidth * g_aMeshField[0].nWidthMesh)
				&& pPos->z + vtxMax.z >= g_aMeshField[0].Polygon.pos.z - (g_aMeshField[0].Polygon.fHeight * g_aMeshField[0].nHeightMesh)
				&& pPos->z + vtxMin.z <= g_aMeshField[0].Polygon.pos.z + (g_aMeshField[0].Polygon.fHeight * g_aMeshField[0].nHeightMesh))
			{//�t�B�[���h�͈͓̔��ɂ���ꍇ
				if (pPosOld->y >= g_aMeshField[0].Polygon.pos.y &&
					pPos->y < g_aMeshField[0].Polygon.pos.y)
				{//�߂荞�񂾏ꍇ
					if (g_aMeshField[0].nTextype != 1 && GetMode() == MODE_GAME || GetMode() != MODE_GAME)
					{
						*pMeshField = &g_aMeshField[0];
					}

					pPos->y = g_aMeshField[0].Polygon.pos.y;
					pMove->y = 0.0f;
					bLand = true;
				}
				else if (pPosOld->y + 50.0f <= g_aMeshField[0].Polygon.pos.y &&
					pPos->y + 50.0f > g_aMeshField[0].Polygon.pos.y &&  g_aMeshField[0].Polygon.rot.z == D3DX_PI)
				{
					pMove->y = 0.0f;
					pPos->y = g_aMeshField[0].Polygon.pos.y - 50.0f;
				}
			}
		}
		else if (g_aMeshField[0].Polygon.rot.y == D3DX_PI * 0.5f || g_aMeshField[0].Polygon.rot.y == -D3DX_PI * 0.5f)
		{//������
			if (pPos->x + vtxMax.x >= g_aMeshField[0].Polygon.pos.x - (g_aMeshField[0].Polygon.fHeight * g_aMeshField[0].nHeightMesh)
				&& pPos->x + vtxMin.x <= g_aMeshField[0].Polygon.pos.x + (g_aMeshField[0].Polygon.fHeight * g_aMeshField[0].nHeightMesh)
				&& pPos->z + vtxMax.z >= g_aMeshField[0].Polygon.pos.z - (g_aMeshField[0].Polygon.fWidth * g_aMeshField[0].nWidthMesh)
				&& pPos->z + vtxMin.z <= g_aMeshField[0].Polygon.pos.z + (g_aMeshField[0].Polygon.fWidth * g_aMeshField[0].nWidthMesh))
			{//�t�B�[���h�͈͓̔��ɂ���ꍇ
				if (pPosOld->y >= g_aMeshField[0].Polygon.pos.y &&
					pPos->y < g_aMeshField[0].Polygon.pos.y)
				{//�߂荞�񂾏ꍇ
					if (g_aMeshField[0].nTextype != 1 && GetMode() == MODE_GAME || GetMode() != MODE_GAME)
					{
						*pMeshField = &g_aMeshField[0];
					}

					pPos->y = g_aMeshField[0].Polygon.pos.y;
					pMove->y = 0.0f;
					bLand = true;
				}
				else if (pPosOld->y + 50.0f <= g_aMeshField[0].Polygon.pos.y &&
					pPos->y + 50.0f > g_aMeshField[0].Polygon.pos.y &&  g_aMeshField[0].Polygon.rot.z == D3DX_PI)
				{
					pMove->y = 0.0f;
					pPos->y = g_aMeshField[0].Polygon.pos.y - 50.0f;
				}
			}
		}
	}
	return bLand;
}

//==========================================================
//�p�������p���`��
//==========================================================
void DrawParalellMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshField[0].Polygon.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[0].Polygon.rot.y, g_aMeshField[0].Polygon.rot.x, g_aMeshField[0].Polygon.rot.z);
	D3DXMatrixMultiply(&g_aMeshField[0].Polygon.mtxWorld, &g_aMeshField[0].Polygon.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField[0].Polygon.pos.x, g_aMeshField[0].Polygon.pos.y, g_aMeshField[0].Polygon.pos.z);
	D3DXMatrixMultiply(&g_aMeshField[0].Polygon.mtxWorld, &g_aMeshField[0].Polygon.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[0].Polygon.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshField,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdexBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, *SetTexture(g_aMeshField[0].nTextype));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_aMeshField[0].nVertex,		//�p�ӂ������_��
		nCntIndex,
		g_aMeshField[0].nIndex - 2	//�`�悷��v���~�e�B�u��
	);


	nCntIndex += g_aMeshField[0].nIndex;
}