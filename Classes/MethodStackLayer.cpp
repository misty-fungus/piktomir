//
//  MethodStackLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/19/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "MethodStackLayer.h"
#include "GameLayer.h"
#include "GameParams.h"

USING_NS_CC;
USING_NS_CC_EXT;

MethodStackLayer *MethodStackLayer::create(GameLayer *parent,RobotManager *robotManager)
{
    MethodStackLayer *pRet = new MethodStackLayer(parent, robotManager);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool MethodStackLayer::init()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    stackCapasity = ((int)mapSize.height - 400) / (int)BUTTON_TILE_SIZE;
    
    showButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getShowIconSprite(),
                                          PMTextureCache::instanse()->getShowIconSprite_Selected(),
                                          PMTextureCache::instanse()->getShowIconSprite_Disabled(),
                                          this, menu_selector(MethodStackLayer::show));
    
    hideButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getHideIconSprite(),
                                          PMTextureCache::instanse()->getHideIconSprite_Selected(),
                                          PMTextureCache::instanse()->getHideIconSprite_Disabled(),
                                          this, menu_selector(MethodStackLayer::hide));
    
    clearButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getClearIconSprite(),
                                          PMTextureCache::instanse()->getClearIconSprite_Selected(),
                                          PMTextureCache::instanse()->getClearIconSprite_Disabled(),
                                          this, menu_selector(MethodStackLayer::clear));
    
    showButton->setAnchorPoint( CCPointZero );
    showButton->setPosition( CCPointZero );
    hideButton->setAnchorPoint( CCPointZero );
    hideButton->setPosition( CCPointZero );
    clearButton->setPosition( ccp(0, - hideButton->getContentSize().height) );
    clearButton->setAnchorPoint( CCPointZero );
    clearButton->setEnabled(false);
    
    hideButton->setVisible(false);
    
    CCMenu *menu = CCMenu::create(showButton, hideButton, clearButton, NULL);
    menu->setAnchorPoint( CCPointZero );
    menu->setPosition( ccp(0, stackCapasity * BUTTON_TILE_SIZE - showButton->getContentSize().height) );
    
    setContentSize(CCSizeMake(BUTTON_TILE_SIZE + showButton->getContentSize().width, stackCapasity * BUTTON_TILE_SIZE));
    setPosition(ccp(mapSize.width - showButton->getContentSize().width, 200));
    
    CCLayerColor *colorLayer = CCLayerColor::create(ccc4(145,145,145,255 * 0.5f), BUTTON_TILE_SIZE, stackCapasity * BUTTON_TILE_SIZE);
    colorLayer->setAnchorPoint( ccp(0,0) );
    colorLayer->setPosition( ccp(showButton->getContentSize().width,0) );
    
    scroll = MethodScrollLayer::create(this);
    scroll->setViewSize( CCSizeMake(BUTTON_TILE_SIZE, stackCapasity * BUTTON_TILE_SIZE) );
    scroll->setAnchorPoint( ccp(0,0) );
    scroll->setPosition( ccp(showButton->getContentSize().width,0) );
    scroll->setContentSize( CCSizeMake(BUTTON_TILE_SIZE, stackCapasity * BUTTON_TILE_SIZE) );
    scroll->setContentOffset( ccp(0, 0), true);
    scroll->setClippingToBounds(true);
    scroll->setDirection(kCCScrollViewDirectionVertical);
    scroll->setZoomScale(0.5);
    scroll->setBounceable(true);
    
    addChild(menu);
    addChild(colorLayer);
    addChild(scroll, 10);
    
    topPoint = ccp(showButton->getContentSize().width, getContentSize().height - BUTTON_TILE_SIZE);
    curPoint = CCPointZero;
    
    //clear();
    
	lastButtonRepeater = false;

    return true;
}

void MethodStackLayer::setEnabled(bool flag)
{
    showButton->setEnabled(flag);
    hideButton->setEnabled(flag);
    clearButton->setEnabled(flag);
    
    scroll->setActive(flag);
}

