#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SceneOperation.h"

bool SceneOperation::Init(void)
{
	operationImage_ = LoadGraph("Image/UI/OperationNc.png");
	if (operationImage_ == -1)
	{
		//条件が真
		OutputDebugString("OperationNc.pngファイルが見つからない");
		return false;                      //ゲームの終了
	}

	operationImage1_ = LoadGraph("Image/UI/Sousa.png");
	if (operationImage1_ == -1)
	{
		//条件が真
		OutputDebugString("OperationNc.pngファイルが見つからない");
		return false;                      //ゲームの終了
	}

	operationImage2_ = LoadGraph("Image/UI/gogame.png");
	if (operationImage2_ == -1)
	{
		//条件が真
		OutputDebugString("OperationNc.pngファイルが見つからない");
		return false;                      //ゲームの終了
	}

	operationCounter_ = 0;

	oncPos_ = { -4000,0 };	

	return true;
}

void SceneOperation::Update(void)
{
	operationCounter_++;

	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		soundClick_ = LoadSoundMem("Image/Sound/Click.mp3");	//BGMを鳴らす処理
		PlaySoundMem(soundClick_, DX_PLAYTYPE_BACK, true);		//BGMを再生

		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAME
			, true
		);
	}

	auto& ins = InputManager::GetInstance();

	if (oncPos_.x <= 0 && oncPos_.x >=  -4000)
	{
		if (ins.IsTrgDown(KEY_INPUT_RIGHT))
		{
			oncPos_.x += 1000;
		}

		if (ins.IsTrgDown(KEY_INPUT_LEFT))
		{
			oncPos_.x -= 1000;
		}
	}

	if (oncPos_.x > 0)
	{
		oncPos_.x = 0;
	}

	if (oncPos_.x < -4000)
	{
		oncPos_.x = -4000;
	}
}

void SceneOperation::Draw(void)
{
	DrawGraph(oncPos_.x, oncPos_.y, operationImage_, true);

	DrawGraph(0, 0, operationImage1_, true);

	if ((operationCounter_ / 5) % 4 != 0)
	{
		DrawGraph(30, 60, operationImage2_, true);
	}
}

bool SceneOperation::Release(void)
{
	DeleteGraph(operationImage_);
	DeleteGraph(operationImage1_);

	DeleteSoundMem(soundClick_);

	return false;
}
