//
//  SequenceGenerators.h
//  dvoinoi-artea
//
//  Created by Alexander Bogdanov
//  Copyright © 2022 dvoinoi. All rights reserved.
//

#ifndef SequenceGenerators_h
#define SequenceGenerators_h


#endif /* SequenceGenerators_h */

// TODO Interface for all these 

struct Christoffel
{
    std::vector<int> getSequence(int x, int y, int length, bool upperOrLower)
    {
        std::vector<int> sequence = {};
        int p;
        int q;
        int a = p = x;
        int b = q = y;
        int n = length;
        int i = 0;
        do {
            sequence.push_back(upperOrLower ? 1 : 0);
            ++i;
            for (a = p, b = q; a != b && i < n; ++i)
                  if (a > b){
                    sequence.push_back(1);
                    b += q;
                } else {
                    sequence.push_back(0);
                    a += p;
                }
                if (a == b && i < n){
                    sequence.push_back(upperOrLower ? 0 : 1);
                    ++i;
                }
          } while (i < n);

        return sequence;
    }
};


struct DeBruijn
{
    // TODO
};


struct Natural
{
    // TODO
};

struct Folding
{
    // TODO
};

struct Stochastic
{
    // TODO
};

