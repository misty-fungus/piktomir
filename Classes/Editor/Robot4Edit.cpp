//
//  Robot4Edit.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 15.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "Robot4Edit.h"
#include "Robot4.h"
#include "cocos2d.h"
#include "MapEditorGameLayer.h"
#include "PMCombobox.h"

USING_NS_CC;

void Robot4Edit::setRobot(Robot4* robot) {
    robot_ = robot;
    
//    layerCnt_ = robot->getCurLayer();
    
    state = robot->getCurDirection();
    
    endXCnt_ = robot->getTask()->getEndPoint().x;
    endYCnt_ = robot->getTask()->getEndPoint().y;
    if(endXCnt_ == -1) endYCnt_ = -1;
    endGrassCnt_ = robot->getTask()->getGrassToPaint();
    
    char tmp[10] = "";
    sprintf(tmp,"%d", endXCnt_);
    endXCnt->setString(tmp);
    sprintf(tmp,"%d", endYCnt_);
    endYCnt->setString(tmp);
    sprintf(tmp,"%d", endGrassCnt_);
    paintGrassCnt->setString(tmp);
    
//    baserobot = robot == MapEditorGameLayer::getInstanse()->getWorld()->getBaseRobot();
    
//    char tmp[10] = "";

//    sprintf(tmp,"%d", layerCnt_+1);
//    layerCnt->setString(tmp);
    
    switch (state) {
        case 0:
            setDown(NULL);
            break;
        case 1:
            setRight(NULL);
            break;
        case 2:
            setUp(NULL);
            break;
        case 3:
            setLeft(NULL);
            break;
    }
    
//    if (baserobot) {
//        baseRobotCheckBox->setNormalImage(CCSprite::create("System/checked.png"));
//        baseRobotCheckBox->setSelectedImage(CCSprite::create("System/checked.png"));
//    }
}

//////////////////////////////////////////////////////

void Robot4Edit::plusEndX (CCObject * sender) {
    if (combobox) {
        return;
    }
    endXCnt_ ++;
    if (endXCnt_ == ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->getWidth()) {
        endXCnt_--;
        return;
    }
    if (endXCnt_ == -1) {
        endXCnt_++;
    }
    robot_->getTask()->setEndPoint(ccp(endXCnt_+1,endYCnt_+1));
    char tmp[10] = "";
    sprintf(tmp,"%d", endXCnt_+1);
    endXCnt->setString(tmp);
}

void Robot4Edit::minusEndX (CCObject * sender) {
    if (combobox) {
        return;
    }
    endXCnt_ --;
    if (endXCnt_ == -1) {
        endXCnt_--;
    }
    if(endXCnt_ == -3) {endXCnt_++;return;}
    robot_->getTask()->setEndPoint(ccp(endXCnt_+1,endYCnt_+1));
    char tmp[10] = "";
    sprintf(tmp,"%d", endXCnt_+1);
    endXCnt->setString(tmp);
}

void Robot4Edit::plusEndY (CCObject * sender) {
    if (combobox) {
        return;
    }
    endYCnt_ ++;
    if (endYCnt_ == ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->getHeight()) {
        endYCnt_--;
        return;
    }
    if (endYCnt_ == -1) {
        endYCnt_++;
    }
    robot_->getTask()->setEndPoint(ccp(endXCnt_+1,endYCnt_+1));
    char tmp[10] = "";
    sprintf(tmp,"%d", endYCnt_+1);
    endYCnt->setString(tmp);
}

void Robot4Edit::minusEndY (CCObject * sender) {
    if (combobox) {
        return;
    }
    endYCnt_ --;
    
    if (endYCnt_ == -1) {
        endYCnt_--;
    }
    if(endYCnt_ == -3) {endYCnt_++;return;}
    robot_->getTask()->setEndPoint(ccp(endXCnt_+1,endYCnt_+1));
    
    char tmp[10] = "";
    sprintf(tmp,"%d", endYCnt_+1);
    endYCnt->setString(tmp);
}

//void Robot4Edit::plusEndLayer (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    endLayerCnt_ ++;
//    if (endLayerCnt_ == ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->getLayerCount()) {
//        endLayerCnt_--;
//        return;
//    }
//    if (endLayerCnt_ == -1) {
//        endLayerCnt_++;
//    }
//    
//    robot_->getTask()->setEndLayer(endLayerCnt_+1);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", endLayerCnt_+1);
//    endLayerCnt->setString(tmp);
//    
//}
//
//void Robot4Edit::minusEndLayer (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    endLayerCnt_ --;
//    
//    if (endLayerCnt_ == -1) {
//        endLayerCnt_--;
//    }
//    if(endLayerCnt_ == -3) {endLayerCnt_++;return;}
//    robot_->getTask()->setEndLayer(endLayerCnt_+1);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", endLayerCnt_+1);
//    endLayerCnt->setString(tmp);
//}

