﻿#pragma once

#include "cocos2d.h"
#include "Helper.h"
#include "HPCounter.h"
#include "AttackCommand.h"

USING_NS_CC;

extern enum EnumRaceType;
extern enum EnumStateType;

class BasicCollider;

class Actor : public Node
{
public:	
	CREATE_FUNC(Actor);
	virtual bool init();
	void addEffect(Sprite3D* effect);
	void initPuff();
	void initShadow();
	void playAnimation(std::string name, bool loop);

	//getter & setter
	EnumRaceType getRaceType();
	void setRaceType(EnumRaceType type);
	EnumStateType getStateType();
	void setStateType(EnumStateType type);
	void setTarget(Actor* target);
	void setFacing(float degrees);
	bool getAIEnabled();
	void setAIEnabled(bool enable);
	int hurt(BasicCollider* collider, bool dirKnockMode);
	void hurtSoundEffects();
	void normalAttackSoundEffects();
	void specialAttackSoundEffects();
	void playDyingEffects();

	//attacking collision check
	void normalAttack();
	void specialAttack();

	//State Machine switching functions
	void idleMode();	//switch into idle mode
	void walkMode();	//switch into walk mode
	void attackMode();	//switch into attack mode
	void knockMode(Actor* collider, bool dirKnockMode);
	void dyingMode(Actor* knockSource, int knockAmount);

	//Base Update Functions
	void stateMachineUpdate(float dt);
	Actor* _findEnemy(EnumRaceType HeroOrMonster, bool &allDead);
	bool _inRange();

	//AI function does not run every tick
	void AI();
	void baseUpdate(float dt);
	void knockingUpdate(float dt);
	void attackUpdate(float dt);
	void walkUpdate(float dt);
	void movementUpdate(float dt);

protected:
	float _aliveTime;	//time the actor is alive in seconds
	int _curSpeed;	//current speed the actor is traveling in units/seconds
    Animation* _curAnimation;
	Animation3D* _curAnimation3d;

	std::string _name;

	//runtime modified values
	int _curFacing;	//current direction the actor is facing, in radians, 0 is to the right
	bool _isalive;
	float _AITimer;	//accumulated timer before AI will execute, in seconds
	bool _AIEnabled; //if false, AI will not run
	float _attackTimer;	//accumulated timer to decide when to attack, in seconds
	float _timeKnocked;	//accumulated timer to recover from knock, in seconds
	bool _cooldown;	//if its true, then you are currently playing attacking animation
	int _hp = 1000;	//current hit point
	bool goRight;
	
	//target variables
	int targetFacing;	//direction the actor Wants to turn to
	Actor* _target;	//the enemy actor;
	Vec2 _myPos;
	int _angry;
	int _angryMax;

	//other variables
	HPCounter* _hpCounter;
	Node* _effectNode;
	int _monsterHeight;
	int _heroHeight;
	BillBoard* _puff;
	Sprite* _circle;
	Map<std::string, Animate3D*> _action;

	int _useWeaponId;
	int _useArmourId;
	int _useHelmetId;
	ProgressTimer* _bloodBar;
	ProgressTimer* _bloodBarClone;
	Sprite* _avatar;

	//
	EnumRaceType _racetype;	//type of the actor
	EnumStateType _statetype;	//AI state machine
    Sprite3D* _sprite3d;	//place to hold 3d model

    int _radius;	//actor collider size
	int _shadowSize;	//weight of the role, it affects collision
	int _mass;	//the size of the shadow under the actor

	int _maxhp;
	int _defense;
	int _specialAttackChance;
	float _specialSlowTime;
	float _recoverTime;	//time takes to recover from knock, in seconds

	int _speed; //actor maximum movement speed in units/seconds
	float _turnSpeed;	//actor turning speed in radians/seconds
	int _acceleration;	//actor movement acceleration, in units/seconds
	int _decceleration;	//actor movement decceleration, in units/seconds

	float _AIFrequency;	//how often AI executes in seconds
	float _attackFrequency;	//an attack move every few seconds
	int _searchDistance;	//distance which enemy can be found

	int _attackRange;	//distance the actor will stop and commence attack

	//attack collider info, it can be customized
	struct attackColliderInfo {	//data for normal attack
		int minRange;	//collider inner radius
		int maxRange;	//collider outer radius
		float angle;	//collider angle, 360 for full circle, otherwise, a fan shape is created
		int knock;	//attack knock back distance
		int damage;	//attack damage
		EnumRaceType mask;	//who created this attack collider
		float duration;	//0 means it'll be removed upon calculation
		int speed;	//speed the collider is traveling;
		int criticalChance;
	} _normalAttack;
};