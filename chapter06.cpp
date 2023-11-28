#pragma once
#include"opencv2/opencv.hpp" // iostream����

using namespace std;
using namespace cv;

void operating()
{
	cv::Mat img1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img1.size << std::endl;		//400*600
	cv::Mat img2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	std::cout << img2.size << std::endl;
}

void arithmetic1()  //������ ���� ����			//p.230
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
	cv::Mat dst1 = img1 + img2;							//���ϱ�
	cv::Mat dst2 = img1 - img2;							//����
	cv::Mat dst3(256, 256, CV_8UC1);					//���ϱ�
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

void arithmetic2()	//�����ϴ� �Լ��� ����		p.234
{
	cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
	cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
	if (src1.empty() or src2.empty())
	{
		return;
	}
	cv::Mat dst;
	cv::add(src1, src2, dst);			//�����÷ο� �ذ�,���ϱ�
	cv::Mat dst2;
	cv::subtract(src1, src2, dst2);		//����
	cv::Mat dst3;						//������ ���� �����θ�ŭ�� ���̸� �˼� ���� ex)�ڵ��� p.235
	cv::absdiff(src1, src2, dst3);		//������ ���̰�(���밪 ���� => ����� ����)
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
	//dst1 = src1 & src2;					//������ ǥ��
	//cv::bitwise_and(src1, src2, dst);		//�Լ��� ǥ��
	//dst = src1 | src2;					//������ ǥ��
	//cv::bitwise_or(src1, src2, dst);		//�Լ��� ǥ��
	//dst = src1 ^ src2;					//������ ǥ��
	//cv::bitwise_xor(src1, src2, dst);		//�Լ��� ǥ��
	//dst = ~src1;							//������ ǥ��
	cv::bitwise_not(src1, src2, dst);		//�Լ��� ǥ��
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}