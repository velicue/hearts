#pragma comment(lib, "ws2_32.lib")

#include "HelloWorldScene.h"
#include "PlayTableScene.h"
#include "SimpleAudioEngine.h"
#include <WinSock2.h>



USING_NS_CC;

extern SOCKET ClientSocket;

int InitWinSocket()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	return WSAStartup(DllVersion, &wsaData);
}

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *closeItem = MenuItemImage::create(
                                        "button.png",
                                        "button.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(981 ,720-288));

    // create menu, it's an autorelease object
    Menu* menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    TextFieldTTF* label = TextFieldTTF::textFieldWithPlaceHolder("Username", "Arial Black", 25);

	//label->attachWithIME();
    
    // position the label on the center of the screen
    label->setPosition(Point(874,720-164));

    // add the label as a child to this layer
    this->addChild(label, 1);

	this->username=label;

	label = TextFieldTTF::textFieldWithPlaceHolder("Host Address", "Arial Black", 25);

	//label->attachWithIME();
    
    // position the label on the center of the screen
    label->setPosition(Point(874,720-229));

    // add the label as a child to this layer
    this->addChild(label, 2);

	this->sockaddr=label;

    // add "HelloWorld" splash screen"
    Sprite* sprite = Sprite::create("login.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Raujika-Pastoral.mp3", true);

	this->setTouchEnabled(true);

	/*CCSprite *player = CCSprite::create("cards.png", 
                                   CCRectMake(5, 5, 75, 104) );
    player->setPosition( Point(100,100 ));
	this->addChild(player);*/

	//while(1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    
	
	

	char ip[100];
	int port;


	if (InitWinSocket() != 0) {
		printf("Init Win Socket Error !");
		return;
	}

	
    SOCKADDR_IN addr;
	

	// Create Socket
	ClientSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("Create Socket Error !\n");
		return;
	}

	char c[100];
	strcpy(c,this->sockaddr->getString());
	int lc=strlen(c);
	int i;
	for (i=0;i<lc;i++)
	{
		if (c[i]==':') break;
		ip[i]=c[i];
	}
	ip[i]=0;
	char pc[100];
	for (int j=i+1;j<lc;j++)
	{
		pc[j-i-1]=c[j];
	}
	sscanf(pc,"%d",&port);

	

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	
	if (connect(ClientSocket, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		printf("Could not connect to server ...[ENTER]\n");
		return;
	}


	// Send Username
	send(ClientSocket, this->username->getString(), strlen(this->username->getString()), NULL); 

	Director::getInstance()->replaceScene(PlayTable::scene());	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

	this->username->detachWithIME();
	this->sockaddr->detachWithIME();

	if (abs(location.x - 874)<180&&abs(location.y-(720-164))<25)
	{
		this->username->attachWithIME();
	}

	if (abs(location.x - 874)<180&&abs(location.y-(720-229))<25)
	{
		this->sockaddr->attachWithIME();
	}
}
