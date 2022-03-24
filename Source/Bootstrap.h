//
//  Bootstrap.h
//  dvoinoi-artea
//
//  Created by Alexander Bogdanov
//  Copyright © 2022 dvoinoi. All rights reserved.
//

#ifndef Bootstrap_h
#define Bootstrap_h


#endif /* Bootstrap_h */

int random(int min, int max) //range : [min, max]
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

