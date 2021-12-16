//=======================================
//
//		メイン処理
//		Author	鳴海　高貴
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

//マクロ定義 
#define CLASS_NAME    "WindowClass"               //ウインドウクラスの名前
#define WINDOW_NAME    "無名"	     //ウインドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM iParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//グローバル変数
LPDIRECT3D9	g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
//MODE g_mode = MODE_TITLE;					//現在のモード

//-------------------------------------------------------------------------
//メイン関数
//-------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),               //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                       //ウインドウのスタイル
		WindowProc,                       //ウインドウプロシージャ
		0,                                //0にする
		0,                                //0にする
		hinstance,                        //インスタンスバンドル
		LoadIcon(NULL,IDI_APPLICATION),   //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),       //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),       //クライアントの領域の背景色
		NULL,                             //メニューバー
		CLASS_NAME,                       //ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)    //ファイルのアイコン
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	HWND hWnd;                          //ウインドウハンドル(識別子)
	MSG msg;							//メッセージを格納する変数
	DWORD dwCurrentTime;				//現在時刻
	DWORD dwExcLastTime;				//最後に処理した時刻

										//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,			   //拡張ウインドウスタイル
		CLASS_NAME,                    //ウインドウクラスの名前
		WINDOW_NAME,                   //ウインドウの名前
		WS_OVERLAPPEDWINDOW,           //ウインドウスタイル
		CW_USEDEFAULT,                 //ウインドウの左上X座標
		CW_USEDEFAULT,                 //ウインドウの左上Y座標
		SCREEN_WIDTH,                  //ウインドウの幅
		SCREEN_HEIGHT,                 //ウインドウの高さ
		NULL,                          //親ウインドウのハンドル
		NULL,                          //メニューハンドルまたは子ウインドウID
		hinstance,                     //インスタンスハンドル
		NULL);                         //ウインドウ作成データ

	//初期化処理
	if (FAILED(Init(hinstance, hWnd, true)))
	{//初期化が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化する
	dwExcLastTime = timeGetTime();		//現在時刻を取得(保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);				//ウインドウの表示状態を設定
	UpdateWindow(hWnd);						//クライアント領域を更新

											//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//ウインドウの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwExcLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExcLastTime = dwCurrentTime;		//処理開始の時刻(現在時刻)を保存
			}
			//更新処理
			Update();

			//描画処理
			Draw();
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録と解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:			//ウインドウ破棄のメッセージ
								//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]キーが押された
							//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメーター

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの形式
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//バックバッファの数
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファの切り替え(映像信号に同期)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//フレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

																	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
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
	//レンダステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	//キーボードの初期化処理
	if (FAILED(InitKyeboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//影の初期化
	InitShadow();

	//ポリゴンの初期化
	//InitPolygon();

	//メッシュフィールドの初期化
	InitMeshfield();

	//モデルの初期化
	InitModel();

	//ビルボードの初期化
	InitBillboard();

	//壁の初期化
	InitWall();

	//弾の初期化
	InitBullet();

	//ライトの初期化
	InitLight();

	//カメラの初期化
	InitCamera();

	//InitSound(hWnd);

	////モードの設定
	//InitFade(g_mode);

	//各種オブジェクトの初期化処理
	return S_OK;
}

//終了処理
void Uninit(void)
{
	//影の終了処理
	UninitShadow();

	//ポリゴンの終了処理
	//UninitPolygon();

	//メッシュフィールドの終了処理
	UninitMeshfield();

	//モデルの終了処理
	UninitModel();

	//ビルボードの終了処理
	UninitBillboard();

	//壁の終了処理
	UninitWall();

	//弾の終了処理
	UninitBullet();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	////サウンドの終了処理
	//UninitSound();

	////タイトル画面の終了処理
	//UninitTitle();

	////タイトル画面の終了処理
	//UninitTutorial();

	////ゲーム画面の終了処理
	//UninitGame();

	////リザルト画面の終了処理
	//UninitResult();

	////クリア画面の終了処理
	//UninitClear();

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクト
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	
	//キーボードの終了処理
	UninitKeyboard();

	//UninitFade();
}
//更新処理
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ポリゴンの更新処理
	//UpdatePolygon();

	//モデルの更新処理
	UpdateModel();

	//ビルボードの終了処理
	UpdataBillboard();

	//メッシュフィールドの更新処理
	UpdateMeshfield();

	//壁の更新処理
	UpdataWall();

	//弾の更新処理
	UpdateBullet();

	//影の更新処理
	UpdataShadow();

	//カメラの更新処理
	UpdateCamera();

	//ライトの更新処理
	UpdateLight();

	//ワイヤーフレームモードの設定
	if (GetKeyboardPress(DIK_P) == true)
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (GetKeyboardPress(DIK_O) == true)
	{//元に戻す
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//switch (g_mode)
	//{
	//case MODE_TITLE:		//タイトル画面
	//	UpdateTitle();
	//	break;

	//case MODE_TUTORIAL:			//ゲーム画面
	//	UpdateTutorial();
	//	break;

	//case MODE_GAME:			//ゲーム画面
	//	UpdateGame();
	//	break;

	//case MODE_RESULT:		//リザルト画面
	//	UpdateResult();
	//	break;

	//case MODE_CLEAR:		//リザルト画面
	//	UpdateClear();
	//	break;
	//}
	//UpdateFade();
}

//描画処理
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//成功したとき

	//カメラの描画処理
		SetCamera();

		//メッシュフィールドの描画処理
		DrawMeshfield();

		//ポリゴンの描画処理
		   //DrawPolygon();

	   //影の描画処理
		DrawShadow();

		//ビルボードの描画処理
		DrawBillboard();

		//モデルの描画処理
		DrawModel();

		//壁の描画処理
		DrawWall();

		//弾の描画処理
		//DrawBullet();

		//switch (g_mode)
		//{
		//case MODE_TITLE:		//タイトル画面
		//	DrawTitle();
		//	break;

		//case MODE_TUTORIAL:			//ゲーム画面
		//	DrawTutorial();
		//	break;

		//case MODE_GAME:			//ゲーム画面
		//	DrawGame();
		//	break;

		//case MODE_RESULT:		//リザルト画面
		//	DrawResult();
		//	break;

		//case MODE_CLEAR:		//リザルト画面
		//	DrawClear();
		//	break;
		//}
		//DrawFade();

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//モードの設定
//void SetMode(MODE mode)
//{
//	//現在の画面(モード)の終了処理
//	switch (g_mode)
//	{
//	case MODE_TITLE:		//タイトル画面
//		UninitTitle();
//		break;
//
//	case MODE_TUTORIAL:			//ゲーム画面
//		UninitTutorial();
//		break;
//
//	case MODE_GAME:			//ゲーム画面
//		UninitGame();
//		break;
//
//	case MODE_RESULT:		//リザルト画面
//		UninitResult();
//		break;
//
//	case MODE_CLEAR:		//リザルト画面
//		UninitClear();
//		break;
//	}
//
//	//新しい画面(モード)の初期化処理
//	switch (mode)
//	{
//	case MODE_TITLE:		//タイトル画面
//		InitTitle();
//		break;
//
//	case MODE_TUTORIAL:		//タイトル画面
//		InitTutorial();
//		break;
//
//	case MODE_GAME:			//ゲーム画面
//		InitGame();
//		break;
//
//	case MODE_RESULT:		//リザルト画面
//		InitResult();
//		break;
//
//	case MODE_CLEAR:		//リザルト画面
//		InitClear();
//		break;
//	}
//	g_mode = mode;			//現在の画面(モード)を切り替える
//}
//
////モードの取得
//MODE GetMode(void)
//{
//	return g_mode;
//}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}