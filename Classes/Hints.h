//
//  Hints.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 2/20/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Hints__
#define __pictomir_cocos2d_x__Hints__

#include <vector>
#include "Hint.h"
#include <libxml/xmlreader.h>

class Hints
{
public:
    Hints()
    {
    }

    std::vector<Hint> hints;
    
    static Hints* create()
    {
        return new Hints();
    }
};

#endif /* defined(__pictomir_cocos2d_x__Hints__) */
