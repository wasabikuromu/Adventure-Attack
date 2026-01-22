#pragma once

//#include <vector>
//#include <map>
#include "Vector2F.h"
#include "AsoUtility.h"

//前方宣言
class SceneGame;

class Player
{
public:
	//プレイヤーのサイズ
	static const int SIZE_X = 100;			//横サイズ
	static const int SIZE_Y = 55;			//縦サイズ

	//アニメーションの最大数
	static const int MAX_NUM_ANIM = 10;

	//Atkアニメーション数
	static const int ATK_NUM_ANIM = 6;

	//Idleアニメーション数
	static const int IDLE_NUM_ANIM = 8;

	//Jumpアニメーション数
	static const int JUMP_NUM_ANIM = 6;

	//Runアニメーション数
	static const int RUN_NUM_ANIM = 10;


	//アニメーションスピード
	static constexpr float ANIM_SPEED = 0.2f;

	//加速度(accelerator ＝ アクセラレータ)
	static constexpr float MOVE_ACC = 0.25f;

	//減衰(decelerate)
	static constexpr float MOVE_DEC = 0.1f;

	//速度制限(移動速度の最大)
	static constexpr float MAX_MOVE_SPEED = 4.0f;

	//最大ジャンプ力(初速度)
	static constexpr float MAX_JUMP_POWER = 9.0f;

	//重力加速度
	static constexpr float GRAVITY = 0.25f;

	// 衝突座標(左右)
	enum class COL_LR
	{
		C,
		L,
		R
	};
	// 衝突座標(上下)
	enum class COL_TD
	{
		C,
		T,
		D
	};

	// 状態
	enum class STATE
	{
		NONE
		, PLAY
		, ATK
		, DAMAGED
	};

	// アニメーション状態
	enum class ANIM_STATE
	{
		IDLE
		,RUN
		,JUMP
		,ATK
		,MAX
	};

	// 攻撃状態
	enum class ATK_STATE
	{
		NONE
		,ATK
		,MAX
	};

	//メンバー関数
	//-------------------------------------
	Player(void);			//デフォルトコンストラクタ
	virtual ~Player(void);	//デストラクタ

	bool Init(SceneGame* parent);	//初期化処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	bool Release(void);	//解放処理

	void Damage(int damage);

	Vector2F GetPos(void);			//座標を返す
	void SetPos(Vector2F pos);		//座標を設定する

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 GetHitPos(void);

	// 衝突判定用：範囲
	Vector2 GetHitBox(void);

	// ATk衝突判定用：中心座標(pos_からの相対座標)
	Vector2 GetRAtkHitPos(void);

	// ATk衝突判定用：中心座標(pos_からの相対座標)
	Vector2 GetLAtkHitPos(void);

	// ATK衝突判定用：範囲
	Vector2 GetAtkHitBox(void);

	STATE GetState(void);
	ANIM_STATE GetAnimState(void);

private:
	//メンバー変数 
	//--------------------------------------

	STATE state_;

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 hitPos_;

	// 衝突判定用：範囲
	Vector2 hitBox_;

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 rAtkHitPos_;

	Vector2 lAtkHitPos_;

	// 衝突判定用：範囲
	Vector2 atkHitBox_;

	//サウンド格納
	int soundJump_;
	int soundAtk_;

	//SceneGameのポインタを格納する領域
	SceneGame* sceneGame_;
	
	//状態ごとの画像(モーション、攻撃、アニメーション数)
	//int images_[4][2][4];
	int images_[static_cast<int>(ANIM_STATE::MAX)][static_cast<int>(ATK_STATE::MAX)][MAX_NUM_ANIM];

	//体力
	int hp_;

	int lifeImage_;
	int lifeImage1_;

	//座標
	Vector2F pos_;

	//アニメーション状態
	ANIM_STATE animState_;

	//攻撃状態
	ATK_STATE atkState_;

	//プレイヤーの向き
	AsoUtility::DIR dir_;

	int animIdx_;

	//移動速度
	float moveSpeed_;

	//アニメーションカウンタ
	float stepAnim_;

	//ジャンプ力
	float jumpPower_;

	//ジャンプ中フラグ(true : ジャンプ中)
	bool isJump_;

	// Debug
	Vector2 leftC_;
	Vector2 leftT_;
	Vector2 leftD_;
	Vector2 righC_;
	Vector2 righT_;
	Vector2 righD_;
	Vector2 footC_;
	Vector2 footL_;
	Vector2 footR_;

	int damagedCnt_;

	//メンバー関数
	//------------------------------------------
	//画像読み込み
	bool LoadImages(void);

	//水平方向
	//---------------------------------------
	//操作制御
	void ProcessMove(void);

	//速度制御
	void Accele(float speed);

	//減衰制御
	void Decelerate(float speed);

	//移動制御
	void Move(void);

	//鉛直方向(ジャンプ)
	//---------------------------------------
	//操作制御(ジャンプ)
	void ProcessJump(void);

	//速度制御(ジャンプ)
	void SetJumpPower(float power);

	//減衰処理(ジャンプ : 重力加速度)
	void AddGravity(void);

	//移動制御
	void Jump(void);

	Vector2 GetColPos(COL_LR lr, COL_TD td);

	//右側の当たり判定
	void CollisionRight(void);

	//左側の当たり判定
	void CollisionLeft(void);

	//当たり判定の足
	void CollisionFoot(void);

	//当たり判定の頭
	void CollisionHead(void);

	//プレイヤーの描画
	void DrawPlayer(int graphID);

	void ChangeState(STATE state);
	void UpdatePlay(void);	//更新処理
	void UpdateAttack(void); // 更新処理
	void UpdateDamaged(void);	//更新処理
};
