#ifndef __PLAYTABLE_SCENE_H__
#define __PLAYTABLE_SCENE_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

class PlayTable : public cocos2d::Layer
{
protected:

	vector<int> cards;
	vector<int> cardsont;
	vector<Sprite *> cimgs;
	vector<Sprite *> ctimgs;
	vector<LabelTTF *> stags;

	vector<int> score;

	vector<LabelTTF *> simgs;

	vector<int> can;

	char SendBuffer[256];
	char RBuffer[256];
	char RecvBuffer[256];
	char CircleBuffer[256*3];

	bool scoreboard;

	Sprite *sb;

	Menu *nextbt;

	bool get_all_name;
	char PlayerName[4][256];

	

	int head,tail;

	int id;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuNextCallback(Object* pSender);

	void PlayTable::ccTouchesEnded(CCSet* touches, CCEvent* event);

	void PlayTable::gameLogic(float dt);
    
    // implement the "static node()" method manually
    CREATE_FUNC(PlayTable);


};

#endif // __HELLOWORLD_SCENE_H__
