#include "StageMain.h"
#include "Input.h"

//ジョイスティック・クラスのエージェントを定義
extern CJoyStick* joy;				//Tamama

//==============================================================
// Name : 
// Desc : コンストラクタ
//==============================================================
CStageMain::CStageMain()
{
	car = new CObj3dCar();
	mcar = LoadMesh(Device, _T("xfile/rx7_0.x")); // XFileの読み込み
	skybox = new CObj3d();
	mskybox = LoadMesh(Device, _T("xfile/skybox.x"));
	view = new CView();
	proj = new CProj();
	light = new CLight();
	sprite = new CSprite();
	tex = LoadTexture(Device, _T("tex/tex.png"));
	key = new CInput();

	cource = new CObj3d();
	courceInit();
	
	//Leap関連
	controller.addListener(listener);
	backing = false;

	//シーン遷移関連
	currentScene = Scene::SCENE_READY;
	
	//シーン別メンバ
	//Menu
	
	//Ready
	readyCount = 180;
	//Playing
	
	
	
	//controller.enableGesture(Gesture::Type::TYPE_SCREEN_TAP);
}
//==============================================================
// Name : 
// Desc : 
//==============================================================
void CStageMain::courceInit()
{
	mcource[0]  = LoadMesh(Device, _T("object/ground.x"));
	mcource[1]  = LoadMesh(Device, _T("object/autobacs.x"));
	mcource[2]  = LoadMesh(Device, _T("object/corner.x"));
	mcource[3]  = LoadMesh(Device, _T("object/cushion.x"));
	mcource[4]  = LoadMesh(Device, _T("object/wall.x"));
	mcource[5]  = LoadMesh(Device, _T("object/grass2.x"));
	mcource[6]  = LoadMesh(Device, _T("object/load2.x"));
	mcource[7]  = LoadMesh(Device, _T("object/shell.x"));
	mcource[8]  = LoadMesh(Device, _T("object/tree.x"));
	mcource[9]  = LoadMesh(Device, _T("object/audience.x"));
	mcource[10] = LoadMesh(Device, _T("object/fence.x"));

}

//==============================================================
// Name : 
// Desc : デストラクタ
//==============================================================
CStageMain::~CStageMain()
{
	delete car;
	delete skybox;
	delete view;
	delete proj;
	delete light;
	delete sprite;
	delete key;
	controller.removeListener(listener);
}

//==============================================================
// Name : 
// Desc : 
//==============================================================
void CStageMain::Move()
{
	switch(currentScene)
	{
	case Scene::SCENE_MENU:
		updateMenu();
		break;
	case Scene::SCENE_READY:
		updateReady();
		break;
	case Scene::SCENE_PLAYING:
		updatePlaying();
		break;
	}
	//keyBoard();
	//JoyStick_Move();		//Tamama 2013/8/20
	//leapUpdate();
	//car->move();
	//courceCollision(car);
}
void CStageMain::updateMenu()
{

}
void CStageMain::updateReady()
{
	car->camera();
	readyCount--;
	if(readyCount < 0){
		readyCount = 180;
		currentScene = Scene::SCENE_PLAYING;
	}
}
void CStageMain::updatePlaying()
{
	if(listener.IsActive())
	{
		leapUpdate();
	}
	else
	{
		JoyStick_Move();
//		keyBoard();
	}
	car->move();
	car->camera();
	courceCollision(car);
}

//==============================================================
// Name : 
// Desc : 描画
//==============================================================
void CStageMain::Draw()
{
	switch(currentScene)
	{
	case Scene::SCENE_MENU:
		drawMenu();
		break;
	case Scene::SCENE_READY:
		drawReady();
		break;
	case Scene::SCENE_PLAYING:
		drawPlaying();
		break;
	}

//	sprite->Draw(tex);
	
}
void CStageMain::drawMenu()
{

}

void CStageMain::drawPlaying()
{
	D3DXMATRIX mView;
	view->setFrom(car->getEye());
	view->setLookat(car->getAt());
	view->makeView(mView);
	view->setView(Device,mView);

	D3DXMATRIX mProj;
	proj->makeProj(mProj);
	proj->setProj(Device,mProj);

	D3DXMATRIX mWorld;
	
	drawCource(mView,mProj,mWorld);
	drawCar(mView,mProj,mWorld);
	defaultLighting();
}
void CStageMain::drawReady()
{
	drawPlaying();
}
void CStageMain::drawCource(D3DXMATRIX mView,D3DXMATRIX mProj,D3DXMATRIX mWorld)
{
	skybox->setScale(D3DXVECTOR3(100,100,100));
	skybox->makeWorld(mWorld);
	skybox->setPos(car->getPos());
	skybox->setWorld(Device,mWorld);
	skybox->draw(Device,mskybox);

	cource->makeWorld(mWorld);
	cource->setWorld(Device,mWorld);
	for(int i=0; i<11; i++){cource->draw(Device,mcource[i]);}
}
void CStageMain::drawCar(D3DXMATRIX mView,D3DXMATRIX mProj,D3DXMATRIX mWorld)
{
	car->makeWorld(mWorld);
	car->setWorld(Device,mWorld);
	car->draw(Device,mcar);
}
void CStageMain::defaultLighting()
{
	//ライト設定
	D3DXVECTOR3 direction;
	direction = D3DXVECTOR3(-0.5f, -1.0f, -1.0f);
	light->Directional(Device, direction);
}

