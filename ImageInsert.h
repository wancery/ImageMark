//
// Created by wxrui on 2017/8/8.
//

#ifndef IMAGEMARK_IMAGEINSERT_H
#define IMAGEMARK_IMAGEINSERT_H

#include <iostream>
#include "ConstW.h"

using namespace std;

//水印嵌入总调用函数
bool imageInsert(string srcfile, string dstfile, string wkfile);

//扩频处理,得到指纹 40*256
void
selectFinger(float *Ypart, int with, int height, float *rsCode, float fingerlib[FINGERLIB][FINGERLIB], float *finger);

//卷积函数,用于计算nvf
void juanji(float *Ypart, float *zsame, float *LocalMean, int width, int height);

//计算噪声屏蔽函数nvf
void getNVF(float *Ypart, float *nvf, int width, int height);

//根据finger 40*256 生成模板 h*w
void getTemeplate(float *finger, float *temeplate, int width, int height);

//将模板 h*w 嵌入到Ypart
void doInsert(float *Ypart, float *temeplate, int width, int height);


#endif //IMAGEMARK_IMAGEINSERT_H
