//
// Created by wxrui on 2017/8/9.
//

#ifndef IMAGEMARK_DETECTBLIND_H
#define IMAGEMARK_DETECTBLIND_H

#include <iostream>
#include "ConstW.h"

using namespace std;

//盲检测函数
float detectBlind(string dstfile, string wkfile);

//finger 40*256  ---> rsCode 40*4
void getRS(float *extFinger, float *rsCode);


#endif //IMAGEMARK_DETECTBLIND_H
