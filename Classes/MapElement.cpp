//
//  Map4Element.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "MapElement.h"
#include "PMTextureCache.h"


void MapElement::resize(float scale)
{
    grassSprite->setScaleX(scale);
    grassSprite->setScaleY(scale);
}

void MapElement::setElement(int type)
{
    setType((MapElement::Type)type);
    setGrassSprite(PMTextureCache::instanse()->getMapGrassTile(type));
}
