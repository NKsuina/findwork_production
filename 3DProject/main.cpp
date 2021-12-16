//=======================================
//
//		���C������
//		Author	�C�@���M
//
//=======================================
#include "bullet.h"
#include "camera.h"
#include "light.h"
#include "main.h"
#include "model.h"
#include "meshfield.h"
#include "input.h"
#include "Polygon.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"

//�}�N����` 
#define CLASS_NAME    "WindowClass"               //�E�C���h�E�N���X�̖��O
#define WINDOW_NAME    "����"	     //�E�C���h�E�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM iParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//�O���[�o���ϐ�
LPDIRECT3D9	g_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	//Direct3D�f�o�C�X�ւ̃|�C���^
//MODE g_mode = MODE_TITLE;					//���݂̃��[�h

//-------------------------------------------------------------------------
//���C���֐�
//-------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),               //WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,                       //�E�C���h�E�̃X�^�C��
		WindowProc,                       //�E�C���h�E�v���V�[�W��
		0,                                //0�ɂ���
		0,                                //0�ɂ���
		hinstance,                        //�C���X�^���X�o���h��
		LoadIcon(NULL,IDI_APPLICATION),   //�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),       //�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),       //�N���C�A���g�̗̈�̔w�i�F
		NULL,                             //���j���[�o�[
		CLASS_NAME,                       //�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)    //�t�@�C���̃A�C�R��
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	HWND hWnd;                          //�E�C���h�E�n���h��(���ʎq)
	MSG msg;							//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;				//���ݎ���
	DWORD dwExcLastTime;				//�Ō�ɏ�����������

										//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,			   //�g���E�C���h�E�X�^�C��
		CLASS_NAME,                    //�E�C���h�E�N���X�̖��O
		WINDOW_NAME,                   //�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,           //�E�C���h�E�X�^�C��
		CW_USEDEFAULT,                 //�E�C���h�E�̍���X���W
		CW_USEDEFAULT,                 //�E�C���h�E�̍���Y���W
		SCREEN_WIDTH,                  //�E�C���h�E�̕�
		SCREEN_HEIGHT,                 //�E�C���h�E�̍���
		NULL,                          //�e�E�C���h�E�̃n���h��
		NULL,                          //���j���[�n���h���܂��͎q�E�C���h�EID
		hinstance,                     //�C���X�^���X�n���h��
		NULL);                         //�E�C���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hinstance, hWnd, true)))
	{//�����������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//����������
	dwExcLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);				//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);						//�N���C�A���g�̈���X�V

											//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//�E�C���h�E�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwExcLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExcLastTime = dwCurrentTime;		//�����J�n�̎���(���ݎ���)��ۑ�
			}
			//�X�V����
			Update();

			//�`�揈��
			Draw();
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^�Ɖ���
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:			//�E�C���h�E�j���̃��b�Z�[�W
								//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]�L�[�������ꂽ
							//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^�[

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̌`��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�o�b�N�o�b�t�@�̐�
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//�t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

																	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_������CPU�ōs��)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//�����_�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	//�L�[�{�[�h�̏���������
	if (FAILED(InitKyeboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�e�̏�����
	InitShadow();

	//�|���S���̏�����
	//InitPolygon();

	//���b�V���t�B�[���h�̏�����
	InitMeshfield();

	//���f���̏�����
	InitModel();

	//�r���{�[�h�̏�����
	InitBillboard();

	//�ǂ̏�����
	InitWall();

	//�e�̏�����
	InitBullet();

	//���C�g�̏�����
	InitLight();

	//�J�����̏�����
	InitCamera();

	//InitSound(hWnd);

	////���[�h�̐ݒ�
	//InitFade(g_mode);

	//�e��I�u�W�F�N�g�̏���������
	return S_OK;
}

//�I������
void Uninit(void)
{
	//�e�̏I������
	UninitShadow();

	//�|���S���̏I������
	//UninitPolygon();

	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	//���f���̏I������
	UninitModel();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�ǂ̏I������
	UninitWall();

	//�e�̏I������
	UninitBullet();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	////�T�E���h�̏I������
	//UninitSound();

	////�^�C�g����ʂ̏I������
	//UninitTitle();

	////�^�C�g����ʂ̏I������
	//UninitTutorial();

	////�Q�[����ʂ̏I������
	//UninitGame();

	////���U���g��ʂ̏I������
	//UninitResult();

	////�N���A��ʂ̏I������
	//UninitClear();

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//UninitFade();
}
//�X�V����
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�|���S���̍X�V����
	//UpdatePolygon();

	//���f���̍X�V����
	UpdateModel();

	//�r���{�[�h�̏I������
	UpdataBillboard();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshfield();

	//�ǂ̍X�V����
	UpdataWall();

	//�e�̍X�V����
	UpdateBullet();

	//�e�̍X�V����
	UpdataShadow();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

	//���C���[�t���[�����[�h�̐ݒ�
	if (GetKeyboardPress(DIK_P) == true)
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (GetKeyboardPress(DIK_O) == true)
	{//���ɖ߂�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//switch (g_mode)
	//{
	//case MODE_TITLE:		//�^�C�g�����
	//	UpdateTitle();
	//	break;

	//case MODE_TUTORIAL:			//�Q�[�����
	//	UpdateTutorial();
	//	break;

	//case MODE_GAME:			//�Q�[�����
	//	UpdateGame();
	//	break;

	//case MODE_RESULT:		//���U���g���
	//	UpdateResult();
	//	break;

	//case MODE_CLEAR:		//���U���g���
	//	UpdateClear();
	//	break;
	//}
	//UpdateFade();
}

//�`�揈��
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//���������Ƃ�

	//�J�����̕`�揈��
		SetCamera();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshfield();

		//�|���S���̕`�揈��
		   //DrawPolygon();

	   //�e�̕`�揈��
		DrawShadow();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//���f���̕`�揈��
		DrawModel();

		//�ǂ̕`�揈��
		DrawWall();

		//�e�̕`�揈��
		//DrawBullet();

		//switch (g_mode)
		//{
		//case MODE_TITLE:		//�^�C�g�����
		//	DrawTitle();
		//	break;

		//case MODE_TUTORIAL:			//�Q�[�����
		//	DrawTutorial();
		//	break;

		//case MODE_GAME:			//�Q�[�����
		//	DrawGame();
		//	break;

		//case MODE_RESULT:		//���U���g���
		//	DrawResult();
		//	break;

		//case MODE_CLEAR:		//���U���g���
		//	DrawClear();
		//	break;
		//}
		//DrawFade();

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//���[�h�̐ݒ�
//void SetMode(MODE mode)
//{
//	//���݂̉��(���[�h)�̏I������
//	switch (g_mode)
//	{
//	case MODE_TITLE:		//�^�C�g�����
//		UninitTitle();
//		break;
//
//	case MODE_TUTORIAL:			//�Q�[�����
//		UninitTutorial();
//		break;
//
//	case MODE_GAME:			//�Q�[�����
//		UninitGame();
//		break;
//
//	case MODE_RESULT:		//���U���g���
//		UninitResult();
//		break;
//
//	case MODE_CLEAR:		//���U���g���
//		UninitClear();
//		break;
//	}
//
//	//�V�������(���[�h)�̏���������
//	switch (mode)
//	{
//	case MODE_TITLE:		//�^�C�g�����
//		InitTitle();
//		break;
//
//	case MODE_TUTORIAL:		//�^�C�g�����
//		InitTutorial();
//		break;
//
//	case MODE_GAME:			//�Q�[�����
//		InitGame();
//		break;
//
//	case MODE_RESULT:		//���U���g���
//		InitResult();
//		break;
//
//	case MODE_CLEAR:		//���U���g���
//		InitClear();
//		break;
//	}
//	g_mode = mode;			//���݂̉��(���[�h)��؂�ւ���
//}
//
////���[�h�̎擾
//MODE GetMode(void)
//{
//	return g_mode;
//}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}