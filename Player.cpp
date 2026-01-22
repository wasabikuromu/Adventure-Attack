#include <DxLib.h>
#include "Vector2.h"
#include "Vector2F.h"
#include "AsoUtility.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Stage.h"
#include "SceneGame.h"
#include "Player.h"

//デフォルトコンストラクタ
Player::Player(void)
{

}

//デストラクタ
Player::~Player(void)
{

}

//初期化処理
bool Player::Init(SceneGame* parent)
{
	sceneGame_ = parent;

	//画像読み込み
	LoadImages();

	//アニメションの状態
	animState_ = ANIM_STATE::IDLE;

	//攻撃の状態
	atkState_ = ATK_STATE::NONE;

	//向き
	dir_ = AsoUtility::DIR::RIGHT;

	//移動速度
	moveSpeed_ = 0.0f;

	//アニメーションカウンタ
	stepAnim_ = 0;

	hp_ = 15;

	lifeImage_ = LoadGraph("Image/UI/Life.png");
	if (lifeImage_ == -1)
	{
		return -1;
	}

	lifeImage1_ = LoadGraph("Image/UI/Life1.png");
	if (lifeImage1_ == -1)
	{
		return -1;
	}

	//表示場所の設定
	pos_ = { 150.0f,648.0f };

	//ジャンプ力
	jumpPower_ = 0.0f;

	soundJump_ = LoadSoundMem("Image/Sound/Jump.mp3");		//発射音を鳴らす処理
	soundAtk_ = LoadSoundMem("Image/Sound/Attack.mp3");		//攻撃音を鳴らす処理

	//ジャンプ判定
	isJump_ = false;

	// 衝突判定用：中心座標(pos_からの相対座標)
	hitPos_ = { 0, 4 };

	// 衝突判定用：範囲(矩形の半径)
	hitBox_ = { 14, 19 };

	rAtkHitPos_ = { 30,0 };

	lAtkHitPos_ = { -30,0 };

	atkHitBox_ = { 20,10 };
	
	ChangeState(STATE::PLAY);

	return true;
}

//更新処理
void Player::Update(void)
{

	switch (state_)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::ATK:
		UpdateAttack();
		break;
	case Player::STATE::DAMAGED:
		UpdateDamaged();
		break;
	default:
		break;
	}
}

