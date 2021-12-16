//=============================================================================
//
// 壁処理 [billboard.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "shadow.h"
#include "model.h"

#define NUM_BILLBOARD (128)			//ビルボードの最大数
#define BILLBOARD_X (3.0f)		//ビルボードのXサイズ
#define BILLBOARD_Y (3.0f)		//ビルボードのYサイズ

//グローバル変数宣言
int g_nIdxShadoww;
Billboard g_aBillboard[NUM_BILLBOARD];
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//テクスチャへのポインタ
D3DXVECTOR3 g_posBillboard;								//位置
D3DXVECTOR3 g_rotBillboard;								//向き
D3DXMATRIX  g_mtxWorldBillboard;							//ワールドマトリックス

//==============================================
//壁の初期化処理
//==============================================
void InitBillboard(void)
{
	//グローバル変数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBillboard);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		g_rotBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_X, BILLBOARD_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_X, BILLBOARD_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_X, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_X, 0.0f, 0.0f);

		//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffBillboard->Unlock();
	SetBillboard(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	g_nIdxShadoww = SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
}

//==============================================
//壁の終了処理
//==============================================
void UninitBillboard(void)
{
	//バッファ破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
	//テクスチャ破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//==============================================
//壁の更新処理
//==============================================
void UpdataBillboard(void)
{
	//g_nIdxShadoww = SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
	//SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_posModel.x, 0.1f, g_posModel.z));
}

//==============================================
//壁の描画処理
//==============================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DXMATRIX  mtxView;

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	for (int nCntBillboard = 0; nCntBillboard< NUM_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//カメラの逆行列を設定
			g_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			g_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			g_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			g_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			g_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			g_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//αテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//ライトを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//αテストを無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//===============================
//壁の設定処理
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
