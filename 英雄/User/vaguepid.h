#ifndef _vaguePID_
#define _vaguePID_
//ģ��PID
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
�����,��ά��E,��ά��EC
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
  float setpoint;               /*�趨ֵ*/

  float kp;                     /*����ϵ��*/

  float ki;                     /*����ϵ��*/

  float kd;                     /*΢��ϵ��*/

  float lasterror;              /*ǰһ��ƫ��*/

  float preerror;               /*ǰ����ƫ��*/

  float deadband;               /*����*/

  float output;                 /*���ֵ*/

  float result;                 /*���������ֵ*/

  float maximum;                /*���ֵ������*/

  float minimum;                /*���ֵ������*/

  float maxdKp;                 /*Kp�����������ֵ*/

  float mindKp;                 /*Kp��������С��ֵ*/

  float qKp;                    /*Kp������Ӱ��ϵ��*/

  float maxdKi;                 /*Ki�����������ֵ*/

  float mindKi;                 /*Ki��������С��ֵ*/

  float qKi;                    /*Ki������Ӱ��ϵ��*/

  float maxdKd;                 /*Kd�����������ֵ*/

  float mindKd;                 /*Kd��������С��ֵ*/

  float qKd;                    /*Kd������Ӱ��ϵ��*/

}VAGUEPID;
float LinearRealization(float max,float min,float x);
void VagueComputation (VAGUEPID *vPID,float pv,float *deltaK);
void CalcMembership(float *ms,float qv,int * index);
void LinearQuantization(VAGUEPID *vPID,float pv,float *qValue);

#endif