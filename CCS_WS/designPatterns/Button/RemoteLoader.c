/*
 * RemoteLoader.c
 *
 *  Created on: Nov 1, 2020
 *      Author: Ahmed Yusri Moahmed
 */

#include "Buttons.h"

void RemoteLoader(const uint32 *buttonsCfg)
{
   uint8 cmdIdx;

   for(cmdIdx = 0; cmdIdx < 4; ++cmdIdx)
   {
      SetCommand(cmdIdx, buttonsCfg[cmdIdx]);  
   }
}/*RemoteLoader*/

