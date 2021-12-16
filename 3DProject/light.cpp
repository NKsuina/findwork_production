//=============================================================================
//
// ライト処理 [light.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "light.h"

#define NUM_LIGHT (3)

//グローバル変数
D3DLIGHT9 g_light[NUM_LIGHT];		//ライト情報

//=========================================
//ライトの初期化
//=========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[NUM_LIGHT];		//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&g_light[0], sizeof(g_light));

	//=========================================
	//ライト1個目
	//=========================================
	//ライトの拡散光
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向
	vecDir[0] = D3DXVECTOR3(10.0f, -1.0f, -2.0f);
	
	//=========================================
	//ライト2個目
	//=========================================
	//ライトの拡散光
	g_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向
	vecDir[1] = D3DXVECTOR3(-10.0f, -1.0f, 2.0f);

	//=========================================
	//ライト3個目
	//=========================================
	//ライトの拡散光
	g_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向
	vecDir[2] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//ライトの共通設定
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//正規化する(大きさ１のベクトルにする)
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//==============================================
//ライトの終了処理
//==============================================
void UninitLight(void)
{

}

//==============================================
//ライトの更新処理
//==============================================
void UpdateLight(void)
{

}