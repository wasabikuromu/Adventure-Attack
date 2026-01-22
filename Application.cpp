#include <DxLib.h>
#include "SceneManager.h"
#include "InputManager.h"
#include "Application.h"



//シングルトン化
//----------------------------
//instance_ の初期化処理
Application* Application::instance_ = nullptr;

//デフォルトコンストラクタ
Application::Application(void)
{

}

//デストラクタ
Application::~Application(void)
{

}

//初期化処理
bool Application::Init(void)
{
	//システム処理
	//----------------------------------
	SetWindowText("Adventure Attack");							//ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);		//ゲームウィンドウのサイズと色モードを設定
	ChangeWindowMode(true);								//ゲームウィンドウの表示方法（false：フルスクリーン）
	if (DxLib_Init() == -1)								//DXライブラリの初期化処理
	{
		OutputDebugString("DxLibの初期化失敗");
		return false;						//DXライブラリの初期化失敗の為システム終了
	}

	//乱数の初期値を設定する
	SRand(100);		//乱数の一定化

	//インスタンスを生成する
	//-----------------------------
	//シーンマネージャ(シングルトン)
	SceneManager::CreateInstance();

	InputManager::CreateInstance();

	return true;
}

//ゲームループ処理
void Application::Run(void)
{
	//ゲームループ
	//----------------------------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//ゲームのメイン処理
		//----------------------------------
		SceneManager::GetInstance().Update();
		InputManager::GetInstance().Update();

		//描画処理
		//----------------------------------
		SetDrawScreen(DX_SCREEN_BACK);		//描画する画面を裏の画面に設定
		ClearDrawScreen();					//描画する画面の内容を消去

		SceneManager::GetInstance().Draw();

		ScreenFlip();						//裏の画面を表の画面に瞬間コピー
	}
}

//解放処理
bool Application::Release(void)
{
	//管理マネージャの解放処理
	//シーン管理用
	SceneManager::GetInstance().Release();

	//シングルトン化
	//----------------------------
	Destory();

	//システム終了処理
	//----------------------------------
	DxLib_End();		//DXライブラリの終了処理

	return true;		//ゲームの終了
}


//シングルトン化
//----------------------------

//インスタンスの生成
//外部から静的にインスタンスを生成する
void Application::CreateInstance(void)
{
	//instance_ が nullptr の場合は、初期化されたのち
	//生成されていないので、生成処理を行う。
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}

	instance_->Init();
}

//外部でインスタンスを利用可能とするために
//インスタンスを返す関数を用意する
//利便性を考えて参照型にしているがポインタ型でもよい。
Application& Application::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void Application::Destory(void)
{
	delete instance_;		//インスタンスを削除
	instance_ = nullptr;	//インスタンス格納領域を初期化
}
