#pragma comment(lib, "ws2_32.lib")

#include "PlayTableScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

extern SOCKET ClientSocket;
extern int dx[4];
extern int dy[4];

Scene* PlayTable::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    PlayTable *layer = PlayTable::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PlayTable::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	scoreboard=false;

	// Receive


	char* cID = new char[64];
	ZeroMemory(cID, 64);
	recv(ClientSocket, cID, 64, NULL);
	this->id = atoi(cID);

	/*
	char* buf = new char[256];
	int n = recv(ClientSocket, buf, 256, NULL);
	for (int i = 0; i != n; i++) printf("%c", buf[i]);
	printf("\n");
	*/

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	for (int i=1;i<=13;i++) 
	{
		cards.push_back(i);
	}

	for (int i=1;i<=4;i++)
	{
		cardsont.push_back(i);
	}

   
    // add "PlayTable" splash screen"
    Sprite* sprite = Sprite::create("table.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	sb = Sprite::create("Darken.png");

    sb->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	sb->retain();


    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("The Imperial March.mp3", true);

	

	int suit,point;

	for (int i=0;i<13;i++)
	{
		suit=cards[i]/14;
		point=cards[i]%14;
		point--;
		cimgs.push_back( Sprite::create("cards.png",CCRectMake(4+(int)(75.666667*point), 4+105*suit, 72, 101) ));
		cimgs[i]->setPosition( Point(70+i*75,100 ));
		this->addChild(cimgs[i],i+1);
	}

	for (int i=0;i<4;i++)
	{
		suit=cardsont[i]/14;
		point=cardsont[i]%14;
		point--;
		ctimgs.push_back( Sprite::create("cards.png",CCRectMake(4+(int)(75.666667*point), 4+105*suit, 72, 101) ));
		ctimgs[i]->setPosition( Point(540+dx[i],720-240+dy[i] ));
		this->addChild(ctimgs[i],i+14);
	}

	

	for (int i=0;i<4;i++)
	{
		score.push_back(0);
	}

	for (int i=0;i<4;i++)
	{
		LabelTTF *x=LabelTTF::create("0","Arial Black",18);
		x->setFontFillColor(Color3B::RED);
		x->setPosition(Point(720+100*i,700));
		stags.push_back(x);
		this->addChild(x);
	}

	for (int i=0;i<4;i++)
	{
		can.push_back(0);
	}

	this->schedule( schedule_selector(PlayTable::gameLogic), 1.0 );

	head=0;
	tail=0;


	MenuItemImage *nextItem = MenuItemImage::create(
                                        "next.png",
                                        "next.png",
                                        CC_CALLBACK_1(PlayTable::menuNextCallback, this));
    
	nextItem->setPosition(Point(540 ,100));

    // create menu, it's an autorelease object
    nextbt = Menu::create(nextItem, NULL);
    nextbt->setPosition(Point::ZERO);

	nextbt->retain();

	get_all_name=false;
    
    
    return true;
}

 void PlayTable::gameLogic(float dt)
{
	int n;
	int MAXLEN=256;
	int Length;
	
	pauseSchedulerAndActions();
	if ((n = recv(ClientSocket, RBuffer, MAXLEN, NULL)) > 0) {
		for (int i = 0; i != n; i++) {
			CircleBuffer[tail++] = RBuffer[i];
			if (tail == MAXLEN * 3) tail = 0;
		}
	}
	if (head == tail) Length = 0;
	else if (head < tail) Length = tail - head;
	else Length = MAXLEN * 3 - head + tail;

	if (!get_all_name) {
		int numD = 0;
		for (int i = head; i <= tail; i++) 
		if (CircleBuffer[i] == '_' && CircleBuffer[i+1] == '$' && CircleBuffer[i+2] == '@') {
			++numD;
		}
		if (numD >= 5) {
			numD = 0;
			int iter = 0, st[5];
			for (int i = head; i <= tail; i++) 
				if (CircleBuffer[i] == '_' && CircleBuffer[i+1] == '$' && CircleBuffer[i+2] == '@') {
					st[numD++] = i;
					if (numD == 5) {
						for (int j = 0; j != 4; j++) {
							strncpy(PlayerName[j], CircleBuffer + st[j] + 3, st[j+1] - st[j] - 3);
							PlayerName[j][st[j+1] - st[j]] = '\0';
							printf("%s\n", PlayerName[j]);
						}
						head = i + 3;
						break;
					}
				}
			get_all_name = true;
		}
	}



	while ( (CircleBuffer[head] == 1 && Length >= 28) || 
			(CircleBuffer[head] == 111 && Length >= 6) ) {
		if (CircleBuffer[head] == 1 && Length >= 28) {
			for (int i = 0; i != 28; i++) {
				RecvBuffer[i] = CircleBuffer[head++];
				if (head >= MAXLEN * 3) head = 0;
			}
			for (int i = 0; i != 13; i++)
			{
				cards[i] = RecvBuffer[i + 1];
				if (cards[i]==0) continue;
				if (cards[i]<14) {cards[i]=cards[i]+3*14;continue;}
				if (cards[i]<28) {cards[i]=cards[i]+14;continue;}
				if (cards[i]<42) {cards[i]=cards[i]-14*2;continue;}
				cards[i]=cards[i]-14*2;
			}

			int suit;
			int point;

			for (int i=0;i<13;i++)
			{
				this->removeChild(cimgs[i]);
			}
			cimgs.clear();
			for (int i=0;i<13;i++)
			{
				suit=cards[i]/14;
				point=cards[i]%14;
				point--;
				if (point==-1)
				{
					cimgs.push_back( Sprite::create("toumei.png"));
				} else {
					cimgs.push_back( Sprite::create("cards.png",CCRectMake(4+(int)(75.666667*point), 4+105*suit, 72, 101) ));
				}
				cimgs[i]->setPosition( Point(70+i*75,100 ));
				this->addChild(cimgs[i],i+1);
			}

			for (int i=0;i<4;i++)
			{
				this->removeChild(ctimgs[i]);
			}

			ctimgs.clear();

			for (int i = 0; i != 4; i++) {
				cardsont[(i+4-id)%4]=RecvBuffer[14 + i];
			}

			for (int i=0;i<4;i++)
			{
				if (cardsont[i]==0) continue;
				if (cardsont[i]<14) {cardsont[i]=cardsont[i]+3*14;continue;}
				if (cardsont[i]<28) {cardsont[i]=cardsont[i]+14;continue;}
				if (cardsont[i]<42) {cardsont[i]=cardsont[i]-14*2;continue;}
				cardsont[i]=cardsont[i]-14*2;
			}

			for (int i=0;i<4;i++)
			{
				suit=cardsont[i]/14;
				point=cardsont[i]%14;
				point--;
				ctimgs.push_back( Sprite::create("cards.png",CCRectMake(4+(int)(75.666667*point), 4+105*suit, 72, 101) ));
				ctimgs[i]->setPosition( Point(540+dx[i],720-240+dy[i] ));
				this->addChild(ctimgs[i],i+14);
			}


					
			for (int i = 0; i != 4; i++) {
				score[i] = RecvBuffer[18 + i];
				char c[100];
				sprintf(c,"%d",score[i]);
				stags[i]->setString(c);
				stags[i]->setFontFillColor (Color3B::BLACK);
			}

			if (RecvBuffer[22]!=4) stags[RecvBuffer[22]]->setFontFillColor(Color3B::RED);

					
			if (RecvBuffer[22] == id) {
				this->unschedule( schedule_selector(PlayTable::gameLogic) );
				this->setTouchEnabled(true);
				for (int i=0;i<4;i++)
				{
					can[i]=RecvBuffer[23 + i];
				}
			}
		}
		if (CircleBuffer[head] == 111 && Length >= 6) {
			for (int i = 0; i != 6; i++) {
				RecvBuffer[i] = CircleBuffer[head++];
				if (head >= MAXLEN * 3) head = 0;
			}

			this->addChild(sb,20);

			int t=simgs.size()/4;

			for (int i=0;i<simgs.size();i++)
			{
				this->addChild(simgs[i],20+i);
			}

			for (int i=0;i<4;i++)
			{
				char stri[100];
				
				sprintf(stri, "%d ", RecvBuffer[i+1]);
				
				LabelTTF *x=LabelTTF::create(stri,"Arial Black",18);
				x->setPosition(Point((i+1)*200,720-(100*(t+1))));
				if (i!=id) x->setFontFillColor (Color3B::WHITE); else x->setFontFillColor(Color3B::RED);
				simgs.push_back(x);
				this->addChild(simgs[simgs.size()-1],20+simgs.size()-1);
			}

			this->addChild(nextbt);

			scoreboard=true;

			head=tail=0;

		}

		if (head == tail) Length = 0;
		else if (head < tail) Length = tail - head;
		else Length = MAXLEN * 3 - head + tail;
	}
	if (!scoreboard) resumeSchedulerAndActions();
}



