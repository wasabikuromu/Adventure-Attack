#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "SceneGameOver.h"

////デフォルトコンストラクタ
//SceneGameOver::SceneGameOver(void)
//{
//
//}
//
////デストラクタ
//SceneGameOver::~SceneGameOver(void)
//{
//
//}

//初期化処理
bool SceneGameOver::Init(void)
{

	gameOverImage_ = LoadGraph("Image/UI/GameOver.png");
	if (gameOverImage_ == -1)
	{
		return -1;
	}

	gameOverImage1_ = LoadGraph("Image/UI/SpaceBack.png");
	if (gameOverImage1_ == -1)
	{
		return -1;
	}

	soundGameOver_ = LoadSoundMem("Image/Sound/GameOver2.mp3");		//残念な音を鳴らす処理
	ChangeVolumeSoundMem(255 * 75 / 100, soundGameOver_);			//音量の設定
	PlaySoundMem(soundGameOver_, DX_PLAYTYPE_BACK);					//残念な音を再生

	gameOverCounter = 0;

	return true;
}

//更新処理
void SceneGameOver::Update(void)
{
	gameOverCounter++;

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

//描画処理
void SceneGameOver::Draw(void)
{
	DrawGraph(0, 0, gameOverImage_, true);

	if ((gameOverCounter / 5) % 4 != 0)
	{
		DrawGraph(20,500, gameOverImage1_, true);
	}
}

//解放処理
bool SceneGameOver::Release(void)
{
	DeleteGraph(gameOverImage_);
	DeleteGraph(gameOverImage1_);

	DeleteSoundMem(soundGameOver_);

	DeleteSoundMem(soundClick_);

	return true;
}
