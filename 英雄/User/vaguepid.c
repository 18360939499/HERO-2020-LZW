#include "vaguepid.h"
/*
����ʵ�����У�����ϵͳ����������ʱ����ģ�������Ҫ��̬����PID�Ĳ�����
��ģ�����������ܹ�������һ����,ģ��PID�������ǽ�ģ���㷨��PID���Ʋ���
�����������ϵ�һ�ֿ����㷨
ԭ��
ģ���㷨��һ�ֻ�������������㷨����Ȼ��֮Ϊģ���㷨��ʵ����ģ����ʵ��
����һ�����󾫵�˼�롣һ��ģ����������Ҫ����ģ������ģ��������;�ȷ
����������ģ���֪ʶ�⣨�������ݿ�͹���⣩����
���裺
1������������
2ģ����
3�����
4�����
5��ȷ��
6��������
*/
/*
���ã��������������������Ժ�������ʹ�������½�����̬�ֲ���һԪ������
ԭ��ͨ������������������Ͷ�䵽һ�������ּ�����0�Գƣ�
����[NB]������[NM]����С[NS]����[ZO]����С[PS]������[PM]������[PB]
�ؼ��㣺���㾫�ȣ����䶨��
*/
void LinearQuantization(VAGUEPID *vPID,float pv,float *qValue)
{
  float thisError;
  float deltaError;
  thisError=vPID->setpoint-pv;                  //����ƫ��ֵ
  deltaError=thisError-vPID->lasterror;         //����ƫ�������
  qValue[0]=6.0*thisError/(vPID->maximum-vPID->minimum);
  qValue[1]=3.0*deltaError/(vPID->maximum-vPID->minimum);
}
/*
���ã������ȼ��㺯��
ԭ�����������Ⱥ���
�ؼ��㣺
*/
void CalcMembership(float *ms,float qv,int * index)
{

  if((qv>=-NB)&&(qv<-NM))

  {
		//����������ʱ��ϵ��
    index[0]=0;

    index[1]=1;
		//�����Ĳ�ͬ����Ϊ���䲻ͬ�����Խؾ಻ͬ������ʵб������ͬ��
    ms[0]=-0.5*qv-2.0;  //y=-0.5x-2.0

    ms[1]=0.5*qv+3.0;   //y=0.5x+3.0

  }

  else if((qv>=-NM)&&(qv<-NS))

  {

    index[0]=1;

    index[1]=2;

    ms[0]=-0.5*qv-1.0;  //y=-0.5x-1.0

    ms[1]=0.5*qv+2.0;   //y=0.5x+2.0

  }

  else if((qv>=-NS)&&(qv<ZO))

  {

    index[0]=2;

    index[1]=3;

    ms[0]=-0.5*qv;      //y=-0.5x

    ms[1]=0.5*qv+1.0;   //y=0.5x+1.0

  }

  else if((qv>=ZO)&&(qv<PS))

  {

    index[0]=3;

    index[1]=4;

    ms[0]=-0.5*qv+1.0;  //y=-0.5x+1.0

    ms[1]=0.5*qv;       //y=0.5x

  }

  else if((qv>=PS)&&(qv<PM))

  {

    index[0]=4;

    index[1]=5;

    ms[0]=-0.5*qv+2.0;  //y=-0.5x+2.0

    ms[1]=0.5*qv-1.0;   //y=0.5x-1.0

  }

  else if((qv>=PM)&&(qv<=PB))

  {

    index[0]=5;

    index[1]=6;

    ms[0]=-0.5*qv+3.0;  //y=-0.5x+3.0

    ms[1]=0.5*qv-2.0;   //y=0.5x-2.0

  }

}
/*
���ã���ģ��������
ԭ����������ȷ���������򵥣������ڿ���Ҫ�󲻸߳��ϡ�
���ķ����������ƽ�����������Ѷȴ�
��Ȩƽ��������һ���ڹ�ҵ��Ӧ����㷺
�ؼ��㣺
*/
void VagueComputation (VAGUEPID *vPID,float pv,float *deltaK)
{
	float qValue[2]={0,0};        //ƫ�������������ֵ

  int indexE[2]={0,0};          //ƫ������������

  float msE[2]={0,0};           //ƫ��������

  int indexEC[2]={0,0};         //ƫ����������������

  float msEC[2]={0,0};          //ƫ������������

  float qValueK[3];
  LinearQuantization(vPID,pv,qValue);
  CalcMembership(msE,qValue[0],indexE);

  CalcMembership(msEC,qValue[1],indexEC);

  qValueK[0]=msE[0]*(msEC[0]*ruleKp[indexE[0]][indexEC[0]]+msEC[1]*ruleKp[indexE[0]][indexEC[1]])

            +msE[1]*(msEC[0]*ruleKp[indexE[1]][indexEC[0]]+msEC[1]*ruleKp[indexE[1]][indexEC[1]]);

  qValueK[1]=msE[0]*(msEC[0]*ruleKi[indexE[0]][indexEC[0]]+msEC[1]*ruleKi[indexE[0]][indexEC[1]])

            +msE[1]*(msEC[0]*ruleKi[indexE[1]][indexEC[0]]+msEC[1]*ruleKi[indexE[1]][indexEC[1]]);

  qValueK[2]=msE[0]*(msEC[0]*ruleKd[indexE[0]][indexEC[0]]+msEC[1]*ruleKd[indexE[0]][indexEC[1]])

            +msE[1]*(msEC[0]*ruleKd[indexE[1]][indexEC[0]]+msEC[1]*ruleKd[indexE[1]][indexEC[1]]);

 

  deltaK[0]=LinearRealization(vPID->maxdKp,vPID->mindKp,qValueK[0]);

  deltaK[1]=LinearRealization(vPID->maxdKi,vPID->mindKi,qValueK[1]);

  deltaK[2]=LinearRealization(vPID->maxdKd,vPID->mindKd,qValueK[2]);

}
float LinearRealization(float max,float min,float x){
	if ( x > max)
		return max;
	if( x< min)
		return min;
	else 
		return x;
}