void MethodStackLayer::toggleIcon()
{
    showButton->setVisible( !showButton->isVisible() );
    hideButton->setVisible( !hideButton->isVisible() );
}

void MethodStackLayer::addMethod(SelectButton *button)
{
    lastButton = button;
    
    parent->getProgramLayer()->setEnabled(false);
    
    if(button->isRepeater() && lastButtonRepeater)
        return;
    
    if(button->isRepeater())
        lastButtonRepeater = true;
    
    lastSprite = button->cloneSprite();
    
    if(BUTTON_TILE_SIZE * (methodStack.size() + 1) > scroll->getContentSize().height)
    {
        scroll->setContentSize( CCSizeMake(BUTTON_TILE_SIZE, scroll->getContentSize().height + BUTTON_TILE_SIZE));
        scroll->setContentOffset(ccp(0, BUTTON_TILE_SIZE * stackCapasity - scroll->getContentSize().height), true);
    }
    
    CCPoint worldPosition = parent->getProgramLayer()->getSelectMenu()->convertToWorldSpace(button->getPosition());
    CCPoint thisPostion = this->convertToNodeSpace(worldPosition);
    
    thisPostion = ccp(thisPostion.x - lastSprite->getContentSize().width / 2,thisPostion.y - lastSprite->getContentSize().height / 2);
    
    lastSprite->setPosition(thisPostion);
    
    addChild(lastSprite, 20);
    spriteStack.push(lastSprite);
    
    lastSprite->setAnchorPoint(CCPointZero);
    
    CCFiniteTimeAction* moveSpriteParaboloic = CCJumpBy::create(3.0f * SYSTEM_ANIMATION_DELAY, ccpSub(topPoint,lastSprite->getPosition()),110.0f, 1);
    CCFiniteTimeAction* moveSpriteDown = CCMoveTo::create(3.0f * SYSTEM_ANIMATION_DELAY, curPoint);
    
    CCFiniteTimeAction* reorder = CCCallFunc::create( this, callfunc_selector(MethodStackLayer::reorderSprite));
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this, callfunc_selector(MethodStackLayer::endAddMethod));
    CCFiniteTimeAction* moveStart = CCCallFunc::create( this, callfunc_selector(MethodStackLayer::startAddMethod));
    
    curPoint = ccp(0, curPoint.y + BUTTON_TILE_SIZE);
    
    lastSprite->runAction( CCSequence::create(moveStart, moveSpriteParaboloic, reorder, moveSpriteDown, moveEnd, NULL) );
    

    clearButton->setEnabled(true);
}

void MethodStackLayer::reorderSprite()
{
    CCPoint worldPosition = convertToWorldSpace(lastSprite->getPosition());
    
    lastSprite->retain();
    
    removeChild(lastSprite, false);
    
    scroll->addChild(lastSprite,1);

    CCPoint pos = scroll->convertToNodeSpace(worldPosition);
    
    if(methodStack.size() > stackCapasity)
        pos.y += scroll->getContentSize().height - BUTTON_TILE_SIZE * stackCapasity;
    
    lastSprite->setPosition(pos);
    
    lastSprite->release();
}

void MethodStackLayer::removeMethod()
{
    _instruction instr = methodStack.top();
    methodStack.pop();
    
    _instruction nextInstr = {-1,-1,-1,-1};
    
    if(!methodStack.empty())
        nextInstr = methodStack.top();
    
    if(instr.command != INSTR_CHECK_LOOP)
    {
        instr.param = robotManager->getRobot()->getMethod(instr.param).getInverseMethod();
        
        robotManager->addInstruction(instr);
    }
    
    if(nextInstr.command == INSTR_CHECK_LOOP )
    {
        robotManager->addInstruction(nextInstr);
    }
    
    if(instr.command != INSTR_CHECK_LOOP)
        robotManager->setState(RobotManager::ControlledByStack);
    
    scroll->removeChild(lastSprite);
    
    spriteStack.pop();
    
    if(methodStack.size() > 0)
    {
        lastSprite = spriteStack.top();
        
        if(methodStack.size() + 1 > stackCapasity)
        {
            setContentSize(CCSizeMake(BUTTON_TILE_SIZE, scroll->getContentSize().height - BUTTON_TILE_SIZE));
            scroll->setContentOffset(ccp(0, BUTTON_TILE_SIZE * stackCapasity - scroll->getContentSize().height), true);
        }
    }
    else
        lastSprite = NULL;
    
    curPoint.y -= BUTTON_TILE_SIZE;
}

