//#pragma once

#include "EnemyBase.h"

class EnemyMob1 : public EnemyBase
{
public:

	//ŒÅ—L‚Ìİ’èî•ñ
	void SetParam(void) override;

	void KnockBack(void) override;

protected:

	void Move(void) override;
};
