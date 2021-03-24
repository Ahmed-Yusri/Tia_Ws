/*
 * Hmi_Screen_Cfg.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Ahmeed Yusri Mohamed
 */


#include "Hmi_Screen_Cfg.h"

static const Hmi_ScreenCfgNfcJobType Hmi_ScreenCfgNfcJob =\
{
   {0x72, 0x0A, FALSE},
   {0x73, 0x0A, FALSE},
   {0x77, 0x0C, TRUE},
   {0x78, 0x09, FALSE},
   {0x79, 0x09, FALSE}
};

const Hmi_ScreenCfgType Hmi_ScreanCfg =\
{
   &Hmi_userInterventionJobEndCbk,
   &Hmi_readJobEndCbk,
   &Hmi_writeJobEndCbk,
   (const struct Hmi_ScreenNfcJobSt *)&Hmi_ScreenCfgNfcJob
};

