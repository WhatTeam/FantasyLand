﻿#include "BattleScene.h"
#include "Manager.h"
#include "Actor.h"
#include "BattleFieldUI.h"
#include "MessageDispatchCenter.h"

Scene* BattleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BattleScene::create();
	layer->setCascadeColorEnabled(true);
	scene->addChild(layer);
	return scene;
}

bool BattleScene::init()
{
	Layer::init();
	currentLayer = this;

	isDebug = true;
	if (isDebug)
	{
		auto size = Director::getInstance()->getVisibleSize();
		//add camera
		camera = Camera::createPerspective(60, size.width / size.height, 10, 4000);
		camera->setGlobalZOrder(10);
		camera->setPosition3D(Vec3(-500, 80, 0));
		camera->setCameraFlag(CameraFlag::USER1);
		addChild(camera);
		controlCamera();

		//add background
		auto battlefield = Sprite3D::create("battleScene/changjing.c3b");
		battlefield->setCameraMask(2);
		addChild(battlefield);

		debug();
		scheduleUpdate();
		setCameraMask(2, true);
		return true;
	}

	//specialCamera
	scheduler = Director::getInstance()->getScheduler();
	cameraOffset = { 150, 0, 0 };
	cameraOffsetMin = { -300,  -400 };
	cameraOffsetMax = { 300,  400 };

	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB565);

	enableTouch();
	createBackground();
	initUILayer();
	//gameMaster = GameMaster::create();
	setCamera();
	//scheduler->schedule(gameController, this, 0, false, "gameController");

	//MessageDispatchCenter::registerMessage(MessageType::BLOOD_MINUS, bloodMinus);
	//MessageDispatchCenter::registerMessage(MessageType::ANGRY_CHANGE, angryChange);
	//MessageDispatchCenter::registerMessage(MessageType::SPECIAL_PERSPECTIVE, specialPerspective);

	controlCamera();

	return true;
}

void BattleScene::update(float dt)
{
	camera->setPosition3D(camera->getPosition3D() + cameraVelocity * 5);
	auto i = camera->getPosition3D();
	//camera->setPosition(camera->getPosition() + cameraVelocity);
}

void BattleScene::moveCamera(float dt)
{
	//log("moveCamera");
	if (camera == nullptr)
		return;

	auto cameraPosition = getPosTable(camera);
	auto focusPoint = getFocusPointOfHeros();
	if (specialCamera->isBrushValid()/*?*/)
	{
		auto position = ccpLerp(cameraPosition, ccp(specialCamera->getPosition().x, (cameraOffset.y + focusPoint.y - size.height * 3 / 4)*0.5), 5 * dt);
		camera->setPosition(position);
		camera->lookAt(Vec3(position.x, specialCamera->getPosition().y, 50.0), Vec3(0.0, 1.0, 0.0));
	}
	else if (HeroManager.size() > 0)
	{
		auto temp = ccpLerp(cameraPosition, ccp(focusPoint.x + cameraOffset.x, cameraOffset.y + focusPoint.y - size.height * 3 / 4), 2 * dt);
		auto position = Vec3(temp.x, temp.y, size.height / 2 - 100);
		camera->setPosition3D(position);
		camera->lookAt(Vec3(position.x, focusPoint.y, 50.0), Vec3(0.0, 0.0, 1.0));
		//log("\ncalf %f %f %f \ncalf %f %f 50.000000", position.x, position.y, position.z, focusPoint.x, focusPoint.y);
	}
}

void BattleScene::updateParticlePos()
{
	//log("updateParticlePos");
	for (auto it : HeroManager)
	{
		if (it->getEffectNode() != nullptr)
			it->getEffectNode()->setPosition(getPosTable(it));
	}
}

