#include <functional>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>
#include <vector>

using namespace cv;
using namespace std;

int width, height, n;

void loopOverMat(Mat &img, vector<Mat> &vec, int num,
                 function<void(int, int, int, Mat &, vector<Mat> &)> &&func) {
  static int shift = width / n;
  for (int i = 0; i < height; ++i)
    for (int k = num; (k % shift) != (shift - 1); ++k)
      for (int ch = 0; ch < 3; ++ch)
        func(i, k, ch, img, vec);
}

void fmap(Mat &img, vector<Mat> &vec, int num) {
  loopOverMat(img, vec, num,
              [](int i, int k, int ch, Mat &img, vector<Mat> &vec) {
                ++(vec[img.at<Vec3b>(i, k)[ch]].at<Vec3s>(i, k)[ch]);
              });
}

void fhist(Mat &img, vector<Mat> &vec, int num) {
  loopOverMat(img, vec, num,
              [](int i, int k, int ch, Mat &img, vector<Mat> &vec) {
                int ind = 0;
                for (int j = 0; j < 256; ++j)
                  if (vec[ind].at<Vec3b>(i, k)[ch] < vec[j].at<Vec3s>(i, k)[ch])
                    ind = j;
                img.at<Vec3b>(i, k)[ch] = ind;
              });
}

void parallelize(function<void(Mat &, vector<Mat> &, int)> func, Mat &img,
                 vector<Mat> &vec) {
  static vector<thread> thrds(n);
  int cnt = 0;
  for (int k = 0; cnt < n; k += width / n)
    thrds[cnt++] = thread(func, ref(img), ref(vec), k);

  while (cnt)
    thrds[--cnt].join();
}

int main(int argv, char *str[]) {
  if (argv < 3)
    return -1;

  VideoCapture cam(str[1]);
  n = atoi(str[2]);

  if (!cam.isOpened())
    return -1;

  width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << width << 'x' << height << endl;

  vector<Mat> vec(256);

  for (auto &i : vec)
    i = Mat::zeros(height, width, CV_16UC3);

  Mat res(height, width, CV_8UC3), img(height, width, CV_8UC3);

  while (cam.read(img))
    parallelize(fmap, img, vec);

  VideoWriter video;
  video.open("out.avi", VideoWriter::fourcc('H', '2', '6', '4'), 30.0,
             img.size());

  for (auto &i : vec) {
    i.convertTo(img, CV_8U, 10, 0);
    video << img;
  }

  parallelize(fhist, res, vec);

  stringstream ss;
  ss << "test_" << width << 'x' << height << ".png";

  imwrite(ss.str(), res, {CV_IMWRITE_PNG_COMPRESSION, 0});

  return 0;
}
