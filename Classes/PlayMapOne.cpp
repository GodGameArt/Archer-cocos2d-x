﻿#include "PlayMapOne.h"

//根据tmx决定
#define BODY_GID 9
#define GROUND_GID 7
#define DEAD_LINE_GID 2
#define CAN_DISPEAR_GID 10
#define TMX_WIDTH 80
#define TMX_HEIGHT 23

static int count=0;


PlayMapOne::~PlayMapOne() {

}

void PlayMapOne::onEnter() {
	Layer::onEnter();

	bisover = false;
	//visibleSize，屏幕尺寸
	visibleSize = Director::getInstance()->getVisibleSize();

	//添加碰撞监听器
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayMapOne::onContactBegin, this);
	contactListener->onContactPostSolve = CC_CALLBACK_2(PlayMapOne::onContactPostSolve, this);
	contactListener->onContactPreSolve = CC_CALLBACK_2(PlayMapOne::onContactPreSolve, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(PlayMapOne::onContactSeparate, this);
	//事件分发器
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//网格地图
	auto winSize = Director::getInstance()->getWinSize();
	//map00瓦片地图地图
	tilemap = TMXTiledMap::create("map/map1.tmx");
	tilemap->setAnchorPoint(Vec2(0,0));
	tilemap->setPosition(Vec2(0,0));
	this->addChild(tilemap,-1);

	TMXObjectGroup *objects = tilemap->getObjectGroup("objLayer");
	CCASSERT(NULL != objects, "'Objects' object group not found");
	auto spawnPoint = objects->getObject("hero");
	CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
	print_x = spawnPoint["x"].asInt();
	print_y = spawnPoint["y"].asInt();
	//auto group = tilemap->getObjectGroup("objects");



	TMXLayer* layer = tilemap->getLayer("collideLayer");//从map中取出“bricks”图层
	//这个循环嵌套是为了给每个砖块精灵设置一个刚体
	int count = 0; 
	for (int y = 0; y<TMX_HEIGHT; y++)
	{
		log("xxxxxxxxxxxxxxxxxxxx:%d",y);
		for (int x = 0; x<TMX_WIDTH; x++)
		{
			log("yyyyyyyyyyyyyyyyy:%d",x);
			int gid = layer->getTileGIDAt(Vec2(x, y));//为了提高点效率，我们没必要给每个tile加上刚体，在创建地图时我们设定了空白处的gid值为12，因此我们只对非12的tile加上刚体
			log("GGIIDD %d",gid);
			switch (gid)
			{
			case GROUND_GID:
				{
					log("GGGGGGGGGGGGIIIIIIIIIIIIIDDDDDDDDDDDDD");
					Sprite* sprite = layer->getTileAt(Vec2(x, y));//从tile的坐标取出对应的精灵
					log("sssssssssss  %f,ppppppppppppp %f", sprite->getPosition().x, sprite->getPosition().y);
					auto point = Point(sprite->getPosition().x, sprite->getPosition().y);
					auto size = Size(sprite->getContentSize().width, sprite->getContentSize().height);
					log("xxxxxxxxxxx  %f,llllllllllll %f", sprite->getContentSize().width, sprite->getContentSize().height);
					this->makePhysicsObjAt(point, size, false, 0, 0.0f, 0.0f, 0, GROUND_GID,-1);
				}break;
			case DEAD_LINE_GID:
			{
				Sprite* sprite = layer->getTileAt(Vec2(x, y));//从tile的坐标取出对应的精灵
				log("sssssssssss  %f,ppppppppppppp %f", sprite->getPosition().x, sprite->getPosition().y);
				auto point = Point(sprite->getPosition().x, sprite->getPosition().y);
				auto size = Size(sprite->getContentSize().width, sprite->getContentSize().height);
				log("xxxxxxxxxxx  %f,llllllllllll %f", sprite->getContentSize().width, sprite->getContentSize().height);
				this->makePhysicsObjAt(point, size, false, 0, 0.0f, 0.0f, 0, DEAD_LINE_GID,-1);
			}break;
			case CAN_DISPEAR_GID:
			{
				Sprite* sprite = layer->getTileAt(Vec2(x, y));//从tile的坐标取出对应的精灵
				sprite->setTag(count);
				log("sssssssssss  %f,ppppppppppppp %f", sprite->getPosition().x, sprite->getPosition().y);
				auto point = Point(sprite->getPosition().x, sprite->getPosition().y);
				auto size = Size(sprite->getContentSize().width, sprite->getContentSize().height);
				log("xxxxxxxxxxx  %f,llllllllllll %f", sprite->getContentSize().width, sprite->getContentSize().height);
				this->makePhysicsObjAt(point, size, false, 0, 0.0f, 0.0f, 0, CAN_DISPEAR_GID,count);
				count++;
			}break;
			case  BODY_GID:
			{
				log("GGGGGGGGGGGGIIIIIIIIIIIIIDDDDDDDDDDDDD");
				Sprite* sprite = layer->getTileAt(Vec2(x, y));//从tile的坐标取出对应的精灵
				log("sssssssssss  %f,ppppppppppppp %f", sprite->getPosition().x, sprite->getPosition().y);
				auto point = Point(sprite->getPosition().x, sprite->getPosition().y);
				auto size = Size(sprite->getContentSize().width, sprite->getContentSize().height);
				log("xxxxxxxxxxx  %f,llllllllllll %f", sprite->getContentSize().width, sprite->getContentSize().height);
				this->makePhysicsObjAt(point, size, false, 0, 0.0f, 0.0f, 0, GROUND_GID, -1);
			}break;
			default:
				break;
			}
			if (count == 10000)count = 0;
		}
	}

	
	/*
	//带物理的会动的
	dead = DeadStar::create();
	dead->initWithFile("dead.png");			
	dead->setAnchorPoint(Point(0, 0);
	dead->setPosition(visibleSize.width / 2 + 20, visibleSize.height / 3 + 400);
	dead->getPhysicsBody()->setContactTestBitmask(DEAD_LINE);
	this->addChild(dead);
	*/

	//加入障碍物
//	auto obstacle1 = Obstacle::create();
//	obstacle1->setAnchorPoint(Point(0, 0));//
//	obstacle1->setPosition(visibleSize.width / 2+300 , visibleSize.height /2);
//	obstacle1->getPhysicsBody()->setContactTestBitmask(OBJECT_BIT_MASK);
//	this->addChild(obstacle1);

	//加入圆形
	/*
	auto circular = Circular::create();
	circular->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 3+50);
	circular->getPhysicsBody()->setContactTestBitmask(OBJECT_BIT_MASK);
	addChild(circular);

	auto circular2 = Circular::create();
	circular2->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 3 + 100);
	circular2->getPhysicsBody()->setContactTestBitmask(OBJECT_BIT_MASK);
	addChild(circular2);*/

	//加入小怪兽
//	auto MA1 = MonsterB::create();
//	MA1->initWithFile("redcircle.png");
//	MA1->setAnchorPoint(Point(0, 1));
//	MA1->setPosition(visibleSize.width /3 * 2, visibleSize.height / 3 + 200);
//	MA1->getPhysicsBody()->setContactTestBitmask(MONSTER1_BIT_MASK);
//	log("MA1 - getTag();%d", MA1 - getTag());
//	this->addChild(MA1);

	//设置准确位置，放置怪兽；
	auto printM1 = objects->getObject("monster1");
	CCASSERT(!printM1.empty(), "SpawnPoint object not found");
	int x1 = printM1["x"].asInt();
	int y1 = printM1["y"].asInt();
//	auto _player = Sprite::create("orangetriangle.png");
	auto player1 = MonsterB::create();
	//player1->initWithFile("orangetriangle.png");
	player1->setAnchorPoint(Point(0, 1));
	player1->setPosition(x1, y1);
	player1->getPhysicsBody()->setContactTestBitmask(MONSTER1_BIT_MASK);
	addChild(player1);
	//setViewPointCenter(_player->getPosition());

	auto printM2 = objects->getObject("monster2");
	CCASSERT(!printM2.empty(), "SpawnPoint object not found");
	int x2 = printM2["x"].asInt();
	int y2 = printM2["y"].asInt();
	//	auto _player = Sprite::create("orangetriangle.png");
	auto player2 = MonsterA::create();
	//player2->initWithFile("orangetriangle.png");
	player2->setAnchorPoint(Point(0, 1));
	player2->setPosition(x2, y2);
	player2->getPhysicsBody()->setContactTestBitmask(MONSTER1_BIT_MASK);
	addChild(player2);
	
	auto printM3 = objects->getObject("monster3");
	CCASSERT(!printM3.empty(), "SpawnPoint object not found");
	int x3 = printM3["x"].asInt();
	int y3 = printM3["y"].asInt();
	//	auto _player = Sprite::create("orangetriangle.png");
	auto player3 = MonsterB::create();
	//player2->initWithFile("orangetriangle.png");
	player3->setAnchorPoint(Point(0, 1));
	player3->setPosition(x3, y3);
	player3->getPhysicsBody()->setContactTestBitmask(MONSTER1_BIT_MASK);
	addChild(player3);

	//加入门
	auto door = Ground::create();
	door->groundSize = Size(visibleSize.width / 6, visibleSize.height/4);
	door->init(door->groundSize);
	door->getPhysicsBody()->setContactTestBitmask(DOOR_BIT_MASK);
	//stage1->setAnchorPoint(Point(0, 1));visibleSize.width / 2, visibleSize.height / 3
	door->setPosition(door->groundSize.width / 2 + visibleSize.width * 2, door->groundSize.height / 2 + visibleSize.height / 3);
	addChild(door);
	auto doorpicture = Sprite::create("road_5.png");	//静态的,为不动的刚体添加图片使用这种方式
	doorpicture->setAnchorPoint(Point(0, 0));
	doorpicture->setPosition(visibleSize.width * 2, visibleSize.height / 3);
	this->addChild(doorpicture);//

	//加入桥
/*
	bridge = Board::create();
	bridge->setPosition(100, 100);
	bridge->setPosition(visibleSize.width / 2 + 20, visibleSize.height / 3 + 200);
	addChild(bridge);
*/

	state = 0;
	scheduleUpdate();
}