void Robot4Edit::plusEndGrass (CCObject * sender) {
    if (combobox) {
        return;
    }
    endGrassCnt_ ++;
    
    robot_->getTask()->setGrassToPaint(endGrassCnt_+1);
    char tmp[10] = "";
    sprintf(tmp,"%d", endGrassCnt_+1);
    paintGrassCnt->setString(tmp);
}

void Robot4Edit::minusEndGrass (CCObject * sender) {
    if (combobox) {
        return;
    }
    endGrassCnt_ --;
    if(endGrassCnt_ == -3) {endGrassCnt_++;return;}
    robot_->getTask()->setGrassToPaint(endGrassCnt_+1);
    
    char tmp[10] = "";
    sprintf(tmp,"%d", endGrassCnt_+1);
    paintGrassCnt->setString(tmp);
}

//void Robot4Edit::plusEndOven (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    endOvenCnt_ ++;
////    if (endOvenCnt_ == ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->getLayerCount()) {
////        endOvenCnt_--;
////        return;
//    //    }
//    robot_->getTask()->setFurnaceToBurn(endOvenCnt_+1);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", endOvenCnt_+1);
//    ovenBurnCnt->setString(tmp);
//    
//}
//
//void Robot4Edit::minusEndOven (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    endOvenCnt_ --;
//    if(endOvenCnt_ == -3) {endOvenCnt_++;return;}
//    robot_->getTask()->setFurnaceToBurn(endOvenCnt_+1);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", endOvenCnt_+1);
//    ovenBurnCnt->setString(tmp);
//}

//void Robot4Edit::setEndDown (CCObject * sender) {
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
//        
//        combobox = false;
//    }
//    ((CCLabelTTF*)endStateLable->getLabel())->setString("Вниз");
//    endStateCnt_ = 0;
//    
//    //robot_->setPosition(ccp(widthCnt_,heightCnt_), layerCnt_, state);
//}
//
//void Robot4Edit::setEndUp (CCObject * sender) {
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
//        
//        combobox = false;
//    }
//    ((CCLabelTTF*)endStateLable->getLabel())->setString("Вверх");
//    endStateCnt_ = 2;
//    //robot_->getTask()->set
//    //robot_->setPosition(ccp(widthCnt_,heightCnt_), layerCnt_, state);
//}
//
//void Robot4Edit::setEndLeft (CCObject * sender) {
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
//        
//        combobox = false;
//    }
//    if (combobox) {
//        return;
//    }
//    ((CCLabelTTF*)endStateLable->getLabel())->setString("Влево");
//    endStateCnt_ = 3;
//    
//    //robot_->setPosition(ccp(widthCnt_,heightCnt_), layerCnt_, state);
//}
//
//void Robot4Edit::setEndRight (CCObject * sender) {
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
//        
//        combobox = false;
//    }
//    ((CCLabelTTF*)endStateLable->getLabel())->setString("Вправо");
//    endStateCnt_ = 1;
//    
//    //robot_->setPosition(ccp(widthCnt_,heightCnt_), layerCnt_, state);
//}

////////////////////////////////////////////////////////////////////////////////////

//void Robot4Edit::plusLayer (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    layerCnt_ ++;
//    if (layerCnt_ == ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->getLayerCount()) {
//        layerCnt_--;
//        return;
//    }
//    robot_->setPosition(robot_->getCurPosition(), layerCnt_, state);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", layerCnt_+1);
//    layerCnt->setString(tmp);
//    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
//    
//}
//
//void Robot4Edit::minusLayer (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    layerCnt_ --;
//    if(layerCnt_ == -1) {layerCnt_++;return;}
//    robot_->setPosition(robot_->getCurPosition(), layerCnt_, state);
//    char tmp[10] = "";
//    sprintf(tmp,"%d", layerCnt_+1);
//    layerCnt->setString(tmp);
//    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
//}

void Robot4Edit::setDown (CCObject * sender) {
    if (types != NULL) {
        removeChild(types,true);
        types = NULL;
        
        combobox = false;
    }
    //((CCLabelTTF*)stateLable->getLabel())->setString("Вниз");
    state = 0;
    
    robot_->setPosition(robot_->getCurPosition(), robot_->getCurLayer()/*layerCnt_*/, state);
    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
}

