//pragma once
#include"Vector2F.h"

//前方宣言

class SceneGame;

class Shot
{
public:
	//定数
	//---------------------------------
	//弾のサイズ
	static constexpr int SHOT_SIZE_X = 12;
	static constexpr int SHOT_SIZE_Y = 12;

	//弾のアニメーション数
	static constexpr int SHOT_ANIM_NUM = 2;

	// 弾の生存カウント
	static constexpr int SHOT_CNT_ALIVE = 300;

	//列挙型
	//弾の状態
	enum class STATE
	{
		NONE		//未発射
		,SHOT		//発射中
		,END		//全ての処理が終了
	};

	//メンバー関数
	//----------------------------------

	Shot(void);//デフォルトコンストラクタ
	~Shot(void);//デストラクタ

	//初期化処理(初回の１度のみ実行される)
	bool Init(SceneGame* parent);

	//更新処理(毎フレーム実行)
	void Update();
	virtual void UpdateShot(void);		//弾の更新処理
	
	//描画処理(毎フレーム実行)
	void Draw(void);
	void DrawShot(void);		//弾の描画処理

	//解放処理(終了時の一度のみ実行)
	bool Release(void);

	//弾発射
	void Create(Vector2F pos);

	//弾の生成が可能かどうか判定(NONE or END)
	bool IsEnableCreate(void);

	Vector2F GetPos(void);			//座標を返す

	STATE GetState(void);			//現在の状態を返す

	//状態を変更する
	void ChangeState(STATE value);
protected:
	//メンバー変数
	//--------------------------------------
	//SceneGameのポインタを格納する領域
	SceneGame* sceneGame_;

	//弾の状態
	STATE state_;

	//弾の画像
	int imgs_[SHOT_ANIM_NUM];

	//表示座標
	Vector2F pos_;

	//弾の方向
	Vector2F dir_;

	//弾の移動速度
	float speed_;

	//弾の生存時間(カウンタ)
	int cntAlive_;

	//弾のアニメーション用カウンタ
	int cntAnim_;

	//弾のアニメーション速度
	float speedAnim_;

	//弾のアニメーション番号
	int idxAnim_;

	//メンバー関数
	//-------------------------------

};
