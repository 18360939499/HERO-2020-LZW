#ifndef _vaguePID_
#define _vaguePID_
//模糊PID
#include "stm32f4xx.h"

enum{
  NB=-6,
	NM=-4,
	NS=-2,
	ZO=0,
	PS=2,
	PM=4,
	PB=6
}vagueNum;
/*
规则库,高维是E,低维是EC
*/
int32_t ruleKp[7][7]={{PB,PB,PM,PM,PS,ZO,ZO},
                             {PB,PB,PM,PS,PS,ZO,NS},
                             {PM,PM,PM,PS,ZO,NS,NS},
                             {PM,PM,PS,ZO,NS,NM,NM},
                             {PS,PS,ZO,NS,NS,NM,NM},
                             {PS,ZO,NS,NM,NM,NM,NB},
                             {ZO,ZO,NM,NM,NM,NB,NB}
};
int32_t ruleKi[7][7]={{NB,NB,NM,NM,NS,ZO,ZO},
                             {NB,NB,NM,NS,NS,ZO,ZO},
                             {NB,NM,NS,NS,ZO,PS,PS},
                             {NM,NM,NS,ZO,PS,PM,PM},
                             {NM,NS,ZO,PS,PS,PM,PB},
                             {ZO,ZO,PS,PS,PM,PB,PB},
                             {ZO,ZO,PS,PM,PM,PB,PB}
};
int32_t ruleKd[7][7]={{PS,NS,NB,NB,NB,NM,PS},
                             {PS,NS,NB,NM,NM,NS,ZO},
                             {ZO,NS,NM,NM,NS,NS,ZO},
                             {ZO,NS,NS,NS,NS,NS,ZO},
                             {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                             {PB,NS,PS,PS,PS,PS,PB},
                             {PB,PM,PM,PM,PS,PS,PB}

};

typedef struct
{
  float setpoint;               /*设定值*/

  float kp;                     /*比例系数*/

  float ki;                     /*积分系数*/

  float kd;                     /*微分系数*/

  float lasterror;              /*前一拍偏差*/

  float preerror;               /*前两拍偏差*/

  float deadband;               /*死区*/

  float output;                 /*输出值*/

  float result;                 /*物理量输出值*/

  float maximum;                /*输出值的上限*/

  float minimum;                /*输出值的下限*/

  float maxdKp;                 /*Kp增量的最大限值*/

  float mindKp;                 /*Kp增量的最小限值*/

  float qKp;                    /*Kp增量的影响系数*/

  float maxdKi;                 /*Ki增量的最大限值*/

  float mindKi;                 /*Ki增量的最小限值*/

  float qKi;                    /*Ki增量的影响系数*/

  float maxdKd;                 /*Kd增量的最大限值*/

  float mindKd;                 /*Kd增量的最小限值*/

  float qKd;                    /*Kd增量的影响系数*/

}VAGUEPID;
float LinearRealization(float max,float min,float x);
void VagueComputation (VAGUEPID *vPID,float pv,float *deltaK);
void CalcMembership(float *ms,float qv,int * index);
void LinearQuantization(VAGUEPID *vPID,float pv,float *qValue);

#endif