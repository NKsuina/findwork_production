//=============================================================================
//
// 弾処理 [bullet.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET	(128)		//弾の最大数
#define BULLET_HEIGHT (20.0f)	//弾のYサイズ
#define BULLET_WIDTH (20.0f)	//弾のXサイズ

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

#endif