void Robot4Edit::setUp (CCObject * sender) {
    if (types != NULL) {
        removeChild(types,true);
        types = NULL;
        
        combobox = false;
    }
    //((CCLabelTTF*)stateLable->getLabel())->setString("Вверх");
    state = 2;
    
    robot_->setPosition(robot_->getCurPosition(), robot_->getCurLayer()/*layerCnt_*/, state);
    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
}

void Robot4Edit::setLeft (CCObject * sender) {
    if (types != NULL) {
        removeChild(types,true);
        types = NULL;
        
        combobox = false;
    }
    //((CCLabelTTF*)stateLable->getLabel())->setString("Влево");
    state = 3;
    
    robot_->setPosition(robot_->getCurPosition(), robot_->getCurLayer()/*layerCnt_*/, state);
    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
}

void Robot4Edit::setRight (CCObject * sender) {
    if (types != NULL) {
        removeChild(types,true);
        types = NULL;
        
        combobox = false;
    }
    //((CCLabelTTF*)stateLable->getLabel())->setString("Вправо");
    state = 1;
    
    robot_->setPosition(robot_->getCurPosition(), robot_->getCurLayer()/*layerCnt_*/, state);
    MapEditorGameLayer::getInstanse()->getWorld()->getMap()->update();
}

//void Robot4Edit::setBaseRobot (CCObject * sender) {
//    if (combobox) {
//        return;
//    }
//    baseRobotCheckBox->setNormalImage(CCSprite::create("System/checked.png"));
//    baseRobotCheckBox->setSelectedImage(CCSprite::create("System/checked.png"));
//}

//void Robot4Edit::showRobotTypes (CCObject * sender) {
//    int height = 60;
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
////        
////        combobox = false;
//        return;
//    }
////    combobox = true;
//    types = CCLayerColor::create(ccc4(144, 144, 144, 200), 150, height);
//    types->setPosition(ccp(15,-height-40));
//    
//    CCMenuItemLabel* Robot4Label = CCMenuItemLabel::create(CCLabelTTF::create("Робот4", "Marker Felt", 18),this,menu_selector(Robot4Edit::changeType));
//    Robot4Label->setAnchorPoint(ccp(0,1));
//    Robot4Label->setPosition(ccp(10,height - 5));
//    
//    CCMenuItemLabel* OvenLabel = CCMenuItemLabel::create(CCLabelTTF::create("Духовка", "Marker Felt", 18),this,menu_selector(Robot4Edit::changeType));
//    OvenLabel->setAnchorPoint(ccp(0,1));
//    OvenLabel->setPosition(ccp(10,height - 30));
//    OvenLabel->setEnabled(false);
//
//    CCMenu * menu = CCMenu::create(Robot4Label, OvenLabel, NULL);
//    menu->setPosition(CCPointZero);
//    types->addChild(menu);
//    
//    addChild(types);
//}

//void Robot4Edit::showEndRobotStates (CCObject * sender) {
//    int height = 120;
//    if (types != NULL) {
//        removeChild(types,true);
//        types = NULL;
//        
//        combobox = false;
//        return;
//    }
//    
//    combobox = true;
//    types = CCLayerColor::create(ccc4(144, 144, 144, 200), 180, height);
//    types->setPosition(ccp(215,-height-40));
//    
//    CCMenuItemLabel* downLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вниз", "Marker Felt", 18),this,menu_selector(Robot4Edit::setEndDown));
//    downLabel->setAnchorPoint(ccp(0,1));
//    downLabel->setPosition(ccp(10,height - 5));
//    
//    CCMenuItemLabel* rightLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вправо", "Marker Felt", 18),this,menu_selector(Robot4Edit::setEndRight));
//    rightLabel->setAnchorPoint(ccp(0,1));
//    rightLabel->setPosition(ccp(10,height - 30));
//    
//    CCMenuItemLabel* upLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вверх", "Marker Felt", 18),this,menu_selector(Robot4Edit::setEndUp));
//    upLabel->setAnchorPoint(ccp(0,1));
//    upLabel->setPosition(ccp(10,height - 55));
//    
//    CCMenuItemLabel* leftLabel = CCMenuItemLabel::create(CCLabelTTF::create("Влево", "Marker Felt", 18),this,menu_selector(Robot4Edit::setEndLeft));
//    leftLabel->setAnchorPoint(ccp(0,1));
//    leftLabel->setPosition(ccp(10,height - 80));
//    
//    CCMenu * menu = CCMenu::create(downLabel, rightLabel, upLabel, leftLabel, NULL);
//    menu->setPosition(CCPointZero);
//    types->addChild(menu);
//    
//    addChild(types);
//}