void PlayTable::menuNextCallback(Object* pSender)
{
	this->removeChild(sb);
	sb->retain();
	
	for (int i=0;i<simgs.size();i++)
	{
		simgs[i]->retain();
		this->removeChild(simgs[i]);
		
	}


	this->removeChild(nextbt);
	nextbt->retain();

	scoreboard=false;
	
	resumeSchedulerAndActions();
}

void PlayTable::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	
	int playcard=13;

	for (int i=0;i<13;i++)
	{
		if (abs(location.x-(70+i*75))<70&&abs(location.y-100)<100)
		{
			playcard=i;
			break;
		}
	}

	if (playcard>12) return;

	int aplaycard=cards[playcard];

	if (aplaycard==0) aplaycard=-1; else
	if (aplaycard<14) aplaycard+=28; else
	if (aplaycard<28) aplaycard+=28; else
	if (aplaycard<42) aplaycard-=14; else
	aplaycard-=42;
			

	if (aplaycard==0 || can[aplaycard / 14] == 0) return;

	int MAXLEN=256;						
	ZeroMemory(SendBuffer, MAXLEN);
	SendBuffer[0] = 1;
	SendBuffer[1] = aplaycard;
	send(ClientSocket, SendBuffer, MAXLEN, NULL);

	this->schedule( schedule_selector(PlayTable::gameLogic), 1.0 );

	this->setTouchEnabled(false);
	return;
								

	
}
