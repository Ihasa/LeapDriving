
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Stage.h"
#include "StageMain.h"
#include "Input.h"

// �萔
#define	CAPTION			"�����ю����i�ʐ^������)"
#define	WIDTH			640		// Width
#define	HEIGHT			480		// Height
#define	FULLSCREEN		0		// �X���X�N���[������ 1 �ɂ���

// �ϐ�
int adapter;
HWND hWnd;
int appEnd;
D3DPRESENT_PARAMETERS param;

CStage* Game;

//�W���C�X�e�B�b�N�E�N���X�̃G�[�W�F���g���`
CJoyStick* joy;				//Tamama

// �}�N��
#define	RELEASE(o)	if (o){o->Release();o=NULL;}

// �I�u�W�F�N�g
LPDIRECT3D9 lpD3D = NULL;
LPDIRECT3DDEVICE9 lpD3DDEV = NULL;

// �v���g�^�C�v
int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,char *CmdLine,int CmdShow);
LRESULT CALLBACK procMain(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void DrawMain();

// WinMain - Windows �A�v���P�[�V�����͂�������n�܂�
int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,char *CmdLine,int CmdShow)
{
	// Direct3D �I�u�W�F�N�g���쐬
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// �I�u�W�F�N�g�쐬���s
		MessageBox(NULL,"Direct3D �̍쐬�Ɏ��s���܂����B","Base",MB_OK | MB_ICONSTOP);
		// �I������
		return 0;
	}
	// �g�p����A�_�v�^�ԍ�
	adapter = 0;
	// ���C���E�C���h�E���쐬����
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
	// �E�C���h�E�̑傫�����v�Z
	if (FULLSCREEN)
	{
		// �t���X�N���[�����͂��̂܂܂� OK
		width = WIDTH;
		height = HEIGHT;
	}
	else
	{
		// �E�C���h�E���̓E�C���h�E�̊O�g�̃T�C�Y���l������
		width = WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height = HEIGHT + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	}
	// �E�C���h�E���쐬
	hWnd = CreateWindow("BASE",CAPTION,WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,width,height,NULL,NULL,hInst,NULL);
	// �E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE disp;
	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	// ���݂̉�ʃ��[�h���擾
	lpD3D->GetAdapterDisplayMode(adapter,&disp);
	// Direct3D �������p�����[�^�̐ݒ�
	if (FULLSCREEN)
	{
		// �t���X�N���[�� : �قƂ�ǂ̃A�_�v�^�ŃT�|�[�g����Ă���t�H�[�}�b�g���g�p
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}
	else
	{
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
		d3dpp.BackBufferFormat = disp.Format;
	}
	// �\���̈�T�C�Y�̐ݒ�
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	//�Œ�X�e�b�v�ɂ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (!FULLSCREEN)
	{
		// �E�C���h�E���[�h
		d3dpp.Windowed = 1;
	}
	// Z �o�b�t�@�̎����쐬
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	param = d3dpp;
	// �f�o�C�X�̍쐬 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
	{
		// ���s�����̂� HAL �Ŏ��s
		if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
		{
			// ���s�����̂� REF �Ŏ��s
			if (FAILED(lpD3D->CreateDevice(adapter,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&lpD3DDEV)))
			{
				// ���ǎ��s����
				MessageBox(NULL,"Direct3D �̏������Ɏ��s���܂����B",CAPTION,MB_OK | MB_ICONSTOP);
				RELEASE(lpD3D);
				// �I������
				return 0;
			}
		}
	}

	////KeyBoard�̂�
	//CInput::InitDInput(hInst);
	//CInput::InitKeyboard(hWnd);

	//JoyStick�����グ�i�ʐ^�j
	CInput::InitDInput(hInst);
	CInput::InitKeyboard(hWnd);
	joy = new CJoyStick(hInst, hWnd);


	CStage::SetDevice(lpD3DDEV);
	Game = new CStageMain();
	CSprite::CreateSprite(lpD3DDEV);

	// ���C���E�C���h�E���[�v
	MSG msg;
	while (!appEnd)
	{
		// ���b�Z�[�W�����Ă��邩?
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// ���b�Z�[�W�̏���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �`�揈��
			DrawMain();
		}
	}
	// Direct3D �I�u�W�F�N�g�����
	CSprite::ReleaseSprite();
	CInput::ReleaseDInput();
	delete Game;
	RELEASE(lpD3DDEV);
	RELEASE(lpD3D);

	//JotStick�N���X�E�G�[�W�F���g�̉��
	delete joy;

	// �I������
	return 0;
}

// ���C���E�C���h�E�v���V�[�W��
LRESULT CALLBACK procMain(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		// �I���t���O
		appEnd = 1;
		return 1;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

// DrawMain - �`�惁�C��
// ���̊֐��͂P�t���[�����ƂɌĂяo����܂�
void DrawMain()
{
	// �o�b�N�o�b�t�@�� Z �o�b�t�@���N���A
	lpD3DDEV->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	// �V�[���J�n
	lpD3DDEV->BeginScene();
	// Z �o�b�t�@�L����
	lpD3DDEV->SetRenderState(D3DRS_ZENABLE,TRUE);
	// �|���S���̗��ʂ�`��
	lpD3DDEV->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	
	// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	// �e�N�X�`���̐F���g�p
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	// ���_�̐F���g�p
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	// �t�B���^�ݒ�
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	lpD3DDEV->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	// �A���t�@�u�����f�B���O�L����
	lpD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	// �A���t�@�u�����f�B���O���@��ݒ�
	lpD3DDEV->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	lpD3DDEV->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	// �e�N�X�`���̐F���g�p
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	// ���_�̐F���g�p
	lpD3DDEV->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	lpD3DDEV->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);

	//�X�V�A�`�揈��
	Game->Move();
	Game->Draw();

	// �V�[���I��
	lpD3DDEV->EndScene();
	// �o�b�N�o�b�t�@���v���C�}���o�b�t�@�ɃR�s�[
	if (FAILED(lpD3DDEV->Present(NULL,NULL,NULL,NULL)))
	{
		lpD3DDEV->Reset(&param);
	}
}