void Robot4Edit::showRobotStates (CCObject * sender) {
    int height = 120;
    if (types != NULL) {
        removeChild(types,true);
        types = NULL;
        
        combobox = false;
        return;
    }
    
    combobox = true;
    types = CCLayerColor::create(ccc4(144, 144, 144, 200), 180, height);
    types->setPosition(ccpAdd(((CCNode*)sender)->getPosition(),ccp(-100,-120)));
    
    CCMenuItemLabel* downLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вниз", "Marker Felt", 18),this,menu_selector(Robot4Edit::setDown));
    downLabel->setAnchorPoint(ccp(0,1));
    downLabel->setPosition(ccp(10,height - 5));
    
    CCMenuItemLabel* rightLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вправо", "Marker Felt", 18),this,menu_selector(Robot4Edit::setRight));
    rightLabel->setAnchorPoint(ccp(0,1));
    rightLabel->setPosition(ccp(10,height - 30));
    
    CCMenuItemLabel* upLabel = CCMenuItemLabel::create(CCLabelTTF::create("Вверх", "Marker Felt", 18),this,menu_selector(Robot4Edit::setUp));
    upLabel->setAnchorPoint(ccp(0,1));
    upLabel->setPosition(ccp(10,height - 55));
    
    CCMenuItemLabel* leftLabel = CCMenuItemLabel::create(CCLabelTTF::create("Влево", "Marker Felt", 18),this,menu_selector(Robot4Edit::setLeft));
    leftLabel->setAnchorPoint(ccp(0,1));
    leftLabel->setPosition(ccp(10,height - 80));
    
    CCMenu * menu = CCMenu::create(downLabel, rightLabel, upLabel, leftLabel, NULL);
    menu->setPosition(CCPointZero);
    types->addChild(menu);
    
    addChild(types);
}

//void Robot4Edit::changeType (CCObject * sender) {
//}
//
//void Robot4Edit::deleteRobot (CCObject * sender) {
//}


void Robot4Edit::onShowCombobox(CCObject* sender)
{
    for (int i = 0; i < menu->getChildren()->count(); ++i) {
        ((CCMenuItem*)menu->getChildren()->objectAtIndex(i))->setEnabled(false);
    }
}

void Robot4Edit::onEndCombobox(CCObject* sender)
{
    switch (((PMComboBox*)sender)->getSelectedItemIndex()) {
        case 0:
            setDown(NULL);
            break;
        case 1:
            setRight(NULL);
            break;
        case 2:
            setUp(NULL);
            break;
        case 3:
            setLeft(NULL);
            break;
    }
    
    for (int i = 0; i < menu->getChildren()->count(); ++i) {
        ((CCMenuItem*)menu->getChildren()->objectAtIndex(i))->setEnabled(true);
    }
}

bool Robot4Edit::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    types = NULL;
//    CCMenuItemLabel * name = CCMenuItemLabel::create(CCLabelTTF::create("Робот4", "Marker Felt", 22),this, menu_selector(Robot4Edit::showRobotTypes));
    CCLabelTTF * name = CCLabelTTF::create("Робот", "Marker Felt", 22);
    name->setAnchorPoint(ccp(0,1));
    name->setPosition(ccp(20,-5));
    addChild(name);
    
    sizeHeight = -25;
        
//    CCLabelTTF * layer = CCLabelTTF::create("Слой", "Marker Felt", 18);
//    layer->setAnchorPoint(ccp(0,1));
//    layer->setPosition(ccp(25,sizeHeight-55));
//    
//    CCMenuItemLabel* layerPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusLayer));
//    layerPlus->setAnchorPoint(ccp(0,1));
//    layerPlus->setPosition(ccp(135,sizeHeight-55));
//    
//    CCMenuItemLabel* layerMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusLayer));
//    layerMinus->setAnchorPoint(ccp(0,1));
//    layerMinus->setPosition(ccp(95,sizeHeight-55));
//    
//    layerCnt = CCLabelTTF::create("1", "Marker Felt", 18);
//    layerCnt->setAnchorPoint(ccp(0,1));
//    layerCnt->setPosition(ccp(110,sizeHeight-55));
    
    
    
    CCLabelTTF * state = CCLabelTTF::create("Состояние:", "Marker Felt", 18);
    state->setAnchorPoint(ccp(0,1));
    state->setPosition(ccp(20,sizeHeight-5));
    
