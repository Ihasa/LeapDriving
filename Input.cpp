
#include "Input.h"

// �}�N��
#define	RELEASE(x)	if (x){x->Release();x=NULL;}

LPDIRECTINPUT8	CInput::pDInput = NULL;
LPDIRECTINPUTDEVICE8 CInput::pDIKeyboard = NULL;

//==============================================================
// Name : 
// Desc : �R���X�g���N�^
//==============================================================
CInput::CInput()
{
}

//==============================================================
// Name : 
// Desc : �f�X�g���N�^
//==============================================================
CInput::~CInput()
{
}

//==============================================================
// Name : 
// Desc : Direct Input ������
//==============================================================
HRESULT CInput::InitDInput(HINSTANCE hInstApp)
{
	HRESULT		hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
	if (FAILED(hr))
		return false;	// DirectInput8�̍쐬�Ɏ��s
	
	return S_OK;

}

//==============================================================
// Name : 
// Desc : �L�[�{�[�h�p�I�u�W�F�N�g�쐬
//==============================================================
HRESULT CInput::InitKeyboard(HWND hWnd)
{
	HRESULT		hr;

	//�L�[�{�[�h�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL); 
	if (FAILED(hr))
		return false;  // �f�o�C�X�̍쐬�Ɏ��s

	//�L�[�{�[�h�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	
	if (FAILED(hr))
		return false; // �f�o�C�X�̍쐬�Ɏ��s

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);//DISCL_FOREGROUND);
	if (FAILED(hr))
		return false; // ���[�h�̐ݒ�Ɏ��s

	//�L�[�{�[�h���͐���J�n
	pDIKeyboard->Acquire();

	return S_OK;

}

//==============================================================
// Name : GetKeyboardState()
// Desc : �L�[�{�[�h�̏�Ԃ��擾
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
//	�ʐ^�ǉ�
// -----------------------------------------------------------------------

LPDIRECTINPUT8	CJoyStick::pDInput = NULL;
LPDIRECTINPUTDEVICE8 CJoyStick::pDIDevice = NULL;
DIDEVCAPS CJoyStick::diDevCaps;   

//==============================================================
// Name : 
// Desc : �R���X�g���N�^
//==============================================================
CJoyStick::CJoyStick(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!pDInput){
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL); 
		if (FAILED(hr)) {//DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s
		}
	}
	if(pDInput){
		hr = pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || pDIDevice==NULL){//DirectInputDevice8�I�u�W�F�N�g�̍쐬�Ɏ��s
		}
	}
	if(pDInput&&pDIDevice){
		hr = pDIDevice->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))	{//c_dfDIJoystick2�`���̐ݒ�Ɏ��s
		}

		hr = pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);//DISCL_FOREGROUND);
		if (FAILED(hr)){//�t�H�A�O���E���h����r�����[�h�̐ݒ�Ɏ��s
		}

		hr = pDIDevice->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS);
		if (FAILED(hr)){//�����[�h�̐ݒ�Ɏ��s
		}

		// �����[�h��ݒ�i��Βl���[�h�ɐݒ�B�f�t�H���g�Ȃ̂ŕK�������ݒ�͕K�v�Ȃ��j
		DIPROPDWORD diprop;
		diprop.diph.dwSize	= sizeof(diprop); 
		diprop.diph.dwHeaderSize	= sizeof(diprop.diph); 
		diprop.diph.dwObj	= 0;
		diprop.diph.dwHow	= DIPH_DEVICE;
		diprop.dwData		= DIPROPAXISMODE_ABS;
	//	diprop.dwData		= DIPROPAXISMODE_REL;	// ���Βl���[�h�̏ꍇ
		hr = pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr)){//�����[�h�̐ݒ�Ɏ��s
		}
	
		// �o�b�t�@�����O�E�f�[�^���擾���邽�߁A�o�b�t�@�E�T�C�Y��ݒ�
		diprop.dwData = DIDEVICE_BUFFERSIZE;
		hr = pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr)){//�o�b�t�@�E�T�C�Y�̐ݒ�Ɏ��s
		}
		
		// ���͐���J�n
		pDIDevice->Acquire();
	}

}

//==============================================================
// �֐����@�F�@CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
// �@�\�T�v�F�@�W���C�X�e�B�b�N��񋓂���֐�
//==============================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾����B
	hr = CJoyStick::pDInput->CreateDevice(pdidInstance->guidInstance, &CJoyStick::pDIDevice, NULL);
	if (FAILED(hr)) 
		return DIENUM_CONTINUE;

	// �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	CJoyStick::diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = CJoyStick::pDIDevice->GetCapabilities(&CJoyStick::diDevCaps);
	if (FAILED(hr))
	{
		// �W���C�X�e�B�b�N�\�͂̎擾�Ɏ��s
		CJoyStick::pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//==============================================================
// �֐����@�F�@EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
// �@�\�T�v�F�@�W���C�X�e�B�b�N�̎���񋓂���֐�
//==============================================================
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;

	// ���̒l�͈̔͂�ݒ�i-1000�`1000�j
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
		// �f�o�C�X�̒��ڃf�[�^���擾����
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
// Desc : �f�X�g���N�^
//==============================================================
CJoyStick::~CJoyStick()
{
	RELEASE(pDIDevice);
	RELEASE(pDInput);
}