//描画処理
void Player::Draw(void)
{
	Vector2 cameraPos = sceneGame_->GetCameraPos();

	//裏HP
	for (int pl = 0; pl < 15; pl++)
	{
		DrawGraph(25 + (pl % 15) * 60, 25, lifeImage1_, true);
	}

	//ライフの描画
	for (int pl = 0; pl < (hp_); pl++)
	{
		DrawGraph(25 + (pl % 15) * 60, 25, lifeImage_, true);
	}

	//座標をint型にする
	Vector2 pos = pos_.ToVector2();

	//アニメーション状態
	if (isJump_ == true)
	{
		animState_ = ANIM_STATE::JUMP;
	}
	if (state_ == STATE::ATK)
	{
		animState_ = ANIM_STATE::ATK;
	}

	int animState = static_cast<int>(animState_);
	
	//攻撃状態
	int atkState = static_cast<int>(atkState_);

	stepAnim_ += ANIM_SPEED;
	 
	switch (animState_)
	{
	case ANIM_STATE::IDLE:
		animIdx_ = AsoUtility::Round(stepAnim_) % IDLE_NUM_ANIM;
		DrawPlayer(images_[animState][atkState][animIdx_]);
		break;

	case ANIM_STATE::RUN:
		animIdx_ = AsoUtility::Round(stepAnim_) % RUN_NUM_ANIM;
		DrawPlayer(images_[animState][atkState][animIdx_]);
		break;

	case ANIM_STATE::JUMP:
		animIdx_ = AsoUtility::Round(stepAnim_) % JUMP_NUM_ANIM;
		DrawPlayer(images_[animState][atkState][0]);
		break;

	case ANIM_STATE::ATK:
		animIdx_ = AsoUtility::Round(stepAnim_ * 0.8f) % ATK_NUM_ANIM;
		DrawPlayer(images_[animState][atkState][animIdx_]);
		break;

	}

	//DrawFormatString(0, 0, 0x0, "speed = %f", moveSpeed_);

	//DrawFormatString(0, 16, 0x0, "jump = %f", jumpPower_);

	/*// Debug
	int s = 3;
	int c = 0x00ff00;
	int d = 0x0022ff;

	pos = footC_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = footL_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = footR_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = righC_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = righT_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = righD_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = leftC_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = leftT_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

	pos = leftD_;
	DrawBox(pos.x - s, pos.y - s, pos.x + s, pos.y + s, c, true);

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
	DrawBox(sPos.x, sPos.y, ePos.x, ePos.y, 0xff0000, false);
 
	int size = 5;
	auto fPos = footC_;
	//fPos.x += footC_.x;
	//fPos.y += footC_.y;
	fPos.x -= cameraPos.x;
	fPos.y -= cameraPos.y;
	DrawBox(fPos.x - size, fPos.y - size, fPos.x + size, fPos.y + size, 0x00ff00, true);
	auto pPos = pos_;
	pPos.x -= cameraPos.x;
	pPos.y -= cameraPos.y;
	DrawBox(pPos.x - size, pPos.y - size, pPos.x + size, pPos.y + size, 0xffffff, true);

	Vector2F aPos = pos_;
	aPos.x += rAtkHitPos_.x;
	aPos.y += rAtkHitPos_.y;

	Vector2F kPos = aPos;
	kPos.x -= atkHitBox_.x;
	kPos.y -= atkHitBox_.y;
	Vector2F lPos = aPos;
	lPos.x += atkHitBox_.x;
	lPos.y += atkHitBox_.y;

	kPos.x -= cameraPos.x;
	kPos.y -= cameraPos.y;
	lPos.x -= cameraPos.x;
	lPos.y -= cameraPos.y;
	DrawBox(kPos.x, kPos.y, lPos.x, lPos.y, 0xff0000, false);*/

}

//解放処理
bool Player::Release(void)
{
	DeleteGraph(lifeImage_);
	DeleteGraph(lifeImage1_);

	for (int anim = 0; anim < RUN_NUM_ANIM; anim++)
	{
		//走るの解放
		DeleteGraph(images_[2][0][0]);
	}

	//ジャンプの解放
	DeleteGraph(images_[static_cast<int>(ANIM_STATE::JUMP)][0][0]);

	//ATKの解放
	DeleteGraph(images_[static_cast<int>(ANIM_STATE::ATK)][1][0]);

	//IDLEの解放　
	DeleteGraph(images_[static_cast<int>(ANIM_STATE::IDLE)][0][0]);

	DeleteSoundMem(soundAtk_);
	DeleteSoundMem(soundJump_);

	return true;
}

void Player::Damage(int damage)
{

	hp_ -= damage;
	ChangeState(STATE::DAMAGED);

	if (hp_ <= 0)
	{
		hp_ = 0;
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAMEOVER
			, true);
	}
}

Vector2F Player::GetPos(void)
{
	return pos_;
}

void Player::SetPos(Vector2F pos)
{
	pos_ = pos;
}

Vector2 Player::GetHitPos(void)
{
	return hitPos_;
}

Vector2 Player::GetHitBox(void)
{
	return hitBox_;
}

Vector2 Player::GetRAtkHitPos(void)
{
	return rAtkHitPos_;
}

Vector2 Player::GetLAtkHitPos(void)
{
	return lAtkHitPos_;
}

Vector2 Player::GetAtkHitBox(void)
{
	return atkHitBox_;
}

Player::STATE Player::GetState(void)
{
	return state_;
}

Player::ANIM_STATE Player::GetAnimState(void)
{
	return animState_;
}

