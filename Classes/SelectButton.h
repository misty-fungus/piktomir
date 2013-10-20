//
//  SelectButton.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 09.12.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__SelectButton__
#define __pictomir_cocos2d_x__SelectButton__

#include <iostream>
//#include "ActionButton.h"
#include "UniversalRobot.h"

class SelectButton : public cocos2d::CCMenuItemSprite
{
    //SYNTHESIZE(cocos2d::CCPoint , startPosition, StartPosition);
public:
    enum Type
    {
        None,
        Method,
        Repeater,
        Condition
    };
protected:
    SYNTHESIZE(int, condition, Condition);
    SYNTHESIZE(int, methodID, MethodID);
    SYNTHESIZE(int, repeater, Repeater);
    
    Type type;
    
    cocos2d::CCAction *runnigAction;
    cocos2d::CCPoint startPosition;
public:
    
    bool isCondition() {
        return type == Condition;
    }
    
    bool isRepeater() {
        return type == Repeater;
    }
    
    bool isMethod() {
        return type == Method;
    }

    
    SelectButton():
        condition( EMPTY_CONDITION ),
        methodID(EMPTY_METHOD),
        repeater(1)
    {}
    
    ~SelectButton()
    {        
        stopAllActions();
    }
    
    static SelectButton* create()
    {
        return new SelectButton();
    }
    
    SelectButton(Type type,int value,cocos2d::CCSprite *normalImage,
                 cocos2d::CCSprite *selectedImage, cocos2d::CCSprite *disabledImage,CCObject* target,
                 cocos2d::SEL_MenuHandler selector):
        condition( EMPTY_CONDITION ),
        methodID(EMPTY_METHOD),
        repeater(1),
        type(type)
    {
        initWithNormalSprite(normalImage, selectedImage,disabledImage, target, selector);
        
        switch (type) {
            case Method:
                methodID = value;
                break;
            case Condition:
                condition = value;
                break;
            case Repeater:
                repeater = value;
                break;
            default:
                break;
        }
    }
    
    static SelectButton* create(Type type,int value,cocos2d::CCSprite *normalImage,
                                cocos2d::CCSprite *selectedImage, cocos2d::CCSprite *disabledImage, CCObject* target,
                                cocos2d::SEL_MenuHandler selector)
    {
        return new SelectButton(type, value, normalImage, selectedImage,disabledImage, target, selector);
    }
    
    void startAnimation();
    void jumpOnce();
    
    void stopAnimation();
    
    cocos2d::CCSprite *cloneSprite();
    
    void select();
    void deselect();
    //void animateAction(float s);
};

#endif /* defined(__pictomir_cocos2d_x__SelectButton__) */
