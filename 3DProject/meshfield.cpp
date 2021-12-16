//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "meshfield.h"

#define MESHFIELD_X_BLOCK			(2)		//x方向のブロックの数
#define MESHFIELD_Z_BLOCK			(2)		//z方向のブロックの数
#define MESHFIELD_VERTEX_NUM		((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1))		//頂点数
#define MESHFIELD_INDEX_NUM			((MESHFIELD_X_BLOCK + 1) * 2 * MESHFIELD_Z_BLOCK + (MESHFIELD_Z_BLOCK - 1) * 2)		//インデックス数
#define MESHFIELD_PRIMITIVE_NUM		(MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4)		//ポリゴン数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//メッシュフィールドのテクスチャ
LPDIRECT3DINDEXBUFFER9 g_pidxBuffMeshField = NULL;		//インデックスバッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posMeshfield;								//位置
D3DXVECTOR3 g_rotMeshfield;								//向き
D3DXMATRIX  g_mtxWorldMeshfield;						//ワールドマトリックス

//==============================================
//ポリゴンの初期化処理
//==============================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pidxBuffMeshField,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ <= MESHFIELD_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESHFIELD_X_BLOCK; nCntX++)
		{
			//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
			pVtx[0].pos = D3DXVECTOR3(-20.0f + 20.0f * nCntX, 0.0f, 20.0f - 20.0f * nCntZ);

			//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx++;
		}

	}
	//頂点バッファのアンロック
	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファのロック
	WORD * pidx;
	g_pidxBuffMeshField->Lock(0, 0, (void**)&pidx, 0);

	for (int nCntZ = 0; nCntZ < MESHFIELD_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESHFIELD_X_BLOCK; nCntX++)
		{
			//インデックスの設定
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
	//頂点バッファのアンロック
	g_pidxBuffMeshField->Unlock();
}

//==============================================
//ポリゴンの終了処理
//==============================================
void UninitMeshfield(void)
{
	// 頂点バッファーの解放
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	//インデックスバッファの開放
	if (g_pidxBuffMeshField != NULL)
	{
		g_pidxBuffMeshField->Release();
		g_pidxBuffMeshField = NULL;
	}
}
//==============================================
//ポリゴンの更新処理
//==============================================
void UpdateMeshfield(void)
{

}
//==============================================
//ポリゴンの描画処理
//==============================================
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pidxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHFIELD_PRIMITIVE_NUM, 0, MESHFIELD_PRIMITIVE_NUM);
}