#include <DxLib.h>
#include "Vector2F.h"
#include "AsoUtility.h"
#include "SceneGame.h"
#include "Shot.h"

//デフォルトコンストラクタ
Shot::Shot(void)
{
	//メンバー変数の初期化

	sceneGame_ = nullptr;

	//配列はいったん置く
	//imgs_[SHOT_ANIM_NUM]

	pos_ = { 0.0f,0.0f };

	dir_ = { 0.0f,0.0f };

	speed_ = 0.0f;

	ChangeState(STATE::NONE);

	cntAlive_ = 0;

	cntAnim_ = 0;

	speedAnim_ = 0.0f;

	idxAnim_ = 0;
}

//デストラクタ
Shot::~Shot(void)
{

}

//初期化処理(初回の１度のみ実行される)
bool Shot::Init(SceneGame* parent)
{
	//メンバー変数の初期値を設定
	sceneGame_ = parent;

	ChangeState(STATE::NONE);

	//弾画像の読み込み
	int ret;
	ret = LoadDivGraph(
		"Image/Effect/shot.png"
		, SHOT_ANIM_NUM
		, SHOT_ANIM_NUM
		, 1
		, SHOT_SIZE_X
		, SHOT_SIZE_Y
		, &imgs_[0]
	);
	if (ret == -1)
	{
		OutputDebugString("弾画像の読み込み失敗");
		return false;
	}

	//いったん画面中央
	pos_ = { 0.0f,0.0f };

	//下向き
	dir_ = { 0.0f,1.0f };

	//敵の移動速度
	speed_ = 2.5f;

	cntAlive_ = 0;

	cntAnim_ = 0;

	speedAnim_ = 0.1f;

	idxAnim_ = 0;

	return true;
}

//更新処理(毎フレーム実行)
void Shot::Update()
{
	//弾が発射している時だけ移動しましょう
	if (state_ == STATE::SHOT)
	{
		UpdateShot();
	}
}
void Shot::UpdateShot(void)
{
	cntAnim_++;
	cntAlive_++;

	//弾の移動・アニメーション処理
	pos_.x += dir_.x * speed_;
	pos_.y += dir_.y * speed_;

	//弾が生存限界に達した場合は弾を終了させる
	if (cntAlive_ > SHOT_CNT_ALIVE)
	{
		ChangeState(STATE::END);
	}
	
}

//描画処理(毎フレーム実行)
void Shot::Draw(void)
{
	//弾の描画
	if (state_ == STATE::SHOT)
	{
		DrawShot();
	}
}

void Shot::DrawShot(void)
{
	Vector2 cameraPos = sceneGame_->GetCameraPos();

	int idxAnim_ = static_cast<int>
		(static_cast<float>(cntAnim_) * speedAnim_) % SHOT_ANIM_NUM;

	//弾の描画
	DrawRotaGraph(pos_.x - cameraPos.x
		, pos_.y - cameraPos.y
		, 1.0f			//拡大
		, 0.0f			//回転
		, imgs_[idxAnim_]
		, true
		, false);
}

//解放処理(終了時の一度のみ実行)
bool Shot::Release(void)
{
	//弾の解放処理(画像の解放)
	for (int i = 0; i < SHOT_ANIM_NUM; i++)
	{
		DeleteGraph(imgs_[i]);
	}

	return true;
}

//弾発射
void Shot::Create(Vector2F pos)
{
	//発射条件があっている時のみ発射可能
	//発射してはいけないのは
	if (state_ == STATE::SHOT)
	{
		//発射不可
		return;
 	}
	
	//弾発射可能
	//--------------------------------
	
	//弾の初期位置の設定
	pos_ = pos;

	//弾の移動方向の設定
	dir_ = { sceneGame_->GetPlayerPos().x - pos.x,sceneGame_->GetPlayerPos().y - pos.y};
	dir_ = {static_cast<float>( dir_.x / hypot(dir_.x,dir_.y)),static_cast<float>(dir_.y / hypot(dir_.x,dir_.y)) };

	//弾を発射したことにする
	ChangeState(STATE::SHOT);
}

//弾の生成が可能かどうか判定(NONE or END)
bool Shot::IsEnableCreate(void)
{
	if (state_ == STATE::NONE || state_ == STATE::END)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//状態を変更する
void Shot::ChangeState(STATE value)
{
	state_ = value;

	if (state_ == STATE::SHOT)
	{
		cntAlive_ = 0;		//今から生存開始
		cntAnim_ = 0;
		speedAnim_ = 0.1f;
	}
}

//座標を返す
Vector2F Shot::GetPos(void)
{
	return pos_;
}

//現在の状態を返す
Shot::STATE Shot::GetState(void)
{
	return state_;
}
