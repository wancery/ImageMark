//
// Created by wxrui on 2017/8/8.
//

#include "ImageInsert.h"
#include "ConstW.h"
#include "ZigZag.h"
#include "RS/ecc.h"
#include "Common.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;


//水印嵌入总调用函数
bool imageInsert(string srcfile, string dstfile, string wkfile) {
    //Ypart 0 单通道
    Mat srcimg = imread(srcfile, 0);
    if (srcimg.empty())
        cout << "read picture error" << endl;
    srcimg.convertTo(srcimg, CV_32F);
    int height = srcimg.rows;
    int width = srcimg.cols;
    float *Ypart = new float[height * width];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            Ypart[i * width + j] = srcimg.at<float>(i, j);
    //finger
    float *wk = new float[WKLENGTH];
    memset(wk, 0, sizeof(float) * WKLENGTH);
    if (!getWK(wkfile, wk))
        cout << "read WK error" << endl;
    float fingerlib[FINGERLIB][FINGERLIB];
    if (!getFingerLib(fingerlib))
        cout << "read fingerlib error" << endl;
    float *rsCode = new float[RSLENGTH];
    /****************rsCode <-- wk****************************************/
    unsigned char inputarr[WKLENGTH];
    for (int i = 0; i < WKLENGTH; i++)
        inputarr[i] = wk[i] + '0';
    unsigned char code[RSLENGTH];
    initialize_ecc();
    encode_data(inputarr, code);
    for (int i = 0; i < RSLENGTH; i++)
        rsCode[i] = code[i] - '0';
    /****************rsCode <-- wk****************************************/
    float *finger = new float[FINGERLENGTH];
    memset(finger, 0, sizeof(float) * FINGERLENGTH);
    selectFinger(Ypart, width, height, rsCode, fingerlib, finger);
    //temeplate h*w
    float *temeplate = new float[width * height];
    memset(temeplate, 0, sizeof(float) * width * height);
    getTemeplate(finger, temeplate, width, height);
    //insert
    doInsert(Ypart, temeplate, width, height);
    //write back
    Mat dstimg(srcimg.size(), CV_32F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            dstimg.at<float>(i, j) = Ypart[i * width + j];
    dstimg.convertTo(dstimg, CV_8U);
    imwrite(dstfile, dstimg);
    delete[] Ypart;
    delete[] wk;
    delete[] rsCode;
    delete[] finger;
    delete[] temeplate;
}

//扩频处理,得到指纹 40*256
void
selectFinger(float *Ypart, int width, int height, float *rsCode, float fingerlib[FINGERLIB][FINGERLIB], float *finger) {
    //dct zigzag
    Mat temp(height, width, CV_32FC1);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            temp.at<float>(i, j) = Ypart[i * width + j];
    resize(temp, temp, Size(standW, standH));
    dct(temp, temp);
    float *dct_Ypart = new float[standW * standH];
    for (int i = 0; i < standH; i++)
        for (int j = 0; j < standW; j++)
            dct_Ypart[i * standW + j] = temp.at<float>(i, j);
    float *zig_dct = new float[standW * standH];
    zigZag(zig_dct, dct_Ypart, standW, standH);
    //finger
    int group;
    float *sim = new float[16];
    float *zigPart = new float[FINGERLIB];
    int max_sim_index;
    for (int i = 0; i < RSLENGTH / 4; i++) {
        memset(sim, 0, sizeof(float) * 16);
        memcpy(zigPart, zig_dct + INSPOSITION + i * FINGERLIB, sizeof(float) * FINGERLIB);
        //group 0--15
        group = 8 * rsCode[i * 4] + 4 * rsCode[i * 4 + 1] + 2 * rsCode[i * 4 + 2] + rsCode[i * 4 + 3];
        //fingerlib group*16--group*16+15
        for (int j = group * 16; j < (group + 1) * 16; j++) {
            sim[j % 16] = similar(fingerlib[j], zigPart, FINGERLIB);
            if (j % 16 == 0)
                max_sim_index = 0;
            else if (sim[j % 16] > sim[max_sim_index])
                max_sim_index = j % 16;
        }
        memcpy(finger + i * FINGERLIB, fingerlib[group * 16 + max_sim_index], sizeof(float) * FINGERLIB);
    }
    delete[] dct_Ypart;
    delete[] zig_dct;
    delete[] sim;
    delete[] zigPart;
}

//卷积函数,用于计算nvf
void juanji(float *Ypart, float *zsame, float *LocalMean, int width, int height) {
    int i, j;
    float *newX_2 = new float[(width + 4) * (height + 4)];
    memset(newX_2, 0, sizeof(float) * (width + 4) * (height + 4));
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            newX_2[(i + 2) * (width + 4) + j + 2] = Ypart[i * width + j] * Ypart[i * width + j];
    //先将x进行扩充,上下(左右)加2行(列)
    float *newX = new float[(width + 4) * (height + 4)];
    memset(newX, 0, sizeof(float) * (width + 4) * (height + 4));
    for (j = 0; j < height; j++)
        memcpy(newX + (j + 2) * (width + 4) + 2, Ypart + j * width, sizeof(float) * width);
    const int const_col = width + 4;
    for (i = 0; i < height; i++) {
        //每右移一格，只增加一列
        const int base_row_self = (i + 2) * const_col;
        const int base_row_up = (i + 1) * const_col;
        const int base_row_down = (i + 3) * const_col;
        float col_1 = newX[base_row_self + 1] + newX[base_row_up + 1] + newX[base_row_down + 1];
        float col_2 = newX[base_row_self + 2] + newX[base_row_up + 2] + newX[base_row_down + 2];
        float colZ_1 = newX_2[base_row_self + 1] + newX_2[base_row_up + 1] + newX_2[base_row_down + 1];
        float colZ_2 = newX_2[base_row_self + 2] + newX_2[base_row_up + 2] + newX_2[base_row_down + 2];
        int index1 = i * width;
        for (j = 0; j < width; j++, index1++) {
            float col_3 = newX[base_row_self + j + 3] + newX[base_row_up + j + 3] + newX[base_row_down + j + 3];
            float colZ_3 = newX_2[base_row_self + j + 3] + newX_2[base_row_up + j + 3] + newX_2[base_row_down + j + 3];
            LocalMean[index1] = col_1 + col_2 + col_3;
            LocalMean[index1] /= 9;
            zsame[index1] = colZ_1 + colZ_2 + colZ_3;
            zsame[index1] /= 9;
            col_1 = col_2;
            col_2 = col_3;
            colZ_1 = colZ_2;
            colZ_2 = colZ_3;
        }
    }
    delete[] newX_2;
    delete[] newX;
}

//计算噪声屏蔽函数nvf
void getNVF(float *Ypart, float *nvf, int width, int height) {
    int i = 0;
    int j = 0;
    //第一次求均值卷积时，保存结果
    float *LocalMean = new float[width * height];
    memset(LocalMean, 0, sizeof(float) * width * height);
    //第二次求平方卷积时，保存结果
    float *zsame = new float[width * height];
    memset(zsame, 0, sizeof(float) * width * height);
    float *LocalVar = new float[width * height];
    memset(LocalVar, 0, sizeof(float) * width * height);
    juanji(Ypart, zsame, LocalMean, width, height);
    float maxvar = -1;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int index = i * width + j;
            LocalVar[index] = zsame[index] - LocalMean[index] * LocalMean[index];
            if (LocalVar[index] > maxvar) {
                maxvar = LocalVar[index];
            }
        }
    }
    const float Th0 = 150;
    const float Th = Th0 / maxvar;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int index = i * width + j;
            nvf[index] = 1 / (1 + Th * LocalVar[index]);
        }
    }
    delete[] zsame;
    delete[] LocalMean;
    delete[] LocalVar;
}

