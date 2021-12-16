//=============================================================================
//
// �e���� [bullet.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�}�N����`
#define MAX_BULLET	(128)		//�e�̍ő吔
#define BULLET_HEIGHT (20.0f)	//�e��Y�T�C�Y
#define BULLET_WIDTH (20.0f)	//�e��X�T�C�Y

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

#endif
