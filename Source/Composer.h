//
//  Composer.h
//  dvoinoi-artea
//
//  Created by Alexander Bogdanov
//  Copyright © 2022 dvoinoi. All rights reserved.
//

#ifndef Composer_h
#define Composer_h


#endif

#include "Bootstrap.h"
#include "SequenceGenerators.h"
#include "MelodicGenerators.h"
#include "SignalProcessors.h"

struct Composer
{
    
    std::vector<int> score = {};
    int scoreIndex = 0;
    
    Composer()
    {
        loopStarting();
    }
    
    int tick()
    {
        if (scoreIndex > score.size()) {
            scoreIndex = 0;
            loopStarting();
        }
        
        int note = score[scoreIndex];
        ++scoreIndex;
        
        if (note == 0) {
            return 0;
        }
        return 60;
        return random(60, 66);
    }
    
    void loopStarting()
    {
        // testing rhythmic generation:
        // score = ritmChristoffel(random(1, 24), random(24, 48), random(100, 900), true);
        
        score = {1,0,0,0};
    }
    
};
