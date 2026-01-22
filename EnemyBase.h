#pragma once
#include "Vector2.h"
#include "Vector2F.h"
#include "AsoUtility.h"

//前方宣言
class SceneGame;

class EnemyBase
{
public:

	//アニメーションの最大数
	static constexpr int MAX_NUM_ANIM = 14;

	//移動速度
	static constexpr int SPEED_DEFAULT = 1;

	//敵の種別
	enum class TYPE
	{
		BOSS			//ボス
		, MOB1			//雑魚敵1
		, WIZARD		//魔法使い
		, GIMP			//ギンプ
		, MAX
	};

	//メンバー関数
	//----------------------------------

	EnemyBase(void);//デフォルトコンストラクタ
	~EnemyBase(void);//デストラクタ

	//初期化処理(初回の１度のみ実行される)
	bool Init(SceneGame* parent);

	//更新処理(毎フレーム実行)
	virtual void Update(void);

	//描画処理(毎フレーム実行)
	virtual void Draw(void);

	//解放処理(終了時の一度のみ実行)
	virtual bool Release(void);

	virtual void SetParam(void) = 0;	//固有の機能を設定する

	void SetPos(Vector2F value);		//座標を設定する
	Vector2F GetPos(void);				//座標を返す

	void SetStartPos(Vector2 value);

	Vector2 GetSize(void);				//サイズを返す

	bool IsAlive(void);					//敵の生存状態を返す

	virtual void Hit(int value);				//ダメージを与える関数

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 GetHitPos(void);

	// 衝突判定用：範囲
	Vector2 GetHitBox(void);

	virtual void KnockBack(void);
	void ChengeIsFirst();

protected:

	//SceneGameのポインタを格納する領域
	SceneGame* sceneGame_;

	//画像のファイル名
	std::string imgFileName;

	//表示座標
	Vector2F pos_;

	//サイズ
	Vector2 size_;

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 hitPos_;

	// 衝突判定用：範囲
	Vector2 hitBox_;

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

	//各キャラクターの
	int ImgNumX_;
	int ImgNumY_;

	//各キャラクターのアニメーション数
	int IdleNum_;
	int AtkNum_;
	int DieNum_;
	
	// 画像の向きflag
	bool isRight_;

	//移動速度
	int speed_;

	//アニメーション速度
	float speedAnim_;

	//HP
	int hp_;

	//向き
	AsoUtility::DIR dir_;

	bool isAlive;
	//弾の速度
	float shotSpeed_;

	//更新処理(毎フレーム実行)
	virtual void Move(void);

	Vector2 GetColPos(COL_LR lr, COL_TD td);

	bool isFirst_;

private:

	//画像
	int imgs_[static_cast<int>(AsoUtility::DIR::MAX)][MAX_NUM_ANIM];

	Vector2 startPos_;

	//アニメーションカウンタ
	int cntAnim_;

	//生存判定用のメンバー変数(生存している場合は、true)
	bool isStart_;

	int soundDead_;
	int soundEdamage_;
};
