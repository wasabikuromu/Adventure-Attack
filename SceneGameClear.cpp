#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "SceneGameClear.h"

////デフォルトコンストラクタ
//SceneGameClear::SceneGameClear(void)
//{
//
//}
//
////デストラクタ
//SceneGameClear::~SceneGameClear(void)
//{
//
//}


bool SceneGameClear::Init(void)
{
	gameClearImage_ = LoadGraph("Image/UI/GameClear.png");
	if (gameClearImage_ == -1)
	{
		return -1;
	}

	gameClearImage1_ = LoadGraph("Image/UI/SpaceBack.png");
	if (gameClearImage1_ == -1)
	{
		return -1;
	}
	

	soundGameclear_ = LoadSoundMem("Image/Sound/GameClear.mp3");		//発射音を鳴らす処理
	PlaySoundMem(soundGameclear_, DX_PLAYTYPE_BACK, true);				//発射音を再生

	gameClearCounter = 0;

	return true;
}

void SceneGameClear::Update(void)
{
	gameClearCounter++;

	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		soundClick_ = LoadSoundMem("Image/Sound/Click.mp3");	//BGMを鳴らす処理
		ChangeVolumeSoundMem(255 * 75 / 100, soundClick_);		//音量の設定
		PlaySoundMem(soundClick_, DX_PLAYTYPE_BACK, true);		//BGMを再生

		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE
			, true
		);
	}
}

void SceneGameClear::Draw(void)
{
	DrawGraph(0, 0, gameClearImage_, true);
	
	if ((gameClearCounter / 5) % 4 != 0)
	{
		DrawGraph(20, 500, gameClearImage1_, true);
	}
}

bool SceneGameClear::Release(void)
{
	DeleteGraph(gameClearImage_);
	DeleteGraph(gameClearImage1_);

	DeleteSoundMem(soundGameclear_);

	DeleteSoundMem(soundClick_);

	return false;
}
