#include "SceneGame.h"
#include "EnemyMob1.h"

void EnemyMob1::SetParam(void)
{
	//画像のファイル名
	imgFileName = "Mob2.png";

	//サイズの設定
	size_ = { 150,150 };

	//アニメーション画像数
	ImgNumX_ = 2;
	ImgNumY_ = 1;

	isRight_ = true;

	hitPos_ = { 0,0 };
	hitBox_ = { 15,12 };

	//HP
	hp_ = 1;

	//移動速度
	speed_ = 2.0f;

	//アニメーション速度
	speedAnim_ = 0.2f;
}

void EnemyMob1::KnockBack(void)
{
	//プレイヤーの座標を取得
	Vector2F playerPos = sceneGame_->GetPlayerPos();
	pos_ = { static_cast<float>(playerPos.x) ,static_cast<float>(playerPos.y - 100.0f) };
}

void EnemyMob1::Move(void)
{
	//プレイヤーの座標を取得
	Vector2F playerPos = sceneGame_->GetPlayerPos();

	//敵とプレイヤーの位置ベクトルを作成
	//プレイヤーの座標から敵の座標を引く
	Vector2F posE2P = Vector2F(
		static_cast<float>(playerPos.x - pos_.x)
		, static_cast<float>(playerPos.y - pos_.y)
	);

	//位置ベクトルを正規化して方向ベクトルにする
	//posE2P→direction
	//大きさを計算 (ルートをとる関数→sqrt)
	float size = sqrtf(posE2P.x * posE2P.x + posE2P.y * posE2P.y);

	//敵の移動処理
	if (size < SPEED_DEFAULT)
	{
		//移動量よりも位置差が短い場合はプレイヤーに重なる
		pos_ = { static_cast<float>(playerPos.x) ,static_cast<float>(playerPos.y) };
	}
	else
	{
		//普通に移動処理を行う

		//正規化 位置ベクトルを大きさで割る
		Vector2F direction = Vector2F(posE2P.x / size, posE2P.y / size);

		pos_.x += static_cast<float>(direction.x * SPEED_DEFAULT);
		pos_.y += static_cast<float>(direction.y * SPEED_DEFAULT);

		//向き画像を決める
		//水平か鉛直を選択
		//数値を絶対値(abs関数)としてみる
		if (abs(direction.x) < abs(direction.y))
		{
			//鉛直の向き(UP or DOWN)
			if (direction.y < 0.0F)
			{
				dir_ = AsoUtility::DIR::UP;

			}
			else
			{
				dir_ = AsoUtility::DIR::DOWN;
			}
		}
		else
		{
			//水平の向き(LEFT or RIGHT)
			if (direction.x < 0.0F)
			{
				dir_ = AsoUtility::DIR::LEFT;

			}
			else
			{
				dir_ = AsoUtility::DIR::RIGHT;
			}
		}
	}
}
