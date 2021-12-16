//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "wall.h"

#define NUM_WALL (128)
#define WALL_XSIZE (20.0f)
#define WALL_ZSIZE (10.0f)

//�O���[�o���ϐ��錾
Wall g_aWall[NUM_WALL];
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//�e�N�X�`���ւ̃|�C���^

//==============================================
//�ǂ̏���������
//==============================================
void InitWall(void)
{
	//�O���[�o���ϐ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wall000.jpg",
		&g_pTextureWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
		pVtx[0].pos = D3DXVECTOR3(-WALL_XSIZE, 0.0f, WALL_ZSIZE);
		pVtx[1].pos = D3DXVECTOR3(WALL_XSIZE, 0.0f, WALL_ZSIZE);
		pVtx[2].pos = D3DXVECTOR3(-WALL_XSIZE, 0.0f, -WALL_ZSIZE);
		pVtx[3].pos = D3DXVECTOR3(WALL_XSIZE, 0.0f, -WALL_ZSIZE);

		//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffWall->Unlock();

	//�O���̕�
	SetWall(D3DXVECTOR3(0.0f, 5.0f, 20.0f) , D3DXVECTOR3(1.5f, D3DX_PI / 1.0f, 0.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//���̕�
	SetWall(D3DXVECTOR3(0.0f, 5.0f, -20.0f), D3DXVECTOR3(1.5f, 10.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//��O��
	SetWall(D3DXVECTOR3(20.0f, 5.0f, 0.0f), D3DXVECTOR3(1.75f, D3DX_PI / 2.0f, 0.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));			//�E��
	SetWall(D3DXVECTOR3(-20.0f, 5.0f, 0.0f), D3DXVECTOR3(1.5f, D3DX_PI * 1.5f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//����
	

	//�����̕�
}

//==============================================
//�ǂ̏I������
//==============================================
void UninitWall(void)
{
	//�o�b�t�@�j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//�e�N�X�`���j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//==============================================
//�ǂ̍X�V����
//==============================================
void UpdataWall(void)
{

}

//==============================================
//�ǂ̕`�揈��
//==============================================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureWall);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�Ǖ`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);	//�v���~�e�B�u�̎��
		}
	}
}

//===============================
//�ǂ̐ݒ菈��
//===============================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for ( int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].col = col;
			g_aWall[nCntWall].bUse = true;

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffWall->Unlock();
}