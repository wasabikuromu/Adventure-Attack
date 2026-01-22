#include "SceneGame.h"
#include "EnemyBoss.h"
#include "ShotBoss.h"

void EnemyBoss::Draw(void)
{
	EnemyBase::Draw();
	shot_->Draw();
}

void EnemyBoss::Update(void)
{
	EnemyBase::Update();
	//敵の弾とプレイヤーの当たり判定
	float diff = hypot(sceneGame_->GetPlayerPos().x - shot_->GetPos().x, sceneGame_->GetPlayerPos().y - shot_->GetPos().y);
	if (diff < Shot::SHOT_SIZE_X + sceneGame_->GetPlayerHitBox().x && shot_->GetState() == Shot::STATE::SHOT)
	{
		sceneGame_->PlayerHit();
	}
}

bool EnemyBoss::Release(void)
{
	EnemyBase::Release();
	shot_->Release();
	delete shot_;
	return false;
}

void EnemyBoss::SetParam(void)
{
	//画像のファイル名
	imgFileName = "BossIdle.png";

	//サイズの設定
	size_ = { 100,100 };

	//アニメーション画像数
	ImgNumX_ = 4;
	ImgNumY_ = 1;

	isRight_ = true;

	hitPos_ = { 0,0 };
	hitBox_ = { 25,22 };

	//HP
	hp_ = 5;

	//移動速度
	speed_ = 0.0f;

	//アニメーション速度
	speedAnim_ = 0.1f;
	shot_ = new ShotBoss();
	shot_->Init(sceneGame_);
}

void EnemyBoss::Hit(int value)
{
	EnemyBase::Hit(value);
	if (hp_ <= 0)
	{
		shot_->ChangeState(Shot::STATE::END);
	}
}

void EnemyBoss::Move(void)
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
    
