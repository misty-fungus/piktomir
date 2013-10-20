//
//  ControlLayer4.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/2/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "ControlLayer4.h"
#include "Robot4.h"
#include "GameParams.h"

#include <string.h>

USING_NS_CC;

bool ControlLayer4::init()
{
    setContentSize(CCSizeMake(GetGameParams->mapSize().width,120));

    
    setPosition( ccp(0,0) );
    
    labelCount = CCLabelTTF::create("0" , "Marker Felt", 20);
    labelCount->setPosition( ccp(GetGameParams->mapSize().width / 2, 30) );
    //labelCount->setAnchorPoint( CCPointZero );
    setStepCount(0);
    
    addChild(labelCount, 2, 2);
    
    return true;
}


void ControlLayer4::setStepCount(int stepCount)
{
    char pattern[255];
    
    if(stepCount % 10 == 0 || (stepCount % 10 >= 5 && stepCount % 10 <= 9) || (stepCount % 100 > 10 && stepCount % 100 < 20))
        strcpy(pattern, "StepCountPattern3");
    else if(stepCount % 10 == 1)
        strcpy(pattern, "StepCountPattern1"); 
    else
        strcpy(pattern, "StepCountPattern2"); 

           
    
    labelCount->setString(cocos2d::CCString::createWithFormat(CCLocalizedString(pattern, "NotLocalized %d"), stepCount)->m_sString.c_str());
}

void ControlLayer4::setEnabled(bool flag)
{
//    for(int i=0; i < menu->getChildrenCount(); ++i)
//    {
//        ((CCMenuItemImage *)menu->getChildren()->objectAtIndex(i))->setEnabled(flag);
//    }
}


