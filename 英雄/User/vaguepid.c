#include "vaguepid.h"
/*
在现实控制中，被控系统并非是线性时不变的，往往需要动态调整PID的参数，
而模糊控制正好能够满足这一需求,模糊PID控制器是将模糊算法与PID控制参数
的自整定相结合的一种控制算法
原理：
模糊算法是一种基于智能推理的算法，虽然称之为模糊算法其实并不模糊，实际
上是一种逐步求精的思想。一个模糊控制器主要是由模糊化，模糊推理机和精确
化三个功能模块和知识库（包括数据库和规则库）构成
步骤：
1输入量的量化
2模糊化
3规则库
4推理机
5精确化
6工程量化
*/
/*
作用：对输入量的量化，线性函数（可使用最速下降，正态分布等一元函数）
原理：通过量化函数将输入量投射到一定的数字级别，以0对称：
负大[NB]、负中[NM]、负小[NS]、零[ZO]、正小[PS]、正中[PM]、正大[PB]
关键点：计算精度，区间定义
*/
void LinearQuantization(VAGUEPID *vPID,float pv,float *qValue)
{
  float thisError;
  float deltaError;
  thisError=vPID->setpoint-pv;                  //计算偏差值
  deltaError=thisError-vPID->lasterror;         //计算偏差的增量
  qValue[0]=6.0*thisError/(vPID->maximum-vPID->minimum);
  qValue[1]=3.0*deltaError/(vPID->maximum-vPID->minimum);
}
/*
作用：隶属度计算函数
原理：三角隶属度函数
关键点：
*/
void CalcMembership(float *ms,float qv,int * index)
{

  if((qv>=-NB)&&(qv<-NM))

  {
		//计算隶属度时的系数
    index[0]=0;

    index[1]=1;
		//函数的不同是因为区间不同（所以截距不同），其实斜率是相同的
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
作用：解模糊化操作
原理：最大隶属度法——计算简单，适用于控制要求不高场合。
重心法——输出更平滑，但计算难度大
加权平均法——一般在工业上应用最广泛
关键点：
*/
void VagueComputation (VAGUEPID *vPID,float pv,float *deltaK)
{
	float qValue[2]={0,0};        //偏差及其增量的量化值

  int indexE[2]={0,0};          //偏差隶属度索引

  float msE[2]={0,0};           //偏差隶属度

  int indexEC[2]={0,0};         //偏差增量隶属度索引

  float msEC[2]={0,0};          //偏差增量隶属度

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