//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "meshfield.h"

#define MESHFIELD_X_BLOCK			(2)		//x�����̃u���b�N�̐�
#define MESHFIELD_Z_BLOCK			(2)		//z�����̃u���b�N�̐�
#define MESHFIELD_VERTEX_NUM		((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1))		//���_��
#define MESHFIELD_INDEX_NUM			((MESHFIELD_X_BLOCK + 1) * 2 * MESHFIELD_Z_BLOCK + (MESHFIELD_Z_BLOCK - 1) * 2)		//�C���f�b�N�X��
#define MESHFIELD_PRIMITIVE_NUM		(MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4)		//�|���S����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//���b�V���t�B�[���h�̃e�N�X�`��
LPDIRECT3DINDEXBUFFER9 g_pidxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshfield;								//�ʒu
D3DXVECTOR3 g_rotMeshfield;								//����
D3DXMATRIX  g_mtxWorldMeshfield;						//���[���h�}�g���b�N�X

//==============================================
//�|���S���̏���������
//==============================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pidxBuffMeshField,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ <= MESHFIELD_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESHFIELD_X_BLOCK; nCntX++)
		{
			//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
			pVtx[0].pos = D3DXVECTOR3(-20.0f + 20.0f * nCntX, 0.0f, 20.0f - 20.0f * nCntZ);

			//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx++;
		}

	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD * pidx;
	g_pidxBuffMeshField->Lock(0, 0, (void**)&pidx, 0);

	for (int nCntZ = 0; nCntZ < MESHFIELD_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESHFIELD_X_BLOCK; nCntX++)
		{
			//�C���f�b�N�X�̐ݒ�
			pidx[0] = (WORD)(nCntX + (MESHFIELD_X_BLOCK + 1) * (nCntZ + 1));
			pidx[1] = (WORD)(nCntX + (MESHFIELD_X_BLOCK + 1) * nCntZ);
			pidx += 2;
		}
		if (nCntZ < MESHFIELD_Z_BLOCK - 1)
		{
			pidx[0] = pidx[-1];
			pidx[1] = (WORD)(MESHFIELD_X_BLOCK + 1) * (nCntZ + 2);
			pidx += 2;
		}
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pidxBuffMeshField->Unlock();
}

//==============================================
//�|���S���̏I������
//==============================================
void UninitMeshfield(void)
{
	// ���_�o�b�t�@�[�̉��
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pidxBuffMeshField != NULL)
	{
		g_pidxBuffMeshField->Release();
		g_pidxBuffMeshField = NULL;
	}
}
//==============================================
//�|���S���̍X�V����
//==============================================
void UpdateMeshfield(void)
{

}
//==============================================
//�|���S���̕`�揈��
//==============================================
void DrawMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pidxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHFIELD_PRIMITIVE_NUM, 0, MESHFIELD_PRIMITIVE_NUM);
}