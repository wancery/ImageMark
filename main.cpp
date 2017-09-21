#include <iostream>
#include <fstream>
#include "ImageInsert.h"
#include "DetectBlind.h"
#include "PSNR.h"

using namespace std;


int main() {
    int menu;
    cin >> menu;
    string img_size = "1920";
    if (menu == 1) {
        //生成水印图片
        string srcstr = "material\\" + img_size + "\\ori\\(";
        string disstr = "material\\" + img_size + "\\wk\\(";
        string wkfile = "wk.txt";
        for (int i = 1; i <= 20; i++) {
            string srcfile = srcstr + to_string(i) + ").bmp";
            string dstfile = disstr + to_string(i) + ").bmp";
            imageInsert(srcfile, dstfile, wkfile);
        }
    } else if (menu == 2) {
        //计算PSNR
        ofstream output("result\\" + img_size + "\\PSNR.txt", ios_base::out);
        string srcstr = "material\\" + img_size + "\\ori\\(";
        string disstr = "material\\" + img_size + "\\wk\\(";
        for (int i = 1; i <= 20; i++) {
            string srcfile = srcstr + to_string(i) + ").bmp";
            string dstfile = disstr + to_string(i) + ").bmp";
            output << i << "  " << getPSNR(srcfile, dstfile) << endl;
        }
        output.close();
    } else if (menu == 3) {
        ofstream output;
        string res_ = "result\\" + img_size + "\\";
        string resfile[] = {"none.txt", "comp.txt", "crop.txt", "filter.txt", "noise.txt", "resize.txt", "wk.txt"};
        string dst_ = "material\\" + img_size + "\\";
        string dstfile[] = {"ori\\(", "comp\\(", "crop\\(", "filter\\(", "noise\\(", "resize\\(", "wk\\("};
        string wkfile = "wk.txt";
        //攻击检测
        string filename;
        for (int t = 0; t < 7; t++) {
            output.open(res_ + resfile[t], ios_base::out);
            for (int i = 1; i <= 20; i++) {
                if (t != 1)
                    filename = dst_ + dstfile[t] + to_string(i) + ").bmp";
                else
                    filename = dst_ + dstfile[t] + to_string(i) + ").jpg";
                output << i << "  " << detectBlind(filename, wkfile) << endl;
            }
            output << endl << endl << endl;
            output.close();
        }
    } else {

    }
    return 0;
};