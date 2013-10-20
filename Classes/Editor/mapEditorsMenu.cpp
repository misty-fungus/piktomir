//
//  mapEditorsMenu.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 11.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "mapEditorsMenu.h"

map4EditorsMenu *map4EditorsMenu::instanse = NULL;

 map4EditorsMenu* map4EditorsMenu::getInstanse() {
     if (instanse == NULL) {
         instanse = new map4EditorsMenu();
     }
    
    return instanse;
}