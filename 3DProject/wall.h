//=============================================================================
//
// 壁処理 [wall.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DCOLOR col;
	D3DXMATRIX mtxWorld;
	bool bUse;
}Wall;

//プロトタイプ宣言
void InitWall (void);
void UninitWall(void);
void UpdataWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot , D3DXCOLOR col);
#endif