void BattleScene::createBackground()
{
	auto spriteBg = Sprite3D::create("battleScene/changjing.c3b");

	currentLayer->addChild(spriteBg);
	spriteBg->setScale(2.65);
	spriteBg->setPosition3D(Vec3(-2300, -1000, 0));
	spriteBg->setPosition3D(Vec3::ZERO);
	spriteBg->setRotation3D(Vec3(90, 0, 0));
	spriteBg->setCameraMask(2);

	//auto water = Water::create("shader3D/water.png", "shader3D/wave1.jpg", "shader3D/18.jpg", { width = 5500, height = 400 }, 0.77, 0.3797, 1.2);
	//currentLayer->addChild(water);
	//water->setPosition3D(V3(-3500, -580, -110));
	//water->setAnchorPoint(0, 0);
	//water->setGlobalZOrder(0);
}

void BattleScene::setCamera()
{
	camera = Camera::createPerspective(60.0, size.width / size.height, 10.0, 4000.0);
	camera->setGlobalZOrder(10);
	camera->setPosition3D(Vec3::ZERO);
	camera->setCameraFlag(CameraFlag::USER1);
	currentLayer->addChild(camera);

	for (auto it : HeroManager)
	{
		//if (it->getPuff() != nullptr)
			//it->getPuff()->setcamera
	}

	camera->addChild(uiLayer);
}

void BattleScene::gameController(float dt)
{
	gameMaster->update(dt);
	collisionDetect(dt);
	solveAttacks(dt);
	moveCamera(dt);
}

void BattleScene::initUILayer()
{
	uiLayer = BattleFieldUI::create();

	uiLayer->setPositionZ(Director::getInstance()->getZEye() / 4);
	uiLayer->setScale(0.25);
	uiLayer->ignoreAnchorPointForPosition(false);
	uiLayer->setGlobalZOrder(3000);
}

void BattleScene::bloodMinus(Actor* heroActor)
{
	uiLayer->bloodDrop(heroActor);
}

void BattleScene::angryChange(Actor* heroActor)
{
	uiLayer->angryChange(heroActor);
}

void BattleScene::specialPerspective(Actor* heroActor)
{
	//if specialCamera.valid == true  return 

	//	specialCamera.position = param.pos
	//	specialCamera.valid = true
	//	currentLayer:setColor(c3b(125, 125, 125))--deep grey

	//	auto function restoreTimeScale()
	//	specialCamera.valid = false
	//	currentLayer : setColor(c3b(255, 255, 255))--default white
	//	Director : getInstance() : getScheduler() : setTimeScale(1.0)
	//	param.target : setCascadeColorEnabled(true)--restore to the default state
	//	
	//	delayExecute(currentLayer, restoreTimeScale, param.dur)

	//	Director : getInstance() : getScheduler() : setTimeScale(param.speed)
}

void BattleScene::enableTouch()
{
}

void BattleScene::UIcontainsPoint(Vec2 position)
{

}

void BattleScene::controlCamera()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [](Touch*, Event*)
	{
		return true;
	};
	touchListener->onTouchMoved = [](Touch*, Event*)
	{

	};
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keycode, Event*)
	{
		switch (keycode)
		{
		case EventKeyboard::KeyCode::KEY_W:++cameraVelocity.y; break;
		case EventKeyboard::KeyCode::KEY_A:--cameraVelocity.x; break;
		case EventKeyboard::KeyCode::KEY_S:--cameraVelocity.y; break;
		case EventKeyboard::KeyCode::KEY_D:++cameraVelocity.x; break;
		case EventKeyboard::KeyCode::KEY_Q:--cameraVelocity.z; break;
		case EventKeyboard::KeyCode::KEY_E:++cameraVelocity.z; break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keycode, Event*)
	{
		switch (keycode)
		{
		case EventKeyboard::KeyCode::KEY_W:--cameraVelocity.y; break;
		case EventKeyboard::KeyCode::KEY_A:++cameraVelocity.x; break;
		case EventKeyboard::KeyCode::KEY_S:++cameraVelocity.y; break;
		case EventKeyboard::KeyCode::KEY_D:--cameraVelocity.x; break;
		case EventKeyboard::KeyCode::KEY_Q:++cameraVelocity.z; break;
		case EventKeyboard::KeyCode::KEY_E:--cameraVelocity.z; break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void BattleScene::debug()
{
	//the camera->position is Vec3(-500, 80, 0)
}
