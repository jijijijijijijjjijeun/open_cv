#pragma once
#include"opencv2/opencv.hpp" // iostream포함

using namespace std;
using namespace cv;

void operating()
{
	cv::Mat img1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img1.size << std::endl;		//400*600
	cv::Mat img2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img2.size << std::endl;
}

void arithmetic1()  //식으로 직접 연산			//p.230
{
	cv::Mat img1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img1.size << std::endl;		//400*600
	cv::Mat img2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img2.size << std::endl;
	if (img1.empty() or img2.empty())
	{
		return;
	}
	/*for (int i = 0; i < img2.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			uchar pixel = img2.at<uchar>(i, j);
			std::cout << static_cast<int>(pixel) << " ";
		}
		std::cout << std::endl;
	}*/
	cv::Mat dst1 = img1 + img2;							//더하기
	cv::Mat dst2 = img1 - img2;							//뺴기
	cv::Mat dst3(256, 256, CV_8UC1);					//곱하기
	for (int i = 0; i < img2.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			dst3.at<uchar>(i, j) = cv::saturate_cast<uchar>(img1.at<uchar>(i, j) * img2.at<uchar>(i, j));
		}
	}
	cv::imshow("IMG1", img1);
	cv::imshow("IMG2", img2);
	cv::imshow("DST1", dst1);
	cv::imshow("DST2", dst2);
	cv::imshow("DST3", dst3);
	cv::waitKey(0);
	cv::destroyAllWindows();
} 

void arithmetic2()	//제공하는 함수로 연산		p.234
{
	cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	if (src1.empty() or src2.empty())
	{
		return;
	}
	cv::Mat dst;
	cv::add(src1, src2, dst);			//오버플로우 해결,더하기
	cv::Mat dst2;
	cv::subtract(src1, src2, dst2);		//뺴기
	cv::Mat dst3;						//그전과 현재 움직인만큼의 차이를 알수 있음 ex)자동차 p.235
	cv::absdiff(src1, src2, dst3);		//영상의 차이값(절대값 차이 => 양수가 나옴)
	cv::Mat dst4;
	cv::addWeighted(src1, 0.5, src2, 0.5, 0, dst4);		//ppt.52  p.237
	cv::imshow("SRC1",src1);			
	cv::imshow("SRC2", src2);
	cv::imshow("DST", dst);
	cv::imshow("DST2", dst2);
	cv::imshow("DST3", dst3);
	cv::imshow("DST4", dst4);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void logical()
{
	cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	if (src1.empty() or src2.empty())
	{
		return;
	}
	cv::Mat dst(256, 256, CV_8UC1);
	cv::imshow("SRC1", src1);
	cv::imshow("SRC2", src2);
	//dst1 = src1 & src2;					//식으로 표현
	//cv::bitwise_and(src1, src2, dst);		//함수로 표현
	//dst = src1 | src2;					//식으로 표현
	//cv::bitwise_or(src1, src2, dst);		//함수로 표현
	//dst = src1 ^ src2;					//식으로 표현
	//cv::bitwise_xor(src1, src2, dst);		//함수로 표현
	//dst = ~src1;							//식으로 표현
	cv::bitwise_not(src1, src2, dst);		//함수로 표현
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}