void PlayMapOne::update(float time) {

}

void PlayMapOne::onExit() {
	Layer::onExit();
	auto nodes = this->getChildren();
	for (const auto& node : nodes) {
		auto subnodes = node->getChildren();
		for (const auto& subnode : subnodes) {
			node->removeChild(subnode);
		}
		this->removeChild(node);
	}
}


bool PlayMapOne::onContactBegin(PhysicsContact &contact) {
	switch (contact.getShapeA()->getBody()->getContactTestBitmask() |
		contact.getShapeB()->getBody()->getContactTestBitmask())
	{
	//注意整个解决方法中各类中碰撞处理函数中的 case的值不要相同，同一个的碰撞只能有一个地方在处理
	/*case ARROW_BIT_MASK | MONSTER1_BIT_MASK://箭射中怪物
	{
		//dead->removeFromParentAndCleanup(true);
		MA1->removeFromParentAndCleanup(true);
	}
	break;*/
	case ARROW_BIT_MASK|CAN_DISPEAR_BIT_MASK:
	{
		auto a = contact.getShapeA()->getBody()->getNode();
		auto b = contact.getShapeB()->getBody()->getNode();
		log("dispear！！！！！！！！！！！！！！！");
		if (a != NULL) {
			long at = a->getTag();// tilemap->removeChildByTag();
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect(sound_5);
			}
			a->removeFromParentAndCleanup(true); 
			log("at  %d", at); long bt = b->getTag(); log("bt %d", bt);
			if (-1 < at < 10001) {
				do {
					tilemap->getLayer("collideLayer")->removeChildByTag(at);
				} 
				while (tilemap->getLayer("collideLayer")->getChildByTag(at));
			}

		}
		if (b!= NULL) {
			;
			//long bt = b->getTag();// tilemap->removeChildByTag();
			//b->removeFromParentAndCleanup(true);
			//log("bt %d", bt);
			//if (-1 < bt < 10001)tilemap->getLayer("pengzhuang")->removeChildByTag(bt);
		}
	}break;
	default:
		break;
	}
	return true;
}