//画像読み込み
bool Player::LoadImages(void)
{
	//配列の初期化
	for (int i = 0; i < static_cast<int>(ANIM_STATE::MAX); i++)
	{
		for (int j = 0; j < static_cast<int>(ATK_STATE::MAX); j++)
		{
			for (int k = 0; k < MAX_NUM_ANIM; k++)
			{
				images_[i][j][k] = -1;
			}
		}
	}

	int ret;

	//Idle
	ret = LoadDivGraph("Image/Player/Idle.png"
		, IDLE_NUM_ANIM
		, IDLE_NUM_ANIM
		, 1
		, SIZE_X
		, SIZE_Y
		, &(images_[static_cast<int>(ANIM_STATE::IDLE)][0][0])
	);
	if (ret == -1)
	{
		OutputDebugString("Player Idie.png の読み込み失敗");
		return false;
	}

	//RUN
	ret = LoadDivGraph("Image/Player/Run.png"
		, RUN_NUM_ANIM
		, RUN_NUM_ANIM
		, 1
		, SIZE_X
		, SIZE_Y
		, &(images_[static_cast<int>(ANIM_STATE::RUN)][0][0])
	);
	if (ret == -1)
	{
		OutputDebugString("Player Run.png の読み込み失敗");
		return false;
	}

	//ATK
	ret = LoadDivGraph("Image/Player/Attack.png"
		, ATK_NUM_ANIM
		, ATK_NUM_ANIM
		, 1
		, SIZE_X
		, SIZE_Y
		, &(images_[static_cast<int>(ANIM_STATE::ATK)][0][0])
	);
	if (ret == -1)
	{
		OutputDebugString("Player Attack.png の読み込み失敗");
		return false;
	}

	//JUMP
	ret = LoadDivGraph("Image/Player/Jump.png"
		, JUMP_NUM_ANIM
		, JUMP_NUM_ANIM
		, 1
		, SIZE_X
		, SIZE_Y
		, &(images_[static_cast<int>(ANIM_STATE::JUMP)][0][0])
	);
	if (ret == -1)
	{
		OutputDebugString("Player jump.pngの読み込み失敗");
		return false;
	}
}

//操作制御
void Player::ProcessMove(void)
{
	//向き
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		dir_ = AsoUtility::DIR::RIGHT;
		animState_ = ANIM_STATE::RUN;

		Accele(MOVE_ACC);
	}

	else if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		dir_ = AsoUtility::DIR::LEFT;
		animState_ = ANIM_STATE::RUN;

		Accele(-MOVE_ACC);
	}
	
}

//速度制御
void Player::Accele(float speed)
{
	moveSpeed_ +=speed;

	//右速度の速度制限
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	//左速度の速度制限
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}


