//=============================================================================
//
// モデル処理 [model.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "shadow.h"

//グローバル変数宣言
int g_nIdxShadow;
LPD3DXMESH g_pMeshModel = NULL;					//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;			//マテリアル情報へのポインタ
DWORD g_nNumMatModel = 0;						//マテリアル情報の数
D3DXVECTOR3 g_posModel;							//位置
D3DXVECTOR3 g_rotModel;							//向き
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;		//posの最大値と最小値
D3DXMATRIX g_mtxWorldModel;						//ワールドマトリックス

//==============================================
//モデルの初期化処理
//==============================================
void InitModel(void)
{
	int nNumVtx;		//頂点数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/wasizu_3DGame.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);

	DWORD sizeFVF;		//頂点フォーマットのサイズ

	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファのロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標を比較してモデルの最小値・最大値を取得
		//if (vtx.x < g_posModel.x)
		//{
		//	//比較対象の変数 = vtx.x;
		//}

		//x,y,zの最小値・最大値全てを比較

		//頂点フォーマットのサイズ分ポインタを勧める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	g_pMeshModel->UnlockVertexBuffer();

	g_posModel = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nIdxShadow = SetShadow(D3DXVECTOR3(g_posModel.x, 0.1f, g_posModel.z), g_rotModel); 
}

//====================================
//モデルの終了処理
//====================================
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//================================
//モデルの更新処理
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
//モデルの描画処理
//================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用のマトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);

	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);

	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//モデルパーツの描画
		g_pMeshModel->DrawSubset(nCntMat);
	}
	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}