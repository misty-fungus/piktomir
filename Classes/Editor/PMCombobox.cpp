//
//  PMCombobox.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 31.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "PMCombobox.h"


USING_NS_CC;

PMComboBox* PMComboBox::create(std::vector<std::string> values_, CCObject* target, SEL_MenuHandler onShow, SEL_MenuHandler onEnd, float fontSize, const char * font)
{
    PMComboBox *pRet = new PMComboBox();
    pRet->initWithValuesAndSelector(values_, target, onShow, onEnd, fontSize, font);
    pRet->autorelease();
    return pRet;
}

bool PMComboBox::initWithValuesAndSelector(std::vector<std::string> values_, CCObject* target_, SEL_MenuHandler onShow_, SEL_MenuHandler onEnd_, float fontSize, const char * font)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    onShow = onShow_;
    onEnd = onEnd_;
    target = target_;
    
    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    float maxWidth = 0;
    float maxHeight = 0;
    
    for (int i = 0; i < values_.size(); ++i)
    {
        values.push_back(CCLabelTTF::create(values_[i].c_str(), font, fontSize));
        if (values[i]->getContentSize().width > maxWidth) {
            maxWidth = values[i]->getContentSize().width;
        }
        if (values[i]->getContentSize().height > maxHeight) {
            maxHeight = values[i]->getContentSize().height;
        }
    }

    maxWidth+=40;
    
    width = maxWidth;
    height = maxHeight;
    
    setContentSize(CCSizeMake(maxWidth, maxHeight));
    
    button = CCLayerColor::create(ccc4(144, 144, 144, 200), maxWidth, maxHeight);
    button->setPosition(CCPointZero);
    addChild(button);
    
    showedLabel =  CCLabelTTF::create(values_[0].c_str(), font, fontSize);
    showedLabel->setColor(ccBLACK);
    selectedItem = 0;
    showedLabel->setPosition(ccp(10,1));
    showedLabel->setAnchorPoint(CCPointZero);
    button->addChild(showedLabel,1);
    
    CCSprite * left = CCSprite::create("System/ComboboxLeft.png");
    CCSprite * center = CCSprite::create("System/ComboboxCenter.png");
    CCSprite * right = CCSprite::create("System/ComboboxRight.png");
    
    left->setAnchorPoint(CCPointZero);
    center->setAnchorPoint(CCPointZero);
    right->setAnchorPoint(ccp(1,0));
    
    left->setPosition(CCPointZero);
    center->setPosition(ccp(10,0));
    right->setPosition(ccp(maxWidth,0));
    
    left->setScaleY(maxHeight/30);
    center->setScaleY(maxHeight/30);
    center->setScaleX((maxWidth-40)/20);
    right->setScaleY(maxHeight/30);
    
    button->addChild(left,0);
    button->addChild(center,0);
    button->addChild(right,0);
    
    setTouchPriority(-150);
    scroll = CCScrollView::create(CCSizeMake(maxWidth, maxHeight * values.size() + 16));
    scroll->setPosition(ccp(0, - maxHeight * values.size() - 16));
    
    scroll->setViewSize( CCSizeMake(maxWidth, maxHeight * values.size() + 16) );
    scroll->setContentSize( CCSizeMake(maxWidth, maxHeight * values.size() + 16) );
    scroll->setAnchorPoint( ccp(0,0) );
    scroll->setContentOffset( ccp(0, 0), true);
    scroll->setClippingToBounds(true);
    scroll->setDirection(kCCScrollViewDirectionVertical);
    scroll->setZoomScale(0.5);
    scroll->setBounceable(true);
    
    scroll->setVisible(false);
    scroll->setTouchPriority(-150);
    addChild(scroll,100);
    
    center = CCSprite::create("System/ComboboxCenter.png");
    center->setAnchorPoint(CCPointZero);
    center->setPosition(CCPointZero);
    center->setScaleY((maxHeight * values.size()+16)/30);
    center->setScaleX(maxWidth/20);
    scroll->addChild(center,0);
    
    
    for (int i = 0; i < values.size(); ++i)
    {
        values[values.size()-i-1]->setPosition(ccp(5,i*maxHeight + 8));
        values[values.size()-i-1]->setAnchorPoint(ccp(0,1));
        values[values.size()-i-1]->setColor(ccBLACK);
        scroll->addChild(values[values.size()-i-1], 100);
    }
    
    return true;
}

void PMComboBox::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void PMComboBox::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool PMComboBox::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if (showed)
    {
        scroll->ccTouchBegan(touch, pEvent);
        return true;
    }
    else
    {
        CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
        
        if (CCRectMake(0, 0, width, height).containsPoint(touchPoint)) {
            return true;
        }
        else return false;
    }
}

void PMComboBox::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    scroll->ccTouchMoved(touch, pEvent);
}

void PMComboBox::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if (showed)
    {
        scroll->ccTouchEnded(touch, pEvent);
        CCPoint touchPoint = this->convertToNodeSpace(touch->getStartLocation());
        touchPoint.y = -touchPoint.y;
        if (!CCRectMake(0, 0, width, ((float)values.size()) * height).containsPoint(touchPoint)) {
            scroll->setVisible(false);
            showed = false;
        }
        else
        {
            CCPoint dif = ccpSub(touch->getStartLocation(), touch->getLocation());
            dif.x = fabs(dif.x);
            dif.y = fabs(dif.y);
            if (dif.x < 5 && dif.y < 5)
            {
                selectedItem = (touchPoint.y-15)/height;
                scroll->setVisible(false);
                showed = false;
                showedLabel->setString(values[selectedItem]->getString());
                if(onEnd && target)(target->*onEnd)(this);
            }
        }
    }
    else
    {
        CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
        if (CCRectMake(0, 0, width, height).containsPoint(touchPoint)) {
            if(onShow && target)(target->*onShow)(this);
            scroll->setVisible(true);
            showed = true;
        }
    }
}