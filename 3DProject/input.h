//=============================================================================
//
// �L�[�{�[�h���͏��� [input.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//�v���g�^�C�v�錾
HRESULT InitKyeboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

#endif