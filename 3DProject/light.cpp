//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "light.h"

#define NUM_LIGHT (3)

//�O���[�o���ϐ�
D3DLIGHT9 g_light[NUM_LIGHT];		//���C�g���

//=========================================
//���C�g�̏�����
//=========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[NUM_LIGHT];		//���C�g�̕����x�N�g��

	//���C�g���N���A����
	ZeroMemory(&g_light[0], sizeof(g_light));

	//=========================================
	//���C�g1��
	//=========================================
	//���C�g�̊g�U��
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//���C�g�̕���
	vecDir[0] = D3DXVECTOR3(10.0f, -1.0f, -2.0f);
	
	//=========================================
	//���C�g2��
	//=========================================
	//���C�g�̊g�U��
	g_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//���C�g�̕���
	vecDir[1] = D3DXVECTOR3(-10.0f, -1.0f, 2.0f);

	//=========================================
	//���C�g3��
	//=========================================
	//���C�g�̊g�U��
	g_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//���C�g�̕���
	vecDir[2] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//���C�g�̋��ʐݒ�
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���K������(�傫���P�̃x�N�g���ɂ���)
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//==============================================
//���C�g�̏I������
//==============================================
void UninitLight(void)
{

}

//==============================================
//���C�g�̍X�V����
//==============================================
void UpdateLight(void)
{

}