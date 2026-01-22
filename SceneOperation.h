//#pragma once

#include "Vector2.h"
#include "SceneBase.h"

class SceneOperation : public SceneBase
{
public:
	//SceneGameClear(void);		//デフォルトコンストラクタ
	//~SceneGameClear(void);		//デストラクタ

	bool Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	bool Release(void) override;	//解放処理

private:
	
	Vector2 oncPos_;

	int operationImage_;				//画像格納領域
	int operationImage1_;				//画像格納領域
	int operationImage2_;				//画像格納領域

	int operationCounter_;

	int soundClick_;
};

