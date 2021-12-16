//=============================================================================
//
// ���f������ [model.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "shadow.h"

//�O���[�o���ϐ��錾
int g_nIdxShadow;
LPD3DXMESH g_pMeshModel = NULL;					//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;			//�}�e���A�����ւ̃|�C���^
DWORD g_nNumMatModel = 0;						//�}�e���A�����̐�
D3DXVECTOR3 g_posModel;							//�ʒu
D3DXVECTOR3 g_rotModel;							//����
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;		//pos�̍ő�l�ƍŏ��l
D3DXMATRIX g_mtxWorldModel;						//���[���h�}�g���b�N�X

//==============================================
//���f���̏���������
//==============================================
void InitModel(void)
{
	int nNumVtx;		//���_��
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/wasizu_3DGame.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);

	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y

	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W���r���ă��f���̍ŏ��l�E�ő�l���擾
		//if (vtx.x < g_posModel.x)
		//{
		//	//��r�Ώۂ̕ϐ� = vtx.x;
		//}

		//x,y,z�̍ŏ��l�E�ő�l�S�Ă��r

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^�����߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pMeshModel->UnlockVertexBuffer();

	g_posModel = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nIdxShadow = SetShadow(D3DXVECTOR3(g_posModel.x, 0.1f, g_posModel.z), g_rotModel); 
}

//====================================
//���f���̏I������
//====================================
void UninitModel(void)
{
	//���b�V���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//================================
//���f���̍X�V����
//================================
void UpdateModel(void)
{
	Camera *pCamera = GetCamera();

	if (GetKeyboardPress(DIK_W))
	{
		g_posModel.x += sinf(pCamera->rot.y) * 0.5f;
		g_posModel.z += cosf(pCamera->rot.y) * 0.5f;

		g_rotModel.y = pCamera->rot.y + D3DX_PI;
	}

	if (GetKeyboardPress(DIK_S))
	{
		g_posModel.x -= sinf(pCamera->rot.y) * 0.5f;
		g_posModel.z -= cosf(pCamera->rot.y) * 0.5f;

		g_rotModel.y = pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_A))
	{
		g_posModel.x -= sinf(D3DX_PI * 0.5f + pCamera->rot.y) * 0.5f;
		g_posModel.z -= cosf(D3DX_PI * 0.5f + pCamera->rot.y) * 0.5f;

		g_rotModel.y = D3DX_PI * 0.5f + pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_D))
	{
		g_posModel.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * 0.5f;
		g_posModel.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * 0.5f;

		g_rotModel.y = D3DX_PI * 0.5f + pCamera->rot.y - D3DX_PI;
	}
	SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_posModel.x, 0.1f,g_posModel.z));
}

//================================
//���f���̕`�揈��
//================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);

	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);

	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//���f���p�[�c�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}
	//�ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}