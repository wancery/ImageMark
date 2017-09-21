//
// Created by wxrui on 2017/8/11.
//

#include <fstream>
#include <math.h>
#include "Common.h"

//提取原始水印 128
bool getWK(string wkfile, float *wk) {
    ifstream input(wkfile);
    if (!input) {
        return false;
    }
    for (int i = 0; i < WKLENGTH; i++) {
        input >> *(wk + i);
    }
    input.close();
    return true;
}

//读取指纹库
bool getFingerLib(float fingerlib[FINGERLIB][FINGERLIB]) {
    ifstream input("fingerlib.txt");
    if (!input) {
        return false;
    }
    for (int i = 0; i < FINGERLIB; i++)
        for (int j = 0; j < FINGERLIB; j++)
            input >> fingerlib[i][j];
    input.close();
    return true;

}

//计算相关值sim
float similar(float *ori, float *ext, int len) {
    float sim = 0;
    float norm_ori, norm_ext;
    for (int i = 0; i < len; i++) {
        sim += (*(ori + i)) * (*(ext + i));
    }
    norm_ori = norm(ori, len);
    norm_ext = norm(ext, len);
    if (!norm_ext) {
        cout << "norm_ext===0" << endl;
        return 0;
    } else if (!norm_ori) {
        cout << "norm_ori===0" << endl;
        return 0;
    } else {
        sim /= (norm_ori * norm_ext);
        return sim;
    }
}

//norm
float norm(float *temp, int len) {
    int i = 0;
    double sum = 0;
    for (; i < len; i++)
        sum += (*(temp + i)) * (*(temp + i));
    sum = sqrtf(sum);
    return sum;
}

//distance
float distance(float *ori, float *ext, int len) {
    float res = 0;
    float sum = 0;
    for (int i = 0; i < len; i++)
        sum += ((*(ori + i)) - (*(ext + i))) * ((*(ori + i)) - (*(ext + i)));
    res = 1 - sqrtf(sum) / sqrtf(len);
    return res;
}

//1-bit error rate
float BER(float *ori, float *ext, int len) {
    float res = 0;
    float sum = 0;
    for (int i = 0; i < len; i++)
        if (*(ori + i) != *(ext + i))
            sum++;
    res = 1 - sum / len;
    return res;
}