//    stateLable = CCMenuItemLabel::create(CCLabelTTF::create("Вниз", "Marker Felt", 18),this,menu_selector(Robot4Edit::showRobotStates));
//    stateLable->setAnchorPoint(ccp(0,1));
//    stateLable->setPosition(ccp(115,sizeHeight-30));
    
    std::vector<std::string> values;
    values.push_back("Вниз");
    values.push_back("Вправо");
    values.push_back("Вверх");
    values.push_back("Влево");
    
    PMComboBox * combobox_ = PMComboBox::create(values, this, menu_selector(Robot4Edit::onShowCombobox), menu_selector(Robot4Edit::onEndCombobox), 18.0, "Marker Felt");
    combobox_->setPosition(ccp(100,sizeHeight-35));
    addChild(combobox_,100);
    
    sizeHeight-= 25;
    
//    CCLabelTTF * baseRobot = CCLabelTTF::create("Основной:", "Marker Felt", 18);
//    baseRobot->setAnchorPoint(ccp(0,1));
//    baseRobot->setPosition(ccp(25,sizeHeight-105));
//    
//    baseRobotCheckBox = CCMenuItemImage::create("System/unchecked.png", "System/unchecked.png", this, menu_selector(Robot4Edit::setBaseRobot));
//    baseRobotCheckBox->setAnchorPoint(ccp(0,1));
//    baseRobotCheckBox->setPosition(ccp(115,sizeHeight-105));
//    
//    CCMenuItemLabel * deleteRobot = CCMenuItemLabel::create(CCLabelTTF::create("Удалить", "Marker Felt", 16),this, menu_selector(Robot4Edit::deleteRobot));
//    ((CCLabelTTF*)deleteRobot->getLabel())->setColor(ccc3(200, 0, 0));
//    deleteRobot->setAnchorPoint(ccp(0,1));
//    deleteRobot->setPosition(ccp(25,sizeHeight-130));

    
/////////////////////
    
    CCLabelTTF * taskLabel = CCLabelTTF::create("Задание", "Marker Felt", 22);
    taskLabel->setAnchorPoint(ccp(0,1));
    taskLabel->setPosition(ccp(20,sizeHeight-5));
    addChild(taskLabel);
    sizeHeight -= 25;
    
    CCLabelTTF * endX = CCLabelTTF::create("x", "Marker Felt", 18);
    endX->setAnchorPoint(ccp(0,1));
    endX->setPosition(ccp(20,sizeHeight-5));
    
    CCMenuItemLabel* endXPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusEndX));
    endXPlus->setAnchorPoint(ccp(0,1));
    endXPlus->setPosition(ccp(155,sizeHeight-5));
    
    CCMenuItemLabel* endXMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusEndX));
    endXMinus->setAnchorPoint(ccp(0,1));
    endXMinus->setPosition(ccp(115,sizeHeight-5));
    
    endXCnt = CCLabelTTF::create("1", "Marker Felt", 18);
    endXCnt->setAnchorPoint(ccp(0,1));
    endXCnt->setPosition(ccp(130,sizeHeight-5));
    
    
    CCLabelTTF * endY = CCLabelTTF::create("y", "Marker Felt", 18);
    endY->setAnchorPoint(ccp(0,1));
    endY->setPosition(ccp(20,sizeHeight-30));
    
    CCMenuItemLabel* endYPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusEndY));
    endYPlus->setAnchorPoint(ccp(0,1));
    endYPlus->setPosition(ccp(155,sizeHeight-30));
    
    CCMenuItemLabel* endYMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusEndY));
    endYMinus->setAnchorPoint(ccp(0,1));
    endYMinus->setPosition(ccp(115,sizeHeight-30));
    
    endYCnt = CCLabelTTF::create("1", "Marker Felt", 18);
    endYCnt->setAnchorPoint(ccp(0,1));
    endYCnt->setPosition(ccp(130,sizeHeight-30));
    
    