//根据finger 40*256 生成模板 h*w
void getTemeplate(float *finger, float *temeplate, int width, int height) {
    //在嵌入位置插入指纹
    float *wk = new float[standW * standH];
    memset(wk, 0, sizeof(float) * standW * standH);
    memcpy(wk + INSPOSITION, finger, sizeof(float) * FINGERLENGTH);
    //izigzag 扫描
    float *izigM = new float[standW * standH];
    memset(izigM, 0, sizeof(float) * standW * standH);
    izigZag(izigM, wk, standW, standH);
    //创建单通道Mat
    Mat WKTemp(standH, standW, CV_32FC1);
    for (int i = 0; i < standH; i++)
        for (int j = 0; j < standW; j++)
            WKTemp.at<float>(i, j) = izigM[i * standW + j];
    //idct变换
    Mat temp(WKTemp.size(), CV_32FC1);
    idct(WKTemp, temp);
    resize(temp, temp, Size(width, height));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            temeplate[i * width + j] = temp.at<float>(i, j);
    delete[] wk;
}

//将模板 h*w 嵌入到Ypart
void doInsert(float *Ypart, float *temeplate, int width, int height) {
    float *mask = new float[width * height];
    float *nvf = new float[width * height];
    getNVF(Ypart, nvf, width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mask[i * width + j] = alpha * (17 * (1 - nvf[i * width + j]) + 3 * nvf[i * width + j]);
            Ypart[i * width + j] += mask[i * width + j] * temeplate[i * width + j];
        }
    }
    delete[] nvf;
    delete[] mask;
}

