//
// Created by wxrui on 2017/8/11.
//

#include <opencv2/core/hal/interface.h>
#include <opencv/cv.h>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "PSNR.h"

using namespace std;
using namespace cv;

float getPSNR(string srcfile, string dstfile) {
    Mat srcimg = imread(srcfile);
    Mat dstimg = imread(dstfile);
    Mat difmat;
    absdiff(srcimg, dstimg, difmat);
    difmat.convertTo(difmat, CV_32F);
    difmat = difmat.mul(difmat);

    Scalar s = sum(difmat);
    float sse = s.val[0] + s.val[1] + s.val[2];

    if (sse <= 1e-10)
        return 0;
    else {
        float mse = sse / (float) (srcimg.channels() * srcimg.total());
        float psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;//返回PSNR
    }

}