//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "wall.h"

#define NUM_WALL (128)
#define WALL_XSIZE (20.0f)
#define WALL_ZSIZE (10.0f)

//グローバル変数宣言
Wall g_aWall[NUM_WALL];
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//テクスチャへのポインタ

//==============================================
//壁の初期化処理
//==============================================
void InitWall(void)
{
	//グローバル変数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wall000.jpg",
		&g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
		pVtx[0].pos = D3DXVECTOR3(-WALL_XSIZE, 0.0f, WALL_ZSIZE);
		pVtx[1].pos = D3DXVECTOR3(WALL_XSIZE, 0.0f, WALL_ZSIZE);
		pVtx[2].pos = D3DXVECTOR3(-WALL_XSIZE, 0.0f, -WALL_ZSIZE);
		pVtx[3].pos = D3DXVECTOR3(WALL_XSIZE, 0.0f, -WALL_ZSIZE);

		//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffWall->Unlock();

	//外側の壁
	SetWall(D3DXVECTOR3(0.0f, 5.0f, 20.0f) , D3DXVECTOR3(1.5f, D3DX_PI / 1.0f, 0.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//奥の壁
	SetWall(D3DXVECTOR3(0.0f, 5.0f, -20.0f), D3DXVECTOR3(1.5f, 10.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//手前壁
	SetWall(D3DXVECTOR3(20.0f, 5.0f, 0.0f), D3DXVECTOR3(1.75f, D3DX_PI / 2.0f, 0.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));			//右壁
	SetWall(D3DXVECTOR3(-20.0f, 5.0f, 0.0f), D3DXVECTOR3(1.5f, D3DX_PI * 1.5f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));		//左壁
	

	//内側の壁
}

//==============================================
//壁の終了処理
//==============================================
void UninitWall(void)
{
	//バッファ破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//テクスチャ破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//==============================================
//壁の更新処理
//==============================================
void UpdataWall(void)
{

}

//==============================================
//壁の描画処理
//==============================================
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureWall);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//壁描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);	//プリミティブの種類
		}
	}
}

//===============================
//壁の設定処理
//===============================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D*pVtx;

	//頂点バッファをロック
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
	//頂点バッファのアンロック
	g_pVtxBuffWall->Unlock();
}