//    CCLabelTTF * endLayer = CCLabelTTF::create("Слой", "Marker Felt", 18);
//    endLayer->setAnchorPoint(ccp(0,1));
//    endLayer->setPosition(ccp(225,sizeHeight-55));
//    
//    CCMenuItemLabel* endLayerPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusEndLayer));
//    endLayerPlus->setAnchorPoint(ccp(0,1));
//    endLayerPlus->setPosition(ccp(355,sizeHeight-55));
//    
//    CCMenuItemLabel* endLayerMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusEndLayer));
//    endLayerMinus->setAnchorPoint(ccp(0,1));
//    endLayerMinus->setPosition(ccp(315,sizeHeight-55));
//    
//    endLayerCnt = CCLabelTTF::create("1", "Marker Felt", 18);
//    endLayerCnt->setAnchorPoint(ccp(0,1));
//    endLayerCnt->setPosition(ccp(330,sizeHeight-55));
//    
//    CCLabelTTF * endState = CCLabelTTF::create("Состояние:", "Marker Felt", 18);
//    endState->setAnchorPoint(ccp(0,1));
//    endState->setPosition(ccp(225,sizeHeight-80));
//    
//    endStateLable = CCMenuItemLabel::create(CCLabelTTF::create("Вниз", "Marker Felt", 18),this,menu_selector(Robot4Edit::showEndRobotStates));
//    endStateLable->setAnchorPoint(ccp(0,1));
//    endStateLable->setPosition(ccp(335,sizeHeight-80));
//    endStateLable->setEnabled(false);
    
    CCLabelTTF * paintGrass = CCLabelTTF::create("Закрасить", "Marker Felt", 18);
    paintGrass->setAnchorPoint(ccp(0,1));
    paintGrass->setPosition(ccp(20,sizeHeight-55));
    
    CCMenuItemLabel* paintGrassPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusEndGrass));
    paintGrassPlus->setAnchorPoint(ccp(0,1));
    paintGrassPlus->setPosition(ccp(155,sizeHeight-55));
    
    CCMenuItemLabel* paintGrassMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusEndGrass));
    paintGrassMinus->setAnchorPoint(ccp(0,1));
    paintGrassMinus->setPosition(ccp(115,sizeHeight-55));
    
    paintGrassCnt = CCLabelTTF::create("1", "Marker Felt", 18);
    paintGrassCnt->setAnchorPoint(ccp(0,1));
    paintGrassCnt->setPosition(ccp(130,sizeHeight-54));
    
    
//    CCLabelTTF * ovenBurn = CCLabelTTF::create("Печки", "Marker Felt", 18);
//    ovenBurn->setAnchorPoint(ccp(0,1));
//    ovenBurn->setPosition(ccp(225,sizeHeight-130));
//    
//    CCMenuItemLabel* ovenBurnPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(Robot4Edit::plusEndOven));
//    ovenBurnPlus->setAnchorPoint(ccp(0,1));
//    ovenBurnPlus->setPosition(ccp(355,sizeHeight-130));
//    
//    CCMenuItemLabel* ovenBurnMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(Robot4Edit::minusEndOven));
//    ovenBurnMinus->setAnchorPoint(ccp(0,1));
//    ovenBurnMinus->setPosition(ccp(315,sizeHeight-130));
//    
//    ovenBurnCnt = CCLabelTTF::create("1", "Marker Felt", 18);
//    ovenBurnCnt->setAnchorPoint(ccp(0,1));
//    ovenBurnCnt->setPosition(ccp(330,sizeHeight-130));
    
    
//    CCMenu * menu = CCMenu::create(name,deleteRobot, layerPlus, layerMinus, stateLable, baseRobotCheckBox, endXPlus, endXMinus, endYPlus, endYMinus, endLayerPlus, endLayerMinus, endStateLable, paintGrassPlus, paintGrassMinus, ovenBurnPlus, ovenBurnMinus, NULL);
    menu = CCMenu::create( endXPlus, endXMinus, endYPlus, endYMinus, paintGrassPlus, paintGrassMinus, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
    
//    addChild(layer);
    addChild(state);
    
    addChild(endX);
    addChild(endY);
//    addChild(endLayer);
//    addChild(endState);
    addChild(paintGrass);
//    addChild(ovenBurn);
//    
//    
//    addChild(baseRobot);
//    addChild(layerCnt);

    addChild(endXCnt);
    addChild(endYCnt);
//    addChild(endLayerCnt);
    addChild(paintGrassCnt);
//    addChild(ovenBurnCnt);
    
    return true;
}

void Robot4Edit::draw () {
    CCLayer::draw();
    
    ccDrawInit();
    ccDrawColor4F(255,255,255,255);
    glLineWidth(2.0f);
    //draw lines
    
    ccDrawLine(ccp(200,-200), ccp(200,0));
}