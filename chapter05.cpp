#pragma once
#include"opencv2/opencv.hpp" // iostream포함

using namespace std;
using namespace cv;

static void on_brightness_changed(int, void*);
static cv::Mat calcGrayHist(const cv::Mat&);
static cv::Mat getGrayHistImage(const cv::Mat& hist);

void example()
{
	cv::Mat img = cv::imread("Lenna1.png", cv::IMREAD_GRAYSCALE);	//ppt.38
	cv::Mat img2 = cv::imread("Lenna1.png", cv::IMREAD_COLOR);
	cv::Mat img3;
	cv::cvtColor(img2, img3, cv::COLOR_BGR2GRAY);	//이미지3 = 이미지2=>GRAYSCALE
	
	cv::imshow("IMG", img);
	cv::imshow("IMG2", img2);
	cv::imshow("IMG3", img3);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			//uchar temp = img.at <uchar>(i, j) + 50;	//안된다 오버플로우가 발생 0~255가 넘어갈시에는 
			uchar temp1 = img.at <uchar>(i, j);
			int temp2 = temp1 - 50;	//int temp2 = temp1 + 50;
			if (temp2 < 0)			//if (temp2 > 255)
			{
				temp2 = 0;			//temp2 = 255;
			}
			img.at<uchar>(i, j) = static_cast<uchar>(temp2);
		}
	}
	cv::imshow("changed", img);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void brightness()	//example()와 달리 범위를 넘어가는것을 자동으로 보정해줌
{
	cv::Mat src = cv::imread("dog.jpg",cv::IMREAD_GRAYSCALE);
	cv::Mat dst(src.rows, src.cols, src.type());
	//cv::Mat dst(512,512,CV_8UC1);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(src.at<uchar>(i, j) - 100);
		}
	}
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void brightness2()	//p.205
{
	cv::Mat src = cv::imread("cat.bmp",cv::IMREAD_GRAYSCALE);
	cv::namedWindow("CAT");				  //0 ~ 100단계
	cv::createTrackbar("CAT_BRIGHT", "CAT", 0, 100, on_brightness_changed, &src);
	cv::imshow("SRC", src);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void on_brightness_changed(int position, void* userdata)
{
	cv::Mat src = *(static_cast<cv::Mat*>(userdata));
	cv::Mat dst = src + position;
	cv::imshow("CANVAS", dst);
}

void brightness3()
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	float alpha = 1.0f;
	cv::Mat dst = (1 + alpha) * src - 128 * alpha;
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

cv::Mat calcGrayHist(const cv::Mat& img)
{
	cv::Mat hist;
	int channels[] = { 0 };
	int dims = 1;	//흑백
	int histSize[] = { 256 };
	float graylevel[] = { 0,256 };
	const float* ranges[] = { graylevel };
	cv::calcHist(&img, 1, channels, cv::noArray(), hist, dims, histSize, ranges);
	return hist;//	   영상의 개수						 차원:1,영상이 하나니까
}

cv::Mat getGrayHistImage(const cv::Mat& hist)
{
	double histMax;
	cv::minMaxLoc(hist, 0, &histMax);
	cv::Mat imgHist(100, 256, CV_8UC1, cv::Scalar(255));
	for (int i = 0; i < 256; i++)
	{
		cv::line(imgHist, cv::Point(i, 100), cv::Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), cv::Scalar(0));
	}
	return imgHist;
}

void show_hit()		//p.216,p.221
{					//히스토 그램: 0(어두움) ~ 255(밝음)
	cv::Mat src = cv::imread("hawkes.bmp", cv::IMREAD_GRAYSCALE);
	//람다 함수     [](){		};  함수 안에 함수만들어 쓸 수 있음, 한번만 쓸꺼라서
	//auto calcGrayHist = [](const cv::Mat& img) {
	//	cv::Mat hist;
	//	int channels[] = { 0 };
	//	int dims = 1;	//흑백
	//	int histSize[] = {256};
	//	float graylevel[] = { 0,256 };
	//	const float* ranges[] = { graylevel };
	//	cv::calcHist(&img, 1, channels, cv::noArray(), hist, dims, histSize, ranges);
	//	return hist;//	   영상의 개수						  차원:1,영상이 하나니까
	//};
	cv::Mat hist1 = calcGrayHist(src);

	/*auto getGrayHistImage = [](const cv::Mat& hist)				//로컬함수
	{
		double histMax;
		cv::minMaxLoc(hist, 0, &histMax);
		cv::Mat imgHist(100, 256, CV_8UC1, cv::Scalar(255));
		for (int i = 0; i < 256; i++)
		{
			cv::line(imgHist, cv::Point(i, 100), cv::Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), cv::Scalar(0));
		}
		return imgHist;
	};*/
	cv::Mat hist_img = getGrayHistImage(hist1);
	cv::imshow("SRC", src);
	cv::imshow("HISTOGRAM", hist_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void histogram_stretching()	//p,222
{
	cv::Mat src = cv::imread("hawkes.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	double gmin = 0.0;
	double gmax = 0.0;
	cv::minMaxLoc(src,&gmin, &gmax);	//src에서 가장 작은 값과 가장 큰 값을 찾아줌
	cv::Mat dst = ((src - gmin) / (gmax - gmin)) * 255;		//p.221공식
	cv::Mat hist1 = calcGrayHist(src);
	cv::Mat hist_img = getGrayHistImage(hist1);
	cv::imshow("SRC", src);
	cv::imshow("HISTOGRAM", hist_img);

	cv::imshow("DST", dst);
	cv::imshow("DST_IMG", getGrayHistImage(calcGrayHist(dst)));

	cv::waitKey(0);
	cv::destroyAllWindows();
}