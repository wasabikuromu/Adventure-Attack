//#pragma once

#include "EnemyBase.h"
class Shot;

class EnemyBoss : public EnemyBase
{
public:

	void Draw(void)override;
	void Update(void)override;
	bool Release(void)override;
	void SetParam(void) override;
	void Hit(int value) override;

protected:

	void Move(void) override;
	//’e—p
	Shot* shot_;
};
