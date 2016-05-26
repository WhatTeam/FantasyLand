﻿#pragma once

#include "cocos2d.h"
#include "Actor.h"
#include "GlobalVariables.h"
#include "MessageDispatchCenter.h"
#include "Helper.h"
#include "AttackCommand.h"


USING_NS_CC;

extern enum EnumRaceType;
extern enum EnumStateType;
static std::string file = "model/archer/archer.c3b";

class Archer : public Actor
{
public:
	Archer();
	CREATE_FUNC(Archer);
	virtual bool init();
	virtual void update(float dt);
	Sprite3D* createArrow();
	void ArcherlAttackCallback(int audioID, std::string filePath);
	void playDyingEffects();
	void hurtSoundEffects();
	void normalAttack();
	void specialAttack();
	void init3D();
	void initActions();

	//set default equipments
	void setDefaultEqt();
	void updataWeapon();
	void updateHelmet();
	void updateArmour();

	//switth weapon
	void switchWeapon();
	//switch helmet
	void switchHelmet();
	//switch armour
	void switchArmour();
	//show/hide arrow
	//isShow: true : Show false : Hide
	//type: 0 : show / hide all 1 : show / hide 1 2 : show / hide 2
	void showOrHideArrow(bool isShow, int arrowType);
	//get weapon id
	int getWeaponID();
	//get armour id
	int getArmourID();
	//get helmet id
	int getHelmetID();
	int hurt(Actor* collider, bool dirKnockMode);


private:
	struct attackCollierInfoForSpecial {
		int minRange;
		int maxRange;
		float angle;
		int knock;
		int damage;
		EnumRaceType mask;
		float duration;
		int speed;
		float criticalChance;
		float DOTTimer;
		float curDOTTimer;
		bool DOTApplied;
	}_specialAttack;
	
};