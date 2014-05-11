//=============================================================================
// File: 
// Desc: CInput�N���X�̃C���^�[�t�F�[�X
//=============================================================================
#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

class CInput
{
protected:
	static LPDIRECTINPUT8	pDInput;
	static LPDIRECTINPUTDEVICE8	pDIKeyboard;
	

public:
	BYTE					diKeyState[256];
	static HRESULT InitDInput(HINSTANCE);
	static HRESULT InitKeyboard(HWND);
	void GetKeyboardState(void);
	static HRESULT ReleaseDInput(void);

	CInput();
	~CInput();
};

// -----------------------------------------------------------------------
//	�ʐ^�ǉ�
// -----------------------------------------------------------------------
class CJoyStick
{
public:
	static LPDIRECTINPUT8	pDInput;
	static LPDIRECTINPUTDEVICE8	pDIDevice;
	static DIDEVCAPS	diDevCaps;	// �W���C�X�e�B�b�N�̔\��
private:
	DIJOYSTATE  dijf;
	DIJOYSTATE2 dijs;
	

public:
	
	DIJOYSTATE2 GetState(void);

	CJoyStick(HINSTANCE hInst, HWND hWnd);
	~CJoyStick();
};

#define DIDEVICE_BUFFERSIZE	100						// �f�o�C�X�ɐݒ肷��o�b�t�@�E�T�C�Y
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
// -----------------------------------------------------------------------

#endif 
