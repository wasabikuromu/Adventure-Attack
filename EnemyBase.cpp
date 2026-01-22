#include <DxLib.h>
#include "Vector2.h"
#include "Vector2F.h"
#include "SceneGame.h"
#include "Stage.h"
#include "EnemyBase.h"

//デフォルトコンストラクタ
EnemyBase::EnemyBase(void)
{
	sceneGame_ = nullptr;

	imgFileName = "";

	pos_ = { 0,0 };

	size_ = { 0,0 };

	hitPos_ = { 0,0 };

	hitBox_ = { 0,0 };

	speed_ = 0;

	dir_ = AsoUtility::DIR::MAX;

	cntAnim_ = 0;

	speedAnim_ = 0.0f;

	shotSpeed_ = 0;

	isAlive = true;

	hp_ = 0;
}

EnemyBase::~EnemyBase(void)
{

}

//初期化処理(初回の１度のみ実行される)
bool EnemyBase::Init(SceneGame* parent)
{
	sceneGame_ = parent;

	//敵キャラ個別のパラメータ設定
	SetParam();

	//画像の読み込み
	std::string path = "Image/Enemy/";
	path = path + imgFileName;

	int ret = LoadDivGraph(
		path.c_str()	
		, ImgNumX_ * ImgNumY_
		, ImgNumX_
		, ImgNumY_
		, size_.x
		, size_.y
		, &imgs_[0][0]
	);
	if (ret == -1)
	{
		OutputDebugString("ボスの読み込み失敗");
		return false;
	}

	//初期位置
	pos_ = { 300,400 };

	//向いている方向
	dir_ = AsoUtility::DIR::LEFT;
	soundEdamage_ = LoadSoundMem("Image/Sound/Edamage.mp3");	//攻撃音を鳴らす処理
	soundDead_ = LoadSoundMem("Image/Sound/Edead.mp3");		//攻撃音を鳴らす処理

	//生存フラグ
	isAlive = true;
	isStart_ = false;
	isFirst_ = false;
	//カウンタリセット
	cntAnim_ = 0;

	return true;
}

//更新処理(毎フレーム実行)
void EnemyBase::Update(void)
{
	//プレイヤーの座標を取得
	Vector2F playerPos = sceneGame_->GetPlayerPos();

	if (isAlive == false)
	{
		return;
	}

	if (isStart_ == false)
	{
		if (startPos_.x < playerPos.x)
		{
			isStart_ = true;
		}
		else
		{
			return;
		}
	}

	Move();

	//アニメーションのカウンタを更新
	cntAnim_++;
}

//描画処理(毎フレーム実行)
void EnemyBase::Draw(void)
{
	Vector2 cameraPos = sceneGame_->GetCameraPos();

	if (isAlive)
	{

		int animNum = static_cast<int>(cntAnim_ * speedAnim_) % (ImgNumX_ * ImgNumY_);

		//オーバーロード関数を利用した
		Vector2 pos = AsoUtility::Round(pos_);
		
		//プレイヤーの座標を取得
		Vector2F playerPos = sceneGame_->GetPlayerPos();
		float vecx = playerPos.x - pos_.x;
		bool bard;
		if (vecx > 0)
		{
			bard = false;
		}
		else
		{
			bard = true;
		}

		if (isRight_ == false)
		{
			bard = !bard;
		}

		//DrawGraph(pos_.x,pos_.y,imgs_[2][0], true);
		DrawRotaGraphF(pos.x - cameraPos.x, pos.y - cameraPos.y
			, 1.0f		//拡大
			, 0.0f		//回転
			, imgs_[0][animNum]
			, true
			, bard);
	}

	/*// debug
	Vector2F dPos = pos_;
	dPos.x += hitPos_.x;
	dPos.y += hitPos_.y;

	Vector2F sPos = dPos;
	sPos.x -= hitBox_.x;
	sPos.y -= hitBox_.y;
	Vector2F ePos = dPos;
	ePos.x += hitBox_.x;
	ePos.y += hitBox_.y;

	sPos.x -= cameraPos.x;
	sPos.y -= cameraPos.y;
	ePos.x -= cameraPos.x;
	ePos.y -= cameraPos.y;
	DrawBox(sPos.x, sPos.y, ePos.x, ePos.y, 0xff0000, false);*/

}

//解放処理(終了時の一度のみ実行)
bool EnemyBase::Release(void)
{
	for (int y = 0; y < static_cast<int>(AsoUtility::DIR::MAX); y++)
	{
		for (int x = 0; x < MAX_NUM_ANIM; x++)
		{
			DeleteGraph(imgs_[y][x]);
		}
	}

	DeleteSoundMem(soundDead_);
	DeleteSoundMem(soundEdamage_);

	return true;
}

//座標を設定する
void EnemyBase::SetPos(Vector2F value)
{
	pos_.x = value.x;
	pos_.y = value.y;
}

//座標を返す
Vector2F EnemyBase::GetPos(void)
{
	return pos_;
}

void EnemyBase::SetStartPos(Vector2 value)
{
	startPos_ = value;
}

//サイズを返す
Vector2 EnemyBase::GetSize(void)
{
	return size_;
}

//敵の生存状態を返す
bool EnemyBase::IsAlive(void)
{
	return isAlive;
}


//ダメージを与える関数
void EnemyBase::Hit(int value)
{
	if (!isFirst_)
	{
		hp_ -= value;
		isFirst_ = true;

		ChangeVolumeSoundMem(255 * 50 / 100, soundEdamage_);		//音量の設定
		PlaySoundMem(soundEdamage_, DX_PLAYTYPE_BACK, true);		//発射音を再生
	}
	if (hp_ <= 0)
	{
		ChangeVolumeSoundMem(255 * 80 / 100, soundDead_);		//音量の設定
		PlaySoundMem(soundDead_, DX_PLAYTYPE_BACK, true);		//発射音を再生

		isAlive = false;
	}
}

Vector2 EnemyBase::GetHitPos(void)
{
	return hitPos_;
}

Vector2 EnemyBase::GetHitBox(void)
{
	return hitBox_;
}

void EnemyBase::KnockBack(void)
{

}

void EnemyBase::Move(void)
{

}

Vector2 EnemyBase::GetColPos(COL_LR lr, COL_TD td)
{
	Vector2 ret = pos_.ToVector2();

	// 相対座標を足す
	ret.x += hitPos_.x;
	ret.y += hitPos_.y;

	// 左右
	switch (lr)
	{
	case EnemyBase::COL_LR::L:
		ret.x -= hitBox_.x;
		break;

	case EnemyBase::COL_LR::R:
		ret.x += hitBox_.x;
		break;
	}
	// 上下
	switch (td)
	{
	case EnemyBase::COL_TD::T:
		ret.y -= hitBox_.y;
		break;

	case EnemyBase::COL_TD::D:
		ret.y += hitBox_.y;
		break;
	}
	return ret;
}

void EnemyBase::ChengeIsFirst()
{
	isFirst_ = false;
}

