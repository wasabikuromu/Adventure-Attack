//#pragma once

#include "SceneBase.h"

class SceneGameClear : public SceneBase
{
public:
	//SceneGameClear(void);		//デフォルトコンストラクタ
	//~SceneGameClear(void);		//デストラクタ

	bool Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	bool Release(void) override;	//解放処理

private:
	int gameClearImage_;				//ゲームオーバー画像格納領域
	int gameClearImage1_;				//ゲームオーバー画像格納領域

	int gameClearCounter;

	int soundGameclear_;		//サウンド格納3

	int soundClick_;
};
