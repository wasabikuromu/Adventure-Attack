#pragma once

class SceneBase
{
public:
	SceneBase(void);			//デフォルトコンストラクタ
	virtual ~SceneBase(void);	//デストラクタ

	virtual bool Init(void);	//初期化処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	virtual bool Release(void);	//解放処理
};
