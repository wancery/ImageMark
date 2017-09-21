//
// Created by wxrui on 2017/8/11.
//

#ifndef IMAGEMARK_COMMON_H
#define IMAGEMARK_COMMON_H

#include <iostream>
#include "ConstW.h"

using namespace std;

//提取原始水印 128
bool getWK(string wkfile, float *wk);

//读取指纹库
bool getFingerLib(float fingerlib[FINGERLIB][FINGERLIB]);

//计算相关值sim
float similar(float *temp1, float *temp2, int len);

//norm
float norm(float *temp, int len);

//distance
float distance(float *ori, float *ext, int len);

//1-BER
float BER(float *ori, float *ext, int len);


#endif //IMAGEMARK_COMMON_H
