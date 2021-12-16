//=============================================================================
//
// �J�������� [camera.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"

//�O���[�o���ϐ�
Camera g_camera;		//�J�������
//=======================================
//�J��������������
//=======================================
void InitCamera(void)
{
	//���_�E�����_�E�������ݒ肷��
	g_camera.posV = D3DXVECTOR3(0.0f, 30.0f, -30.0f);		//���_
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����_
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������x�N�g��
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//��]
	float Distance1 = g_camera.posV.z - g_camera.posR.z;
	float Distance2 = g_camera.posV.x - g_camera.posR.x;
	g_camera.fDistance = sqrtf((Distance1 * Distance1) + (Distance2 * Distance2));  	//���_���璍���_�̋���
}

//==============================================
//�J�����̏I������
//==============================================
void UninitCamera(void)
{

}

//==============================================
//�J�����̍X�V����
//==============================================
void UpdateCamera(void)
{
	//�����_�̉�]
	if (GetKeyboardPress(DIK_Q) == true)
	{
		g_camera.rot.y -= 0.05f;
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{
		g_camera.rot.y += 0.05f;
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//���_�̐���
	if (GetKeyboardPress(DIK_X) == true)
	{
		g_camera.rot.y += 0.05f;
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	else if (GetKeyboardPress(DIK_Z) == true)
	{
		g_camera.rot.y -= 0.05f;
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//�J�����̈ړ�
	if (GetKeyboardPress(DIK_LEFTARROW) == true)
	{//���ړ�
		g_camera.posV.x -= sinf(D3DX_PI * 0.5f - g_camera.rot.y) * 0.5f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 0.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_RIGHTARROW) == true)
	{//�E�ړ�
		g_camera.posV.x -= sinf(D3DX_PI * -0.5f + g_camera.rot.y) * 0.5f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 0.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_UPARROW) == true)
	{//���ړ�
		g_camera.posV.x += sinf(g_camera.rot.y) * 0.0f;
		g_camera.posV.z += cosf( 0.5f + g_camera.rot.y) * 0.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_DOWNARROW) == true)
	{//��O�ړ�
		g_camera.posV.x -= sinf(g_camera.rot.y) * 0.0f;
		g_camera.posV.z -= cosf(0.5f + g_camera.rot.y) * 0.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

}
//==============================================
//�J�����̐ݒ菈��
//==============================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera. posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̍쐬
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);		//�r���[���W�s��̐ݒ�

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian
		(60.0f),//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//�A�X�y�N�g��
		1.0f,
		1000.0f);

	//�v���W�F�N�g�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}
//===============================
//Camera�̎擾
//===============================
Camera *GetCamera(void)
{
	return &g_camera;
}