//減衰制御
void Player::Decelerate(float speed)
{
	//移動方向(右)
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
	
	//移動方向(左)
	else if (moveSpeed_ < 0.0f)
	{
		moveSpeed_ += speed;
		if (moveSpeed_ > 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
}

//移動制御
void Player::Move(void)
{
	pos_.x += moveSpeed_;

	if (pos_.x < 0)
	{
		pos_.x = 0;
	}

	if (pos_.x > 4800)
	{
		pos_.x = 4800;
	}

	//右側の当たり判定
	CollisionRight();

	//左側の当たり判定
	CollisionLeft();
}

//操作制御(ジャンプ)
void Player::ProcessJump(void)
{
	//ジャンプできるのはジャンプをしていないときだけ
	if (isJump_ == false)
	{
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)
		{
			PlaySoundMem(soundJump_, DX_PLAYTYPE_BACK, true);		//発射音を再生

			SetJumpPower(-MAX_JUMP_POWER);
			isJump_ = true;
		}
	}
}

//速度制御(ジャンプ)
void Player::SetJumpPower(float power)
{
	jumpPower_ = power;

	//重力がかかりすぎるのを制限する
	if (jumpPower_ > MAX_JUMP_POWER)
	{
		jumpPower_ = MAX_JUMP_POWER;
	}
}

//減衰処理(ジャンプ : 重力加速度)
void Player::AddGravity(void)
{
	//重力による減衰
	SetJumpPower(jumpPower_ + GRAVITY);
}

//移動制御
void Player::Jump(void)
{
	//移動処理
	pos_.y += jumpPower_;

	//足元の当たり判定
	CollisionFoot();

	//頭の当たり判定
	CollisionHead();
}

Vector2 Player::GetColPos(COL_LR lr, COL_TD td)
{
	Vector2 ret = pos_.ToVector2();

	// 相対座標を足す
	ret.x += hitPos_.x;
	ret.y += hitPos_.y;

	// 左右
	switch (lr)
	{
	case Player::COL_LR::L:
		ret.x -= hitBox_.x;
		break;

	case Player::COL_LR::R:
		ret.x += hitBox_.x;
		break;
	}
	// 上下
	switch (td)
	{
	case Player::COL_TD::T:
		ret.y -= hitBox_.y;
		break;

	case Player::COL_TD::D:
		ret.y += hitBox_.y;
		break;
	}
	return ret;
}

//右側の当たり判定
void Player::CollisionRight(void)
{
	//右の中心座標
	Vector2 rightPosC = GetColPos(COL_LR::R, COL_TD::C);
	//righC_ = rightPosC;

	//右の上座標
	Vector2 rightPosT = GetColPos(COL_LR::R, COL_TD::T);
	//righT_ = rightPosT;

	//右の下座標
	Vector2 rightPosD = GetColPos(COL_LR::R, COL_TD::D);
	//righD_ = rightPosD;
	

	//当たり判定
	if (sceneGame_->IsCollisionStage(rightPosC)
		|| sceneGame_->IsCollisionStage(rightPosT)
		|| sceneGame_->IsCollisionStage(rightPosD))
	{
		//自分の座標からマップの配列位置を計算する
		Vector2 mapPos = sceneGame_->WorldPos2MapPos(rightPosC);

		//壁の左上からプレイヤーの座標を計算する
		//pos_.x = static_cast<float>(mapPos.x * Stage::CHIP_SIZE_X - 32 / 2 - 1);
		pos_.x = static_cast<float>(mapPos.x * Stage::CHIP_SIZE_X  - hitBox_.x - 1);
	}
}

//左側の当たり判定
void Player::CollisionLeft(void)
{
	//左の中心座標
	Vector2 leftPosC = GetColPos(COL_LR::L, COL_TD::C);
	//leftC_ = leftPosC;

	//左の上座標
	Vector2 leftPosT = GetColPos(COL_LR::L, COL_TD::T);
	//leftT_ = leftPosT;

	//左の下座標
	Vector2 leftPosD = GetColPos(COL_LR::L, COL_TD::D);
	//leftD_ = leftPosD;

	//当たり判定
	if (sceneGame_->IsCollisionStage(leftPosC)
		|| sceneGame_->IsCollisionStage(leftPosT)
		|| sceneGame_->IsCollisionStage(leftPosD))
	{
		//自分の座標からマップの配列位置を計算する
		Vector2 mapPos = sceneGame_->WorldPos2MapPos(leftPosC);

		//壁の左上からプレイヤーの座標を計算する
		//pos_.x = static_cast<float>((mapPos.x + 1) * Stage::CHIP_SIZE_X + (32 / 2) + 1);
		pos_.x = static_cast<float>((mapPos.x + 1) * Stage::CHIP_SIZE_X + hitBox_.x + 1);
	}
}

//当たり判定
void Player::CollisionFoot(void)
{
	//足元の中心座標
	Vector2 footPosC = GetColPos(COL_LR::C, COL_TD::D);
	footC_ = footPosC;

	//足元の左座標
	Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);
	footL_ = footPosL;

	//足元の右座標
	Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);
	footR_ = footPosR;

	if (sceneGame_->IsCollisionStage(footPosC)
		|| sceneGame_->IsCollisionStage(footPosL)
		|| sceneGame_->IsCollisionStage(footPosR))
	{
		//自分の座標からマップの配列位置を計算する

		Vector2 mapPos = sceneGame_->WorldPos2MapPos(footPosC);
		pos_.y = static_cast<float>(mapPos.y * Stage::CHIP_SIZE_Y - 1 - hitBox_.y - hitPos_.y);

		//地面に接地したのでジャンプを初期化
		isJump_ = false;
		SetJumpPower(0.0f);
	}
}

