
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Stage.h"
#include "StageMain.h"
#include "Input.h"

// 定数
#define	CAPTION			"高大一貫実験（玉真研究室)"
#define	WIDTH			640		// Width
#define	HEIGHT			480		// Height
#define	FULLSCREEN		0		// スルスクリーン時に 1 にする

// 変数
int adapter;
HWND hWnd;
int appEnd;
D3DPRESENT_PARAMETERS param;

CStage* Game;

//ジョイスティック・クラスのエージェントを定義
CJoyStick* joy;				//Tamama

// マクロ
#define	RELEASE(o)	if (o){o->Release();o=NULL;}

// オブジェクト
LPDIRECT3D9 lpD3D = NULL;
LPDIRECT3DDEVICE9 lpD3DDEV = NULL;

// プロトタイプ
int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,char *CmdLine,int CmdShow);
LRESULT CALLBACK procMain(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void DrawMain();

// WinMain - Windows アプリケーションはここから始まる
int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,char *CmdLine,int CmdShow)
{
	// Direct3D オブジェクトを作成
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// オブジェクト作成失敗
		MessageBox(NULL,"Direct3D の作成に失敗しました。","Base",MB_OK | MB_ICONSTOP);
		// 終了する
		return 0;
	}
	// 使用するアダプタ番号
	adapter = 0;
	// メインウインドウを作成する
	appEnd = 0;
	WNDCLASS wc;
	ZeroMemory(&wc,sizeof(WNDCLASS));
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpfnWndProc = procMain;
	wc.lpszClassName = "BASE";
	RegisterClass(&wc);
	int width,height;
	// ウインドウの大きさを計算
	if (FULLSCREEN)
	{
		// フルスクリーン時はそのままで OK
		width = WIDTH;
		height = HEIGHT;
	}
	else
	{
		// ウインドウ時はウインドウの外枠のサイズを考慮する
		width = WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height = HEIGHT + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	}
	// ウインドウを作成
	hWnd = CreateWindow("BASE",CAPTION,WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,width,height,NULL,NULL,hInst,NULL);
	// ウインドウの作成が完了したので、Direct3D を初期化する
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE disp;
	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	// 現在の画面モードを取得
	lpD3D->GetAdapterDisplayMode(adapter,&disp);
	// Direct3D 初期化パラメータの設定
	if (FULLSCREEN)
	{
		// フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}
	else
	{
		// ウインドウ : 現在の画面モードを使用
		d3dpp.BackBufferFormat = disp.Format;
	}
	// 表示領域サイズの設定
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	//固定ステップにする
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (!FULLSCREEN)
	{
		// ウインドウモード
		d3dpp.Windowed = 1;
	}
	// Z バッファの自動作成
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	param = d3dpp;
	// デバイスの作成 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
	{
		// 失敗したので HAL で試行
		if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
		{
			// 失敗したので REF で試行
			if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
			{
				// 結局失敗した
				MessageBox(NULL,"Direct3D の初期化に失敗しました。",CAPTION,MB_OK | MB_ICONSTOP);
				RELEASE(lpD3D);
				// 終了する
				return 0;
			}
		}
	}

	////KeyBoardのみ
	//CInput::InitDInput(hInst);
	//CInput::InitKeyboard(hWnd);

	//JoyStick立ち上げ（玉真）
	CInput::InitDInput(hInst);
	CInput::InitKeyboard(hWnd);
	joy = new CJoyStick(hInst, hWnd);


	CStage::SetDevice(lpD3DDEV);
	Game = new CStageMain();
	CSprite::CreateSprite(lpD3DDEV);

	// メインウインドウループ
	MSG msg;
	while (!appEnd)
	{
		// メッセージが来ているか?
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// メッセージの処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 描画処理
			DrawMain();
		}
	}
	// Direct3D オブジェクトを解放
	CSprite::ReleaseSprite();
	CInput::ReleaseDInput();
	delete Game;
	RELEASE(lpD3DDEV);
	RELEASE(lpD3D);

	//JotStickクラス・エージェントの解放
	delete joy;

	// 終了する
	return 0;
}

// メインウインドウプロシージャ
LRESULT CALLBACK procMain(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		// 終了フラグ
		appEnd = 1;
		return 1;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

// DrawMain - 描画メイン
// この関数は１フレームごとに呼び出されます
void DrawMain()
{
	// バックバッファと Z バッファをクリア
	lpD3DDEV->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	// シーン開始
	lpD3DDEV->BeginScene();
	// Z バッファ有効化
	lpD3DDEV->SetRenderState(D3DRS_ZENABLE,TRUE);
	// ポリゴンの両面を描画
	lpD3DDEV->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	
	// レンダリング時の色の計算方法の設定
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	// テクスチャの色を使用
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	// 頂点の色を使用
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	// フィルタ設定
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	// アルファブレンディング有効化
	lpD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	// アルファブレンディング方法を設定
	lpD3DDEV->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	lpD3DDEV->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	// レンダリング時のアルファ値の計算方法の設定
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	// テクスチャの色を使用
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	// 頂点の色を使用
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);

	//更新、描画処理
	Game->Move();
	Game->Draw();

	// シーン終了
	lpD3DDEV->EndScene();
	// バックバッファをプライマリバッファにコピー
	if (FAILED(lpD3DDEV->Present(NULL,NULL,NULL,NULL)))
	{
		lpD3DDEV->Reset(&param);
	}
}










