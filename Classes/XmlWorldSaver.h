//
//  XmlWorldSaver.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 22.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__XmlWorldSaver__
#define __pictomir_cocos2d_x__XmlWorldSaver__

#include <iostream>
#include "UniversalWorld.h"
#include "GameLayer.h"

class XMLWorldSaver {
    UniversalWorld* world_;
    GameLayer* layer_;
    std::string path_;
    
    void saveWorld4(xmlNodePtr map);
    xmlNodePtr saveMap4Layer (int layer);
    
    xmlNodePtr saveProgram ();
    xmlNodePtr saveMethod (int layer);
    xmlNodePtr saveTask (int robot);
    
    xmlNodePtr saveHint(int methodlayer);
    
    xmlNodePtr saveRobot (int robot);
    
public:
    XMLWorldSaver(UniversalWorld * world, GameLayer * gamelayer, std::string path);
};

#endif /* defined(__pictomir_cocos2d_x__XmlWorldSaver__) */
