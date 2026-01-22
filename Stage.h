//#pragma once

#include "Vector2.h"

//前方宣言
class SceneGame;

class Stage
{
public:
	//定数(すべて大文字)
	//----------------------------------

	//マップチップ関連
	//1チップのサイズ
	static constexpr int CHIP_SIZE_X = 32;			//横サイズ
	static constexpr int CHIP_SIZE_Y = 32;			//縦サイズ

	//マップチップの素材数
	static constexpr int NUM_MAP_CHIPS_X = 18;		//横素材数
	static constexpr int NUM_MAP_CHIPS_Y = 10;		//縦素材数

	//マップチップの素材総数
	static constexpr int NUM_MAP_CHIPS = NUM_MAP_CHIPS_X * NUM_MAP_CHIPS_Y;

	//マップの最大サイズ
	static constexpr int MAP_MAX_SIZE_X = 150;
	static constexpr int MAP_MAX_SIZE_Y = 25;

	//マップの縦横サイズ
	static constexpr int MAP_GROUND_SIZE_X = 150;
	static constexpr int MAP_GROUND_SIZE_Y = 25;

	//地下マップの縦横サイズ
	static constexpr int UN_MAP_GROUND_SIZE_X = 150;
	static constexpr int UN_MAP_GROUND_SIZE_Y = 25;

	//マップ種別
	enum class TYPE
	{
		NONE
		,GROUND			//地上
		,UN_GROUND		//地下
	};

	//メンバー関数
	//----------------------------------

	Stage(void);	//デフォルトコンストラクタ
	~Stage(void);	//デストラクタ

	//初期化処理(初回の１度のみ実行される)
	bool Init(SceneGame* parent);

	//更新処理(毎フレーム実行)
	void Update(void);

	//描画処理(毎フレーム実行)
	void Draw(void);

	//解放処理(終了時の一度のみ実行)
	bool Release(void);

	//外部ファイルから地上ステージCSVデータを読み込む
	void LoadGroundCsvData(void);

	//外部ファイルから地下ステージCSVデータを読み込む
	void LoadUnGroundCsvData(void);

	//マップチップ番号を返す
	int GetChipNo(Vector2 mapPos);

	//マップサイズを返す
	Vector2 GetUsedMapSize(void);

	//ワープエリアかどうかをチェックする
	bool IsWarp(Vector2 mapPos);

	//ワープエリアかどうかをチェックする
	bool IsGoal(Vector2 mapPos);

	//マップ種別を返す
	TYPE GetMapType(void);

	//マップを変更する
	void ChangeMap(TYPE value);

private:
	//メンバー変数 
	//----------------------------------
	
	//SceneGameのポインタを格納する領域
	SceneGame* sceneGame_;

	//マップチップ1の格納領域
	int imgMapChips_[NUM_MAP_CHIPS];

	//処理で利用するマップ配列
	int usedMap_[MAP_MAX_SIZE_Y][MAP_MAX_SIZE_X];
	
	//処理で利用する格納したマップのサイズ
	Vector2 usedMapSize_;

	//処理しているマップの種別
	TYPE type_;

	//地上マップの格納領域
	int groundMap_[MAP_GROUND_SIZE_Y][MAP_GROUND_SIZE_X];

	//地下マップの格納領域
	int unGroundMap_[UN_MAP_GROUND_SIZE_Y][UN_MAP_GROUND_SIZE_X];

	//メンバー関数
	//--------------------------------------------
	
	//処理で利用するマップ配列を初期化する
	void ClearUsedMap(void);

};