void Player::CollisionHead(void)
{
	//頭の中心座標
	Vector2 headPosC = GetColPos(COL_LR::C, COL_TD::T);

	//頭の左座標
	Vector2 headPosL = GetColPos(COL_LR::L, COL_TD::T);

	//頭の右座標
	Vector2 headPosR = GetColPos(COL_LR::R, COL_TD::T);

	if (sceneGame_->IsCollisionStage(headPosC)
		|| sceneGame_->IsCollisionStage(headPosL)
		|| sceneGame_->IsCollisionStage(headPosR))
	{
		//自分の座標からマップの配列位置を計算する
		Vector2 mapPos = sceneGame_->WorldPos2MapPos(headPosC);

		pos_.y = static_cast<float>((mapPos.y + 1) * Stage::CHIP_SIZE_Y + 1 + hitBox_.y + hitPos_.y);
		jumpPower_ = 0.0f;
	}
}

//プレイヤーの描画
//引数：描画する画像
void Player::DrawPlayer(int graphID)
{
	//座標を float から int に変換する
	Vector2 pos = pos_.ToVector2();

	//向きの決定
	bool isLeftDir = true;			//左向きの変数(true : 左向き)
	if (dir_ == AsoUtility::DIR::RIGHT)
	{
		isLeftDir = false;
	}

	Vector2 cameraPos = sceneGame_->GetCameraPos();
	
	if (damagedCnt_ > 0)
	{
		int cnt = damagedCnt_ % 8;
		if (cnt == 0 || cnt == 1)
		{
			//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			SetDrawAddColor(255, 255, 255);
		}
	}

	//描画処理
	DrawRotaGraph(
		//pos.x - cameraPos.x, pos.y - cameraPos.y + 32 //描画場所(中央位置)
		pos.x - cameraPos.x, pos.y - cameraPos.y //描画場所(中央位置)
		, 1.0			//拡大率
		, 0.0			//回転角度
		, graphID		//画像ID
		, true			//透過
		, isLeftDir		//横反転X
		, false			//縦反転Y
		);

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawAddColor(0, 0, 0);

}

void Player::ChangeState(STATE state)
{

	state_ = state;
	switch (state_)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::ATK:
		stepAnim_ = 0.0f;
		animState_ = ANIM_STATE::ATK;
		break;
	case Player::STATE::DAMAGED:
		damagedCnt_ = 0;
		break;
	default:
		break;
	}

}

void Player::UpdatePlay(void)
{
	//基本的には操作がないときにはアイドル
	animState_ = ANIM_STATE::IDLE;

	//水平移動
	//-------------------------------
	//操作制御
	ProcessMove();

	//減衰制御
	Decelerate(MOVE_DEC);

	//移動制御
	Move();

	//鉛直移動
	//--------------------------------
	//操作制御(ジャンプ)
	ProcessJump();

	//減衰制御
	AddGravity();

	//移動制御
	Jump();

	//攻撃
	atkState_ = ATK_STATE::NONE;			//基本は攻撃してない

	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_V))
	{
		ChangeVolumeSoundMem(255 * 60 / 100, soundAtk_);		//音量の設定
		PlaySoundMem(soundAtk_, DX_PLAYTYPE_BACK, true);		//発射音を再生
		ChangeState(STATE::ATK);
	}
}

void Player::UpdateAttack(void)
{
	//水平移動
	//-------------------------------
	// 
	ProcessMove();
	//減衰制御
	Decelerate(MOVE_DEC);

	//移動制御
	Move();

	//鉛直移動
	//--------------------------------
	//操作制御(ジャンプ)
	ProcessJump();

	//減衰制御
	AddGravity();

	//移動制御
	Jump();

	if (stepAnim_ > 1.0f / 0.8f * 5.0f) 
	{ 
		ChangeState(STATE::PLAY);
	}
}

void Player::UpdateDamaged(void)
{
	damagedCnt_++;
	if (damagedCnt_ > 60)
	{
		damagedCnt_ = 0;
		ChangeState(STATE::PLAY);
	}

	//水平移動
	//-------------------------------
	//操作制御
	ProcessMove();

	//減衰制御
	Decelerate(MOVE_DEC);

	//移動制御
	Move();

	//鉛直移動
//	--------------------------------
	//操作制御(ジャンプ)
	ProcessJump();

	//減衰制御
	AddGravity();

	//移動制御
	Jump();
}
