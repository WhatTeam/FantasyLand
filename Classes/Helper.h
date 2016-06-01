﻿#pragma once
#include <map>
#include <math/Vec2.h>
#include "cocos2d.h"
#include <cmath>
#include "Actor.h"

USING_NS_CC;

extern Camera* camera;

typedef
struct 
{
	float begin;
	float ended;
	float speed;
}timerange;

typedef
struct 
{
	Actor * attacker;
	Actor * target;
}msgStruct;




float radNormalize(float rad);
Vec2 getPosTable(Node *obj);
Vec2 getNextStepPos(Vec2 curPos, Vec2 targetPos , float speed, float dt);
timerange createAnimationStruct(float var1, float var2, float var3);
Animate3D * createAnimation(std::string file, float begin, float finish, float speed);
msgStruct createKnockedMsgStruct(Actor * object);

//function CreateEnumTable(tbl, index)
//function removeAll(table)
//void copytable()
void delayExecute(Actor * target, void (*func)() ,float delay);

float DEGREES_TO_RADIANS(float _ANGLE);
float RADIANS_TO_DEGREES(float _ANGLE);

//void print(std::map<> tab);
