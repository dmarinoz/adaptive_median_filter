#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
using namespace std;
using namespace cv;

uchar adaptiveProcess(const Mat &im, int row, int col, int kernelSize, int maxSize)
{
    vector <uchar> pixels;
    for(int a = -kernelSize / 2; a <= kernelSize / 2; a++){
        for(int b = -kernelSize / 2; b <= kernelSize / 2; b++){
            pixels.push_back(im.at<uchar>(row + a, col + b));
        }
    }
    sort(pixels.begin(), pixels.end());
    auto min = pixels[0];
    auto max = pixels[kernelSize * kernelSize - 1];
    auto med = pixels[kernelSize * kernelSize / 2];
    auto zxy = im.at<uchar>(row, col);
    if(med > min && med < max){
        if(zxy > min && zxy < max){
            return zxy;
        }else{
            return med;
        }
    }
    else{
        kernelSize += 2;
        if(kernelSize <= maxSize)
            return adaptiveProcess(im, row, col, kernelSize, maxSize);
        else
            return med;
    }
}

Mat work(Mat src){
    Mat dst;
    int minSize = 3;
    int maxSize = 7;
    copyMakeBorder(src, dst, maxSize / 2, maxSize / 2, maxSize / 2, maxSize / 2, BORDER_REFLECT);
    int rows = dst.rows;
    int cols = dst.cols;
    for(int j = maxSize / 2; j < rows - maxSize / 2; j++){
        for(int i = maxSize / 2; i < cols * dst.channels() - maxSize / 2; i++){
            dst.at<uchar>(j, i) = adaptiveProcess(dst, j, i, minSize, maxSize);
        }
    }
    return dst;
}

int main(){
    // Inicia el reloj
    clock_t start, end;
    start = clock();

    Mat src = cv::imread("sample_corrupted.bmp",0);
    Mat dst = work(src);

    // Tiempo de ejecución
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "El tiempo que ha tomado el programa es : " << fixed << time_taken << setprecision(5);
    cout << " segundos " << endl;

    Mat smp = cv::imread("sample.bmp", 0);


    imshow("Imagen original", smp);
    moveWindow("Imagen original", 100,200);

    imshow("Imagen corrupta", src);
    moveWindow("Imagen corrupta", 700,200);

    imshow("Imagen filtrada", dst);
    moveWindow("Imagen filtrada", 1300,200);
    //imwrite("../result.jpg", dst);
    waitKey(0);
}
