#include <DxLib.h>
#include "Fader.h"
#include "SceneTitle.h"
#include "SceneOperation.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneGameClear.h"
#include "SceneManager.h"


//シングルトン化
//----------------------------
//instance_ の初期化処理
SceneManager* SceneManager::instance_ = nullptr;


//デフォルトコンストラクタ
SceneManager::SceneManager(void)
{
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;

	isSceneChanging_ = false;

	fader_ = nullptr;
}

//デストラクタ
SceneManager::~SceneManager(void)
{

}

//初期化処理
bool SceneManager::Init(void)
{
	//フェード
	fader_ = new Fader();
	fader_->Init();

	//シーンの生成処理
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::TITLE;
	DoChangeScene();

	//タイトルをフェードインで表示する
	fader_->SetFade(Fader::STATE::FADE_IN);
	isSceneChanging_ = true;

	return true;
}

//更新処理
void SceneManager::Update(void)
{
	//フェード処理
	//※フェードが行われていない場合は処理は何もされない
	fader_->Update();
	if (isSceneChanging_ == true)
	{
		//フェード中
		Fade();
	}
	else
	{
		//各シーンの更新処理
		scene_->Update();
	}
}

//描画処理
void SceneManager::Draw(void)
{
	scene_->Draw();

	//フェード
	fader_->Draw();
}

//解放処理
bool SceneManager::Release(void)
{
	//シーンの解放処理
	ReleaseScene(sceneID_);

	//フェードの解放
	delete fader_;
	fader_ = nullptr;


	//シングルトン化
	//----------------------------
	Destory();

	return true;
}


//シーン切り替えを依頼する関数
//isToFadeがtrueの場合、フェードアウト・インを実行する
void SceneManager::ChangeScene(SCENE_ID nextID, bool isToFade)
{
	//フェード処理が終了後にシーンを遷移する場合も
	//ある為、遷移先シーンIDをメンバー変数に保持する
	waitSceneID_ = nextID;

	if (isToFade)
	{
		//フェードを実施し、シーン遷移する
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else
	{
		//フェードを実施せずにシーン遷移する
		DoChangeScene();
	}

}


//シーンを切り替える
void SceneManager::DoChangeScene(void)
{
	//現在のシーン(sceneID_)を解放する。
	ReleaseScene(sceneID_);

	//次に移動するシーン(waitSceneID_)を生成しましょう。
	sceneID_ = waitSceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = new SceneTitle();
		break;

	case SCENE_ID::OPERATION:
		scene_ = new SceneOperation();
		break;

	case SCENE_ID::GAME:
		scene_ = new SceneGame();
		break;

	case SCENE_ID::GAMEOVER:
		scene_ = new SceneGameOver();
		break;

	case SCENE_ID::GAMECLEAR:
		scene_ = new SceneGameClear();
		break;

	default:
		break;
	}
	scene_->Init();

	//シーンの遷移が終了したので、次のシーンをクリアする
	waitSceneID_ = SCENE_ID::NONE;
}


//フェード実施用関数
void SceneManager::Fade(void)
{
	//現在のフェード設定を取得する。
	Fader::STATE fState = fader_->GetState();

	switch (fState)
	{
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd() == true)
		{
			//シーンの切り替え
			DoChangeScene();

			//フェードで明るくしていく。
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;

	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd() == true)
		{
			//フェードを終了する
			fader_->SetFade(Fader::STATE::NONE);

			//シーン切り替えの終了
			isSceneChanging_ = false;
		}
		break;
	}
}


//指定したシーンの解放関数
void SceneManager::ReleaseScene(SCENE_ID iSceneID)
{
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
		scene_ = nullptr;
	}
}


//シングルトン化
//----------------------------

//インスタンスの生成
//外部から静的にインスタンスを生成する
void SceneManager::CreateInstance(void)
{
	//instance_ が nullptr の場合は、初期化されたのち
	//生成されていないので、生成処理を行う。
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}

	instance_->Init();
}

//外部でインスタンスを利用可能とするために
//インスタンスを返す関数を用意する
//利便性を考えて参照型にしているがポインタ型でもよい。
SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void SceneManager::Destory(void)
{
	delete instance_;		//インスタンスを削除
	instance_ = nullptr;	//インスタンス格納領域を初期化
}
