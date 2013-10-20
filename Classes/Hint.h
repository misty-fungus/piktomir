//
//  Hint.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 2/20/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Hint__
#define __pictomir_cocos2d_x__Hint__

#include <string>
#include "GameParams.h"

enum HintType {
    NoHint = 0,
    TextHint,
    MethodHint,
    GlobalHint
};

struct HintPart
{
    HintType type;
    std::string hintText;
    std::vector<int> funcHint;
};

class Hint {

    
public:
    Hint();
    
    std::vector<HintPart> hintParts;
    
    static Hint* create()
    {
        return new Hint();
    }
};

#endif /* defined(__pictomir_cocos2d_x__Hint__) */
