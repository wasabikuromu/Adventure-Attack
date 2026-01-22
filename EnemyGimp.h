//#pragma once

#include "EnemyBase.h"

class EnemyGimp : public EnemyBase
{
public:

	//ŒÅ—L‚Ìİ’èî•ñ
	void SetParam(void) override;

	void KnockBack(void) override;

	void SetStartPos(Vector2F pos);
	void SetEndPos(Vector2F pos);

	Vector2F GetSPos(void);
	Vector2F GetEPos(void);
	float GetTime(void);

	Vector2F SetSPos(void);
	Vector2F SetEPos(void);
	float SetTime(void);

protected:


	//•\¦À•W
	Vector2F sPos_;
	Vector2F ePos_;
	float time_;
	float step_;
	float moveRight_;
	void Move(void) override;
};