void CStageMain::leapUpdate()
{
	//アクセル、ブレーキ、バック
	if(listener.getBack())
	{
		car->back();
		backing = true;
	}
	else if(listener.gestureAccel())
	{
		car->accel();
		backing = false;
	}
	else if(listener.gestureBrake())
	{
		car->brake(1);
	}else
		car -> brake(1);
	if(listener.HandCount() == 0)
		car -> brake(1);

	//if(listener.getBoost())
	//{
	//	car->boost();
	//}

	//ハンドル操作
	float leftY = listener.getLeftHandY();
	float rightY = listener.getRightHandY();

	float curve = listener.GetCurve();
	if(leftY - rightY > 0)//右手の方が下
	{
		car -> turnRight(curve);
	}
	else if(rightY - leftY > 0)//左手の方が下
	{
		car->turnLeft(curve);
	}
	
	if(car->getVel() == 0)
	{
		car->setAngVelZero();
	}
}
//==============================================================
// Name : 
// Desc : 
//==============================================================
void CStageMain::keyBoard()
{
	bool LeftKey=false, RightKey=false, LRKey=false;

	key->GetKeyboardState();
	if(key->diKeyState[DIK_LEFT] & 0x80){
		car->turnLeft(1.0f);
		LeftKey=true;
	}
	if(key->diKeyState[DIK_RIGHT] & 0x80){
		car->turnRight(1.0f);
		RightKey=true;
	}
	if (key->diKeyState[DIK_UP] & 0x80){
		car->accel();
	}
	else if (key->diKeyState[DIK_DOWN] & 0x80){
		car->brake(1);
	}else
	{
		car->engineBrake();
	}
	if (key->diKeyState[DIK_RETURN] & 0x80){
		car->back();
	}

	//Tamama (左右キーが押されていないときは角速度をゼロにする)
	if(LeftKey==false && RightKey==false) car->setAngVelZero();
}


//==============================================================
// Name : 
// Desc : コースとの衝突
//==============================================================
void CStageMain::courceCollision(CObj3dCar* car)
{
	D3DXVECTOR3 pos = car->getPos();
	// ここにプログラムを追加していく
	//


	//float W=2.0f, L=3.0f;	//車の幅(2xW)と高さ(2xL)
	//float x,z; //衝突判定用のx,z座標

	//
	//// -----------------------------------------------
	////	車の左上角の衝突判定
	//// -----------------------------------------------
	////車の左上角の座標
	//x =	pos.x - W/2;
	//z =	pos.z + L / 2;

	////衝突判定
	//if(z >= -188.0f && z <= 210.0) {
	//	if(x < -14.0f) {	//衝突　→ pos.xを何にすれば良いか？
	//		pos.x = -14 + W/2;
	//		car -> setPos(pos);
	//	}
	//}

	//// -----------------------------------------------
	////	車の右上角の衝突判定
	//// -----------------------------------------------

	////車の左上角の座標
	//x =	pos.x + W/2;
	//z =	pos.z + L / 2;

	////衝突判定
	//if(z >= -188.0f && z <= 210.0f) {
	//	if(x > 12.0f) {	//衝突　→ pos.xを何にすれば良いか？
	//		pos.x = 12 - W/2;
	//		car -> setPos(pos);
	//	}
	//}
	//
}

int CStageMain::CollisionDetection(D3DXVECTOR2& carPosition, float radius, D3DXVECTOR2& start, D3DXVECTOR2& end)
{
	D3DXVECTOR2 v1 = carPosition - start;
	D3DXVECTOR2 v2 = carPosition - end;
	D3DXVECTOR2 v3 = end - start;

	if (0.0f < D3DXVec2Dot(&v1, &v3) && 0.0f < D3DXVec2Dot(&v2, &-v3))
	{
		float ccw = D3DXVec2CCW(&v1, &v3);
		if (radius > (fabsf(ccw) / D3DXVec2Length(&v3)))
		{
			return (0 < ccw) ? 1 : -1;
		}
	}
	return 0;
}
//==============================================================
// ジョイスティック処理
//==============================================================
void CStageMain::JoyStick_Move(void)
{

	bool LeftKey=false, RightKey=false, LRKey=false;

	DIJOYSTATE2 state = joy->GetState();


	//４つのボタン操作
	//後退(Back)
	//if(state.rgbButtons[0] & 0x80) {	//ボタンA
	//	//car->brake();
	//	car->back();
	//}
	////右旋回
	//else if(state.rgbButtons[1] & 0x80) {	//ボタンB
	//	car->turnRight();
	//	RightKey=true;
	//}	
	////左旋回
	//else if(state.rgbButtons[2] & 0x80) {	//ボタンX
	//	car->turnLeft();
	//	LeftKey=true;
	//}	
	////前進
	//else if(state.rgbButtons[3] & 0x80) {	//ボタンY
	//	car->accel();
	//}

	//左側の方向レバー
	if(state.lX>200) {
		car->turnRight(fabs((float)state.lX / 1000.0f));
		RightKey=true;
	}
	else if(state.lX<-200) {
		car->turnLeft(fabs((float)state.lX / 1000.0f));
		LeftKey=true;
	}
	
	//アクセル、ブレーキ、バック
	if(state.rgbButtons[1] & 0x80) {//B
		car->brake(1);
	}
	else if(state.rgbButtons[0] & 0x80) { //A
		car->accel();
	}else if(car->getVel() <= 0 && state.lY > 200){
		car -> back();
	}else{
		car->engineBrake();
	}

	////右側の方向レバー
	//if(state.lRx>500) {
	//	car->turnRight();
	//	RightKey=true;
	//}
	//else if(state.lRx<-500) {
	//	car->turnLeft();
	//	LeftKey=true;
	//}

	//if(state.lRy>500) {
	//	car->brake();
	//}
	//else if(state.lRy<-500) {
	//	car->accel();
	//}

	//Tamama (左右キーが押されていないときは角速度をゼロにする)
	if(LeftKey==false && RightKey==false) car->setAngVelZero();
}
