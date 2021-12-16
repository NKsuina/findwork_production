//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"

//グローバル変数
Camera g_camera;		//カメラ情報
//=======================================
//カメラ初期化処理
//=======================================
void InitCamera(void)
{
	//視点・注視点・上方向を設定する
	g_camera.posV = D3DXVECTOR3(0.0f, 30.0f, -30.0f);		//視点
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//注視点
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向ベクトル
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//回転
	float Distance1 = g_camera.posV.z - g_camera.posR.z;
	float Distance2 = g_camera.posV.x - g_camera.posR.x;
	g_camera.fDistance = sqrtf((Distance1 * Distance1) + (Distance2 * Distance2));  	//視点から注視点の距離
}

//==============================================
//カメラの終了処理
//==============================================
void UninitCamera(void)
{

}

//==============================================
//カメラの更新処理
//==============================================
void UpdateCamera(void)
{
	//注視点の回転
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

	//視点の旋回
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

	//カメラの移動
	if (GetKeyboardPress(DIK_LEFTARROW) == true)
	{//左移動
		g_camera.posV.x -= sinf(D3DX_PI * 0.5f - g_camera.rot.y) * 0.5f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 0.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_RIGHTARROW) == true)
	{//右移動
		g_camera.posV.x -= sinf(D3DX_PI * -0.5f + g_camera.rot.y) * 0.5f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 0.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_UPARROW) == true)
	{//奥移動
		g_camera.posV.x += sinf(g_camera.rot.y) * 0.0f;
		g_camera.posV.z += cosf( 0.5f + g_camera.rot.y) * 0.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_DOWNARROW) == true)
	{//手前移動
		g_camera.posV.x -= sinf(g_camera.rot.y) * 0.0f;
		g_camera.posV.z -= cosf(0.5f + g_camera.rot.y) * 0.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

}
//==============================================
//カメラの設定処理
//==============================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera. posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの作成
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);		//ビュー座標行列の設定

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian
		(60.0f),//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//アスペクト比
		1.0f,
		1000.0f);

	//プロジェクトマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}
//===============================
//Cameraの取得
//===============================
Camera *GetCamera(void)
{
	return &g_camera;
}