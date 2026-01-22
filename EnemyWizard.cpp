#include "SceneGame.h"
#include "Shot.h"
#include "Player.h"
#include "EnemyWizard.h"

void EnemyWizard::Draw(void)
{
	EnemyBase::Draw();
	shot_->Draw();
}

void EnemyWizard::Update(void)
{
	EnemyBase::Update();
	//敵の弾とプレイヤーの当たり判定
	float diff = hypot(sceneGame_->GetPlayerPos().x - shot_->GetPos().x, sceneGame_->GetPlayerPos().y - shot_->GetPos().y);
	if (diff < Shot::SHOT_SIZE_X + sceneGame_->GetPlayerHitBox().x && shot_->GetState() == Shot::STATE::SHOT )
	{
		sceneGame_->PlayerHit();
	}

}

bool EnemyWizard::Release(void)
{
	EnemyBase::Release();
	shot_->Release();
	delete shot_;
	return false;
}

void EnemyWizard::SetParam(void)
{
	//画像のファイル名
	imgFileName = "WizardIdle.png";

	//サイズの設定
	size_ = { 80,80 };

	//アニメーション画像数
	ImgNumX_ = 10;
	ImgNumY_ = 1;

	isRight_ = false;

	hitPos_ = { 2,0 };
	hitBox_ = { 18 ,27 };

	//HP
	hp_ = 3;

	//移動速度
	speed_ = 2.0f;

	//アニメーション速度
	speedAnim_ = 0.2f;
	shot_ = new Shot();
	shot_->Init(sceneGame_);
}

void EnemyWizard::Hit(int value)
{
	EnemyBase::Hit(value);
	if (hp_ <= 0)
	{
		shot_->ChangeState(Shot::STATE::END);
	}
}

void EnemyWizard::KnockBack(void)
{
}

void EnemyWizard::Move(void)
{
	//プレイヤーの座標を取得
	Vector2F playerPos = sceneGame_->GetPlayerPos();

	//敵とプレイヤーの位置ベクトルを作成
	//プレイヤーの座標から敵の座標を引く
	Vector2F posE2P = Vector2F(
		static_cast<float>(playerPos.x - pos_.x)
		, static_cast<float>(playerPos.y - pos_.y)
	);
	if (isAlive)
	{
		shot_->Create(pos_);
		shot_->Update();
	}
	else
	{
		shot_->ChangeState(Shot::STATE::END);
	}
}