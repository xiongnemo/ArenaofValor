#include "HouYi.h"
#include "Scene/HelloWorldScene.h"


HouYi* HouYi::create(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode)
{
	HouYi* houYi = new(std::nothrow)HouYi;
	if (houYi && houYi->init(combatScene, camp, heroName, attackMode))
	{
		houYi->autorelease();
		return houYi;
	}
	CC_SAFE_DELETE(houYi);
	return nullptr;
}

bool HouYi::init(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode)
{
	if (!Hero::init(combatScene, camp, heroName, attackMode))
	{
		return false;
	}

	_punishState = 0;
	_punishStateTime = 0.f;

	return true;
}

void HouYi::castSkill_1()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	//TODO: ����
	if (_skillLevel_1 > 0 && nowTime - _lastSkillTime_1 > _calmTime_1)
	{
		takeBuff(Buff::create(EBuffType::MULTIPLE, 5, 0.5 * _attack + 100 + 100 * _skillLevel_1 * 100, 0, 0, 0, 0, 0, 0, 0, 0));
	}

	_lastSkillTime_1 = nowTime;
}

void HouYi::castSkill_2(Point mousePosition)
{
	auto nowTime = GetCurrentTime() / 1000.f;
	//TODO: ����
	if (_skillLevel_2 > 0 && nowTime - _lastSkillTime_2 > _calmTime_2)
	{
		float skillRadius = 350.f;
		float effectRadius = 60.f;

		auto positionInMap = mousePosition - _combatScene->getMap()->getPosition();
		auto effectPosition = positionInMap;
		auto myPosition = getPosition();

		if (positionInMap.distance(myPosition) >= skillRadius)
		{
			auto delta = positionInMap - myPosition;
			effectPosition = myPosition + skillRadius / delta.length() * delta;
		}

		auto skillAnimation = AnimationCache::getInstance()->getAnimation("HouYiSkill2");
		skillAnimation->setLoops(1);
		skillAnimation->setDelayPerUnit(0.07);
		auto effectSpr = Sprite::create();

		auto node = _combatScene->getMap()->getChildByTag(TAG_HOUYI_SKILL2);
		if (node)
		{
			_combatScene->getMap()->removeChild(node);
		}

//		effectSpr->runAction(Hide::create());
		effectSpr->setPosition(effectPosition);
		effectSpr->setTag(TAG_HOUYI_SKILL2);
		_combatScene->getMap()->addChild(effectSpr);

		auto animation = Sequence::create
		(
			DelayTime::create(0.2f),
			Animate::create(skillAnimation),
			Hide::create(),
			NULL
		);

		effectSpr->runAction(animation);
	//	_combatScene->getMap()->removeChild(effectSpr);


		Vector<Hero*>& allHeroes = _combatScene->_heroes;
		Vector<Soldier*> allSoldiers = _combatScene->_soldiers;
		auto buff = Buff::create(EBuffType::NORMAL, 2, 0, 0, 0, 0, 0, 0, 0, -180, 0);

		for (auto it = allHeroes.begin(); it != allHeroes.end(); ++it)
		{
			if (_camp != (*it)->getCamp() && !(*it)->getAlreadyDead() && (*it)->getPosition().distance(effectPosition) <= effectRadius)
			{
				auto damage = Damage(150 + 100 * _skillLevel_2, this, *it, EDamageType::MAGIC_DAMAGE, GetCurrentTime() / 1000 + 0.5);
				_combatScene->_damages.push_back(damage);

				(*it)->takeBuff(buff);
			}
		}

		for (auto it = allSoldiers.begin(); it != allSoldiers.end(); ++it)
		{
			if (_camp != (*it)->getCamp() && (*it)->getPosition().distance(effectPosition) <= effectRadius)
			{
				auto damage = Damage(1500 + 100 * _skillLevel_2, this, *it, EDamageType::MAGIC_DAMAGE, GetCurrentTime() / 1000 + 0.5);
				_combatScene->_damages.push_back(damage);

				(*it)->takeBuff(buff);
			}
		}
	}

	_lastSkillTime_2 = nowTime;
}

void HouYi::castSkill_3()
{

}


void HouYi::updateAttackTarget()
{
	INT32 minHealth = 10000000;
	Actor* tmpTarget = NULL;

	Vector<Hero*>& allHeroes = _combatScene->_heroes;
	Vector<Soldier*>& allSoldiers = _combatScene->_soldiers;
	Vector<Actor*>& allTowers = _combatScene->_towers;
	for (auto it = allHeroes.begin(); it != allHeroes.end(); ++it)
	{
		if ((*it)->getHealthComp()->getCurrentState() < minHealth&&_camp != (*it)->getCamp() && !(*it)->getAlreadyDead() && (*it)->getPosition().distance(this->getPosition()) <= this->getAttackRadius())
		{
			minHealth = (*it)->getHealthComp()->getCurrentState();
			tmpTarget = *it;
		}
	}
	if (!tmpTarget)
	{
		for (auto it = allSoldiers.begin(); it != allSoldiers.end(); ++it)
		{
			if ((*it)->getHealthComp()->getCurrentState() > minHealth&&_camp != (*it)->getCamp() && !(*it)->getAlreadyDead() && (*it)->getPosition().distance(this->getPosition()) <= this->getAttackRadius())
			{
				minHealth = (*it)->getHealthComp()->getCurrentState();
				tmpTarget = *it;
			}
		}
	}
	if (!tmpTarget)
	{
		for (auto it = allTowers.begin(); it != allTowers.end(); ++it)
		{
			if ((*it)->getHealthComp()->getCurrentState() > minHealth&&_camp != (*it)->getCamp() && !(*it)->getAlreadyDead() && (*it)->getPosition().distance(this->getPosition()) <= this->getAttackRadius())
			{
				minHealth = (*it)->getHealthComp()->getCurrentState();
				tmpTarget = *it;
			}
		}
	}
	this->setAttackTarget(tmpTarget);
}


bool HouYi::attack()
{
	stopAllActions();

	

	auto nowTime = GetCurrentTime() / 1000;
	if (nowTime - _lastAttackTime > _minAttackInterval&&_attackTarget)
	{
		if ((getPosition() - _attackTarget->getPosition()).length() < this->getAttackRadius())
		{
						
		}

	}

	return false;
}