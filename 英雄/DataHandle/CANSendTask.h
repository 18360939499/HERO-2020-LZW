#ifndef __DATASENDTASK_H__
#define __DATASENDTASK_H__

#include "main.h"

void CAN1_FrictSend(short a,short b,short c,short d);
void CAN1_BigPullerSend(short	a);
void CAN2_GimbalSend(short pitch,short yaw);
void CAN2_SmlPullerSend(short	a);
void CAN2_BigPushSend(short a);
#endif

