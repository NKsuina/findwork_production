//=============================================================================
//
// �Ǐ��� [billboard.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "shadow.h"
#include "model.h"

#define NUM_BILLBOARD (128)			//�r���{�[�h�̍ő吔
#define BILLBOARD_X (3.0f)		//�r���{�[�h��X�T�C�Y
#define BILLBOARD_Y (3.0f)		//�r���{�[�h��Y�T�C�Y

//�O���[�o���ϐ��錾
int g_nIdxShadoww;
Billboard g_aBillboard[NUM_BILLBOARD];
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//�e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posBillboard;								//�ʒu
D3DXVECTOR3 g_rotBillboard;								//����
D3DXMATRIX  g_mtxWorldBillboard;							//���[���h�}�g���b�N�X

//==============================================
//�ǂ̏���������
//==============================================
void InitBillboard(void)
{
	//�O���[�o���ϐ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBillboard);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		g_rotBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_X, BILLBOARD_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_X, BILLBOARD_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_X, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_X, 0.0f, 0.0f);

		//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBillboard->Unlock();
	SetBillboard(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	g_nIdxShadoww = SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
}

//==============================================
//�ǂ̏I������
//==============================================
void UninitBillboard(void)
{
	//�o�b�t�@�j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
	//�e�N�X�`���j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//==============================================
//�ǂ̍X�V����
//==============================================
void UpdataBillboard(void)
{
	//g_nIdxShadoww = SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
	//SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_posModel.x, 0.1f, g_posModel.z));
}

//==============================================
//�ǂ̕`�揈��
//==============================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX  mtxView;

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureBillboard);

	for (int nCntBillboard = 0; nCntBillboard< NUM_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//�J�����̋t�s���ݒ�
			g_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			g_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			g_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			g_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			g_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			g_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//���e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//���C�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//���e�X�g�𖳌���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//===============================
//�ǂ̐ݒ菈��
//===============================
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].rot = rot;
			g_aBillboard[nCntBillboard].bUse = true;
			break;
		}
	}
}
