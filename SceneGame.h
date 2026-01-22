//#pragma once

#include <vector>
#include "SceneBase.h"
#include "Vector2.h"

//前方宣言
class Stage;

class EnemyBase;

class Player;

class Shot;

class SceneGame : public SceneBase
{
public:
	//定数
	static constexpr int CAMERA_WIDTH = 400;
	static constexpr int CAMERA_HEIGHT = 100;
	static constexpr int CAMERA_HEIGHT2 = 400;

	static constexpr int PLYAER_DIE_POINT = 850;

	//SceneGame(void);		//デフォルトコンストラクタ
	//~SceneGame(void);		//デストラクタ

	bool Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	bool Release(void) override;	//解放処理

	//カメラの位置を返す
	Vector2 GetCameraPos(void);

	//ワールド座標をマップ座標に変換する
	Vector2 WorldPos2MapPos(Vector2 worldPos);

	//マップチップを使用した衝突判定
	bool IsCollisionStage(Vector2 worldPos);

	//プレイヤーの座標を返す
	Vector2F GetPlayerPos(void);
	
	void PlayerHit();
	Vector2 GetPlayerHitBox(void);
	//敵を生み出す
	bool CreateEnemy(void);

	bool CreateEnemy1(void);

private:
	int bgImage1_;			//背景画像格納領域1
	int bgImage2_;			//背景画像格納領域2

	int soundHandle0_;		//サウンド格納0
	int soundHandle1_;		//サウンド格納1
	int soundHandle2_;		//サウンド格納2
	int soundHandle3_;		//サウンド格納3
	int soundHandle4_;		//サウンド格納4
	int soundHandle5_;		//サウンド格納5

	//インスタンス格納用
	Stage* stage_;			//ステージ用

	Player* player_;		//プレイヤー用

	//ゲームオーバー判定用
	bool gameOverFlag_;			//true:ゲームオーバー

	//敵の発生タイミング
	int enCounter;

	//音声カウンタ
	int damagedCnt_;

	//複数の敵の動的管理
	std::vector<EnemyBase*>enemys_;	//可変長配列

	//カメラ情報
	//-----------------------------------
	//カメラの位置
	Vector2 cameraPos_;

	//カメラの移動
	void CameraMove(void);

	//当たり判定用関数
	bool IsCollisionRectCenter(
		Vector2 centerPos1		//オブジェクト１の中央座標
		, Vector2 size1			//オブジェクト１のサイズ
		, Vector2 centerPos2	//オブジェクト２の中央座標
		, Vector2 size2			//オブジェクト２のサイズ
	);
};