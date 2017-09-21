//
// Created by wxrui on 2017/7/18.
//

#include <cstring>
#include "ZigZag.h"

void zigZag(float *tarMat, float *srcMat, int width, int height) {
    int h = 1, v = 1;
    int vmin = 1, hmin = 1;
    int vmax = height, hmax = width;
    int i;

    memset(tarMat, 0, sizeof(float) * height * width);

    i = 1;
    while (v <= vmax && h <= hmax) {
        if ((h + v) % 2 == 0) {
            if (v == vmin) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);

                if (h == hmax)
                    v = v + 1;
                else
                    h = h + 1;

                i = i + 1;
            } else if ((h == hmax) && (v < vmax)) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
                v = v + 1;
                i = i + 1;
            } else if ((v > vmin) && (h < hmax)) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
                v = v - 1;
                h = h + 1;
                i = i + 1;
            }
        } else {
            if ((v == vmax) && (h <= hmax)) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
                h = h + 1;
                i = i + 1;
            } else if (h == hmin) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
                if (v == vmax)
                    h = h + 1;
                else
                    v = v + 1;
                i = i + 1;
            } else if ((v < vmax) && (h > hmin)) {
                tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
                v = v + 1;
                h = h - 1;
                i = i + 1;
            }
        }

        if ((v == vmax) && (h == hmax)) {
            tarMat[i - 1] = *(srcMat + width * (v - 1) + h - 1);
            break;
        }
    }

}


void izigZag(float *tarMat, float *srcMat, int width, int height) {
    int h = 1, v = 1;
    int vmin = 1, hmin = 1;
    int vmax = height, hmax = width;
    int i;
    memset(tarMat, 0, sizeof(float) * height * width);
    i = 1;
    while (v <= vmax && h <= hmax) {
        if ((h + v) % 2 == 0) {
            if (v == vmin) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];

                if (h == hmax)
                    v = v + 1;
                else
                    h = h + 1;

                i = i + 1;
            } else if ((h == hmax) && (v < vmax)) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
                v = v + 1;
                i = i + 1;
            } else if ((v > vmin) && (h < hmax)) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
                v = v - 1;
                h = h + 1;
                i = i + 1;
            }
        } else {
            if ((v == vmax) && (h <= hmax)) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
                h = h + 1;
                i = i + 1;
            } else if (h == hmin) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
                if (v == vmax)
                    h = h + 1;
                else
                    v = v + 1;
                i = i + 1;
            } else if ((v < vmax) && (h > hmin)) {
                *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
                v = v + 1;
                h = h - 1;
                i = i + 1;
            }
        }

        if ((v == vmax) && (h == hmax)) {
            *(tarMat + width * (v - 1) + h - 1) = srcMat[i - 1];
            break;
        }
    }
}
