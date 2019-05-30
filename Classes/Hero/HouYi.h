#ifndef __HOUYI_H__
#define __HOUYI_H__

#include "cocos2d.h"
#include "Actor/Hero.h"

USING_NS_CC;

class HelloWorld;

class HouYi :public Hero
{
	CC_SYNTHESIZE(INT32, _punishState, PunishState);
	CC_SYNTHESIZE(INT32, _punishStateTime, PunishStateTime);

public:

	virtual void castSkill_1();

	virtual void castSkill_2(Point mousePosition);

	virtual void castSkill_3();

	virtual void updateAttackTarget();

	virtual bool attack();

	virtual bool init(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode);

	static HouYi* create(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode);
};

#endif // !__HOUYI_H__