bool PlayMapOne::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve) {
	return true;
}

void PlayMapOne::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve) {

}

void PlayMapOne::onContactSeparate(PhysicsContact& contact) {

}

void PlayMapOne::makePhysicsObjAt(Point p, Size size, bool d, float r, float f, float dens, float rest,int gid,int tag)
{

	auto sprite = Ground::create();
	sprite->groundSize = size;
	sprite->setTag(tag);
	sprite->init(sprite->groundSize);
	//stage1->setAnchorPoint(Point(0, 1));visibleSize.width / 2, visibleSize.height / 3
	sprite->setPosition(sprite->groundSize.width / 2 + p.x, sprite->groundSize.height / 2 + p.y);
	switch(gid)
	{
	case GROUND_GID:
	{
		sprite->getPhysicsBody()->setContactTestBitmask(GROUND_BIT_MASK);
	}break;
	case DEAD_LINE_GID:
	{
		sprite->getPhysicsBody()->setContactTestBitmask(DEAD_LINE_BIT_MASK);
	}break;
	case CAN_DISPEAR_GID:
	{
		sprite->getPhysicsBody()->setContactTestBitmask(CAN_DISPEAR_BIT_MASK);
	}break;
	case BODY_GID:
	{
		sprite->getPhysicsBody()->setContactTestBitmask(GROUND_BIT_MASK);
	}break;
	default:
		break;
	}
	addChild(sprite);
}
