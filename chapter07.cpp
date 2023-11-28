#pragma once
#include"opencv2/opencv.hpp" // iostream포함

using namespace std;
using namespace cv;

void filter_embossing()			//p.250
{
	cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	float filter_data[] = { -1.0f,-1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f,1.0f,1.0f };
	cv::Mat emboss(3, 3, CV_32FC1, filter_data);
	cv::Mat dst;
	cv::filter2D(src, dst, -1, emboss, cv::Point(-1, -1), 128);

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void filter_blurring()		///p.253
{
	cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat dst;
	cv::blur(src, dst, cv::Size(3, 3));
	float bulr_filter[] = { 1 / 9.f,1 / 9.f ,1 / 9.f ,1 / 9.f ,1 / 9.f ,1 / 9.f ,1 / 9.f ,1 / 9.f ,1 / 9.f };
	cv::Mat dst2(3, 3, CV_32FC1, bulr_filter);
	cv::Mat dst3;
	cv::filter2D(src, dst3, -1, dst2, cv::Point(-1, -1), 0);
	cv::Mat dst4;
	for (int ksize = 3; ksize <= 7; ksize = ksize + 2)
	{
		cv::blur(src, dst4, cv::Size(ksize, ksize));
		cv::String desc = cv::format("Bulr : %d X %d", ksize, ksize);
		cv::imshow(desc, dst4);
		cv::waitKey(0);
	}

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);
	cv::imshow("DST3", dst3);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void filter_gaussian()
{
	cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);
	cv::Mat dst;
	for (int sigma = 1; sigma <= 5; sigma++)
	{
		cv::GaussianBlur(src, dst, cv::Size(), static_cast<double>(sigma));
		cv::String text = cv::format("Sigma : %d ", sigma);

		cv::putText(dst, text, cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255), 1, cv::LINE_AA);

		cv::imshow(text, dst);
		cv::waitKey(0);
	}
	cv::destroyAllWindows();
}

void filter_unsharp_mask()
{
	cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);
	cv::Mat bulrred;
	float alpha = 1.0f;
	for (int sigma = 1; sigma <= 5; sigma++)
	{
		cv::GaussianBlur(src, bulrred, cv::Size(), sigma);
		cv::Mat dst = (1 + alpha) * src - (alpha * bulrred);
		cv::String text = cv::format("Sigma : %d", sigma);
		cv::putText(dst, text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
		cv::imshow(text, dst);
		cv::waitKey(0);
	}
	cv::destroyAllWindows();
}

void noise_gaussian()
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::imshow("Lenna", src);
	cv::Mat dst;
	for (int stddev = 10; stddev <= 30; stddev = stddev + 10)
	{
		cv::Mat noise(src.size(), CV_32SC1);
		randn(noise, 0, stddev);
		cv::add(src, noise, dst, cv::noArray(), CV_8UC1);
		cv::String text = cv::format("STDDEV : %d", stddev);
		cv::putText(dst, text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
		cv::imshow(text, dst);
		cv::waitKey(0);
	}
	cv::destroyAllWindows();
}	

void filter_bilateral()
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);
	cv::Mat dst1;		//gaussian 용
	cv::Mat dst2;		//bilateral 용
	cv::Mat dst3;		//bilateral 용
	cv::Mat noise(src.size(), CV_32SC1);		//-1.0 ~ 1.0
	cv::randn(noise, 0, 5);					//RAYSCALE
	cv::add(src, noise, src, cv::noArray(), CV_8UC1);
	cv::imshow("Noise_SRC", src);

	cv::GaussianBlur(src, dst1, cv::Size(), 5);
	cv::imshow("GaussianBlur", dst1);

	cv::bilateralFilter(src, dst2, -1, 10, 5);
	cv::imshow("bilateral", dst2);

	cv::bilateralFilter(src, dst3, -1, 20, 10);		//뽀샤시, 잡티 사라짐(노이즈 제거)
	cv::imshow("bilateral-1", dst3);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void filter_meian()
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	cv::imshow("SRC", src);
	int num = static_cast<int>(src.total() * 0.1);		//픽셀총 합 *0.1,src.total() => uchar
	for (int i = 0; i < num; i++)
	{
		int x = rand() % src.cols;	//cols영역안에서만(밖으로 벗어나서는 안된다) 노이즈를 만들어야 하기 때문에
		int y = rand() % src.rows;	//좌표값
		src.at<uchar>(y, x) = (i % 2) * 255;		//홀짝 (흰,검)
	}
	cv::imshow("Salt_and_Papper", src);

	cv::Mat dst1;		//gaussian용
	cv::Mat dst2;		//Median용
	cv::GaussianBlur(src, dst1, cv::Size(), 1);
	cv::imshow("Gaussian", dst1);

	cv::medianBlur(src, dst2, 3);	//3=> 3*3
	cv::imshow("Median", dst2);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
