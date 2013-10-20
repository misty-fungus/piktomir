//
//  PMCombobox.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 31.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__PMCombobox__
#define __pictomir_cocos2d_x__PMCombobox__

#include <iostream>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PMComboBox : public CCLayer {
public:
    virtual bool initWithValuesAndSelector(std::vector<std::string>, CCObject*, SEL_MenuHandler, SEL_MenuHandler, float, const char*);
    static PMComboBox* create (std::vector<std::string>, CCObject*, SEL_MenuHandler, SEL_MenuHandler, float, const char*);
    
    int getSelectedItemIndex() {
        return selectedItem;
    }
    
    PMComboBox () :
        values(),
        button(NULL),
        scroll(NULL),
        onShow(NULL),
        onEnd(NULL),
        selectedItem(-1),
        showedLabel(NULL),
        showed(false),
        width(0),
        height(0)
    {
        
    }
    
private:
    std::vector<CCLabelTTF*> values;
    CCLayerColor * button;
    CCScrollView * scroll;
    SEL_MenuHandler onShow;
    SEL_MenuHandler onEnd;
    CCObject* target;
    int selectedItem;
    
    CCLabelTTF* showedLabel;
    
    bool showed;
    
    float width;
    float height;
    
    virtual void onEnter();
    virtual void onExit();
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
};


#endif /* defined(__pictomir_cocos2d_x__PMCombobox__) */
