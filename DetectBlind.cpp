//
// Created by wxrui on 2017/8/9.
//

#include "ConstW.h"
#include "ZigZag.h"
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "DetectBlind.h"
#include "Common.h"
#include "RS/ecc.h"

using namespace std;
using namespace cv;

//盲检测函数
float detectBlind(string dstfile, string wkfile) {
    Mat dstimg = imread(dstfile, 0);
    if (dstimg.empty())
        cout << "read dstimg error!" << endl;
    dstimg.convertTo(dstimg, CV_32F);
    resize(dstimg, dstimg, Size(standW, standH));
    dct(dstimg, dstimg);
    float *dct_Y = new float[standW * standH];
    for (int i = 0; i < standH; i++)
        for (int j = 0; j < standW; j++)
            dct_Y[i * standW + j] = dstimg.at<float>(i, j);
    float *dct_Y1D = new float[standW * standH];
    zigZag(dct_Y1D, dct_Y, standW, standH);
    float *extFinger = new float[FINGERLENGTH];
    for (int i = 0; i < FINGERLENGTH; i++)
        extFinger[i] = dct_Y1D[INSPOSITION + i];
    float *rsCode = new float[RSLENGTH];
    getRS(extFinger, rsCode);
    float *extWK = new float[WKLENGTH];
    /****************rsCode --> extwk****************************************/
    unsigned char code[RSLENGTH];
    for (int i = 0; i < RSLENGTH; i++)
        code[i] = rsCode[i] + '0';
    unsigned char outputarr[WKLENGTH];
    initialize_ecc();
    decode_data(code, outputarr);
    for (int i = 0; i < WKLENGTH; i++)
        extWK[i] = outputarr[i] - '0';
    /****************rsCode -->extwk****************************************/
    float *wk = new float[WKLENGTH];
    if (!getWK(wkfile, wk))
        cout << "read WK error" << endl;
    float sim = distance(wk, extWK, WKLENGTH);
    delete[] dct_Y;
    delete[] dct_Y1D;
    delete[] extFinger;
    delete[] rsCode;
    delete[] extWK;
    delete[] wk;
    return sim;
}

//finger 40*256  ---> rsCode 40*4
void getRS(float *extFinger, float *rsCode) {
    float fingerLib[FINGERLIB][FINGERLIB];
    getFingerLib(fingerLib);
    float *fingerPart = new float[FINGERLIB];
    float *sim = new float[FINGERLIB];
    int maxsim;
    for (int i = 0; i < RSLENGTH / 4; i++) {
        memcpy(fingerPart, extFinger + i * FINGERLIB, sizeof(float) * FINGERLIB);
        for (int j = 0; j < FINGERLIB; j++) {
            sim[j] = similar(fingerLib[j], fingerPart, FINGERLIB);
            if (j == 0)
                maxsim = 0;
            else if (sim[j] > sim[maxsim])
                maxsim = j;
        }
        maxsim /= 16;
        rsCode[i * 4] = maxsim / 8;
        rsCode[i * 4 + 1] = (maxsim % 8) / 4;
        rsCode[i * 4 + 2] = ((maxsim % 8) % 4) / 2;
        rsCode[i * 4 + 3] = (((maxsim % 8) % 4) % 2);
    }
    delete[] fingerPart;
    delete[] sim;
}
