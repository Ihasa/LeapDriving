
#include "Input.h"

// マクロ
#define	RELEASE(x)	if (x){x->Release();x=NULL;}

LPDIRECTINPUT8	CInput::pDInput = NULL;
LPDIRECTINPUTDEVICE8 CInput::pDIKeyboard = NULL;

//==============================================================
// Name : 
// Desc : コンストラクタ
//==============================================================
CInput::CInput()
{
}

//==============================================================
// Name : 
// Desc : デストラクタ
//==============================================================
CInput::~CInput()
{
}

//==============================================================
// Name : 
// Desc : Direct Input 初期化
//==============================================================
HRESULT CInput::InitDInput(HINSTANCE hInstApp)
{
	HRESULT		hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
	if (FAILED(hr))
		return false;	// DirectInput8の作成に失敗
	
	return S_OK;

}

//==============================================================
// Name : 
// Desc : キーボード用オブジェクト作成
//==============================================================
HRESULT CInput::InitKeyboard(HWND hWnd)
{
	HRESULT		hr;

	//キーボード用にデバイスオブジェクトを作成
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL); 
	if (FAILED(hr))
		return false;  // デバイスの作成に失敗

	//キーボード用のデータ・フォーマットを設定
	hr = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	
	if (FAILED(hr))
		return false; // デバイスの作成に失敗

	//モードを設定（フォアグラウンド＆非排他モード）
	hr = pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);//DISCL_FOREGROUND);
	if (FAILED(hr))
		return false; // モードの設定に失敗

	//キーボード入力制御開始
	pDIKeyboard->Acquire();

	return S_OK;

}

//==============================================================
// Name : GetKeyboardState()
// Desc : キーボードの状態を取得
//==============================================================
void CInput::GetKeyboardState(void)
{
	HRESULT			hr;

	hr = pDIKeyboard->GetDeviceState(256, diKeyState);
	if (hr == DIERR_INPUTLOST)
		pDIKeyboard->Acquire();


}

//==============================================================
// Name : 
// Desc : 
//==============================================================
HRESULT CInput::ReleaseDInput(void)
{
	pDIKeyboard->Release();
	pDInput->Release();
	return S_OK;

}

// -----------------------------------------------------------------------
//	玉真追加
// -----------------------------------------------------------------------

LPDIRECTINPUT8	CJoyStick::pDInput = NULL;
LPDIRECTINPUTDEVICE8 CJoyStick::pDIDevice = NULL;
DIDEVCAPS CJoyStick::diDevCaps;   

//==============================================================
// Name : 
// Desc : コンストラクタ
//==============================================================
CJoyStick::CJoyStick(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!pDInput){
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL); 
		if (FAILED(hr)) {//DirectInput8オブジェクトの作成に失敗
		}
	}
	if(pDInput){
		hr = pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || pDIDevice==NULL){//DirectInputDevice8オブジェクトの作成に失敗
		}
	}
	if(pDInput&&pDIDevice){
		hr = pDIDevice->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))	{//c_dfDIJoystick2形式の設定に失敗
		}

		hr = pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);//DISCL_FOREGROUND);
		if (FAILED(hr)){//フォアグラウンド＆非排他モードの設定に失敗
		}

		hr = pDIDevice->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS);
		if (FAILED(hr)){//軸モードの設定に失敗
		}

		// 軸モードを設定（絶対値モードに設定。デフォルトなので必ずしも設定は必要ない）
		DIPROPDWORD diprop;
		diprop.diph.dwSize	= sizeof(diprop); 
		diprop.diph.dwHeaderSize	= sizeof(diprop.diph); 
		diprop.diph.dwObj	= 0;
		diprop.diph.dwHow	= DIPH_DEVICE;
		diprop.dwData		= DIPROPAXISMODE_ABS;
	//	diprop.dwData		= DIPROPAXISMODE_REL;	// 相対値モードの場合
		hr = pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr)){//軸モードの設定に失敗
		}
	
		// バッファリング・データを取得するため、バッファ・サイズを設定
		diprop.dwData = DIDEVICE_BUFFERSIZE;
		hr = pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr)){//バッファ・サイズの設定に失敗
		}
		
		// 入力制御開始
		pDIDevice->Acquire();
	}

}

//==============================================================
// 関数名　：　CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
// 機能概要：　ジョイスティックを列挙する関数
//==============================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得する。
	hr = CJoyStick::pDInput->CreateDevice(pdidInstance->guidInstance, &CJoyStick::pDIDevice, NULL);
	if (FAILED(hr)) 
		return DIENUM_CONTINUE;

	// ジョイスティックの能力を調べる
	CJoyStick::diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = CJoyStick::pDIDevice->GetCapabilities(&CJoyStick::diDevCaps);
	if (FAILED(hr))
	{
		// ジョイスティック能力の取得に失敗
		CJoyStick::pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//==============================================================
// 関数名　：　EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
// 機能概要：　ジョイスティックの軸を列挙する関数
//==============================================================
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;

	// 軸の値の範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize	= sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwObj	= lpddoi->dwType;
	diprg.diph.dwHow	= DIPH_BYID;
	diprg.lMin	= -1000;
	diprg.lMax	= +1000;
	hr = CJoyStick::pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
		return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//==============================================================
// Name : 
// Desc :
//==============================================================
DIJOYSTATE2 CJoyStick::GetState()
{
	if (pDIDevice!=NULL)
	{
		HRESULT hr;
		// デバイスの直接データを取得する
		hr = pDIDevice->Poll();
		if (FAILED(hr))
		{
			hr = pDIDevice->Acquire();
			while (hr==DIERR_INPUTLOST)
				hr = pDIDevice->Acquire();
		}
		hr = pDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &dijs);
		if (hr==DIERR_INPUTLOST)
			pDIDevice->Acquire();
	}
	return dijs;
}


//==============================================================
// Name : 
// Desc : デストラクタ
//==============================================================
CJoyStick::~CJoyStick()
{
	RELEASE(pDIDevice);
	RELEASE(pDInput);
}