void MethodStackLayer::startAddMethod()
{
    _instruction instr;
    
    if(lastButton->isMethod())
    {
        instr.command = INSTR_EXECUTE;
        instr.param = lastButton->getMethodID();
        instr.otherParams[0] = instr.otherParams[1] = -1;
        
        robotManager->addInstruction(instr);
        
        if(lastButtonRepeater)
            robotManager->addInstruction(methodStack.top());
        
        robotManager->setState(RobotManager::ControlledByStack);
    }
    else if(lastButton->isRepeater())
    {
        instr.command = INSTR_CHECK_LOOP;
        instr.param = lastButton->getRepeater();
        instr.otherParams[1] = 0;
        instr.otherParams[0] = 0;
    }
    
    methodStack.push(instr);
    
}

void MethodStackLayer::clear()
{
    scroll->getContainer()->removeAllChildrenWithCleanup(true);
    
    methodStack = std::stack<_instruction>();
    spriteStack = std::stack<CCSprite *>();
    
    lastButtonRepeater = false;
    lastButton = NULL;
    lastSprite = NULL;
    clearButton->setEnabled(false);
    
    curPoint = CCPointZero;

}

void MethodStackLayer::clear(CCObject* object)
{
    clear();
    
    parent->restartLevel(NULL);
}

void MethodStackLayer::endAddMethod()
{
    if(lastButton->isMethod())
        lastButtonRepeater = false;
    
    parent->getProgramLayer()->setEnabled(true);
}

void MethodStackLayer::show(CCObject *sender)
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(mapSize.width - BUTTON_TILE_SIZE - showButton->getContentSize().width - 10, 200));
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(MethodStackLayer::showEnded));
    
    runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
    
    robotManager->setControlled(true);
    
}

void MethodStackLayer::hide(CCObject *sender)
{
    parent->reorderChild(this,10);
    scroll->setActive(false);
    
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY,  ccp(mapSize.width - showButton->getContentSize().width, 200));
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(MethodStackLayer::hideEnded));
    
    runAction( CCSequence::create(moveLayer, moveEnd, NULL) );

    robotManager->setControlled(false);
}

void MethodStackLayer::showEnded()
{
    parent->reorderChild(this,30);
    parent->getProgramLayer()->setControlledMode(true);
    toggleIcon();
    
    parent->getProgramControlLayer()->setEnabled(false);
    scroll->setActive(true);
}

void MethodStackLayer::hideEnded()
{
    parent->getProgramLayer()->setControlledMode(false);
    toggleIcon();
    
    parent->getProgramControlLayer()->setEnabled(true);
    
    parent->restartLevel(NULL);
}

std::vector<int> MethodStackLayer::getProgram()
{
    std::vector<int> ret;
    
    
    while(!methodStack.empty())
    {
        _instruction instr = methodStack.top();
        methodStack.pop();
        
        if(instr.command == INSTR_CHECK_LOOP)
            continue;
        
        _instruction nextInstr = {-1,-1,-1,-1};
        
        if(!methodStack.empty())
            nextInstr = methodStack.top();
        
        if(nextInstr.command == INSTR_CHECK_LOOP)
        {
            for(int  j = 0; j < nextInstr.param; ++j)
            {
                ret.push_back(instr.param);
            }
        }
        else
            ret.push_back(instr.param);
    }
    //reverse 
    for(int i = 0; i < ret.size() / 2; ++i)
    {
        int temp = ret[i];
        ret[i] = ret[ret.size() - 1 - i];
        ret[ret.size() - 1 - i] = temp;
    }
    
    clear();
    
    hide(NULL);
    
    return ret;
}

