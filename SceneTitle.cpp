#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "SceneTitle.h"

////デフォルトコンストラクタ
//SceneTitle::SceneTitle(void)
//{
//	titleImage_ = 0;
//}

////デストラクタ
//SceneTitle::~SceneTitle(void)
//{
//
//}

//初期化処理
bool SceneTitle::Init(void)
{
	titleImage_ = LoadGraph("Image/UI/Title.png");
	if (titleImage_ == -1)
	{
		return -1;
	}

	titleSpace_ = LoadGraph("Image/UI/SpaceTitle.png");
	if (titleSpace_ == -1)
	{
		return -1;
	}

	soundClick_ = LoadSoundMem("Image/Sound/Click.mp3");	//BGMを鳴らす処理

	titleCounter_ = 0;

	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	titleCounter_++;

	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		PlaySoundMem(soundClick_, DX_PLAYTYPE_BACK, true);		//BGMを再生

		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::OPERATION
			, true
		);
	}
}

//描画処理
void SceneTitle::Draw(void)
{
	//taitoru
	DrawGraph(0, 0, titleImage_, true);

	if ((titleCounter_ / 5) % 4 != 0)
	{
		DrawGraph(80, 500, titleSpace_, true);
	}
}

//解放処理
bool SceneTitle::Release(void)
{
	DeleteGraph(titleImage_);

	DeleteGraph(titleSpace_);

	DeleteSoundMem(soundClick_);

	return true;
}