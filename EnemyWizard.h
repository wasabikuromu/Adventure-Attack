//#pragma once
class Shot;

#include "EnemyBase.h"

class EnemyWizard : public EnemyBase
{
public:

	//ŒÅ—L‚Ìİ’èî•ñ
	void Draw(void)override;
	void Update(void)override;
	bool Release(void)override;
	void SetParam(void) override;
	void Hit(int value) override;
	void KnockBack(void) override;

protected:

	void Move(void) override;

	//’e—p
	Shot* shot_;			
	
};

