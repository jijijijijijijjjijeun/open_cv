#pragma once
#include"opencv2/opencv.hpp" // iostream포함
using namespace std;
using namespace cv;
void ex()
{
	cv::Mat src = cv::imread("Lenna1.png");
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
void eex()
{

	cv::Mat src = cv::imread("Lenna1.png");	//png 4 채널 / bmp 3채널

	if (src.empty())
	{
		return;
	}

	cv::Mat dst = src + cv::Scalar(120, 120, 120);

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();

}
void eex_1()
{

	cv::Mat src = cv::imread("Lenna1.png");	//png 4 채널 / bmp 3채널

	if (src.empty())
	{
		return;
	}

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			cv::Vec3b& p = src.at<cv::Vec3b>(i, j);
			p[0] = cv::saturate_cast<uchar>(120 + p[0]);
			p[1] = cv::saturate_cast<uchar>(120 + p[1]);
			p[2] = cv::saturate_cast<uchar>(120 + p[2]);
		}
	}

	cv::imshow("SRC", src);

	cv::waitKey(0);
	cv::destroyAllWindows();

}
void eeex()
{
	cv::Mat src = cv::imread("Lenna1.png");
	if (src.empty())
	{
		std::cout << "file not found" << std::endl;
		return;
	}
	std::cout << src.size() << std::endl;
	cv::Mat dst = src(cv::Rect(56, 56, 400, 400));

	cv::imshow("IMAGE1", src);
	cv::imshow("IMAGE2", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}
void eeex_1()
{
	const cv::Mat src = cv::imread("Lenna1.png");
	if (src.empty())
	{
		return;
	}
	cv::Mat background(src.size(), src.type());
	const int32_t WIDTH{ 400U };
	const int32_t HEIGHT{ 400U };
	const int32_t START_X{ cvRound((src.rows - WIDTH) / 2) };
	const int32_t START_Y{ cvRound((src.rows - HEIGHT) / 2) };
	const int32_t END_X{ cvRound(START_X + WIDTH) };
	const int32_t END_Y{ cvRound(START_Y + HEIGHT) };

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if ((START_X <= j and j < END_X) and (START_Y <= i and i < END_Y))
			{
				background.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);

			}
			else
			{
				background.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 0);
			}
		}
	}

	cv::imshow("SRC", src);
	cv::imshow("BG", background);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
void eeeex()
{
	cv::Mat src = cv::imread("Lenna1.png");
	if (src.empty())
	{
		return;
	}

	cv::Mat dst;

	cv::TickMeter tick;
	tick.start();
	//cv::Mat dst(src);
	src.copyTo(dst);
	tick.stop();

	std::cout << "---------------------------------------" << std::endl;
	std::cout << "연산에 걸린 시간 : " << tick.getTimeMilli() << " ms" << std::endl;
	std::cout << "원본 사이즈 : " << src.size() << std::endl;
	std::cout << "원본 타입 : " << src.type() << std::endl;
	std::cout << "결과 사이즈 : " << dst.size() << std::endl;
	std::cout << "결과 타입 : " << dst.type() << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	cv::imshow("LENNA", src);
	cv::imshow("DST", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
void eeeex_1()
{
	const cv::Mat src = cv::imread("Lenna1.png",cv::IMREAD_COLOR);
	if (src.empty())
	{
		return;
	}

	cv::Mat dst(src.size(), src.type());

	cv::TickMeter timer;
	timer.start();
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			cv::Vec3b& p_src = const_cast<cv::Vec3b&>(src.at<cv::Vec3b>(i, j));	//잠시 원본을 접근할 권한이 생김
			cv::Vec3b& p_dst = dst.at<cv::Vec3b>(i, j);							//src는 지금 const상태
			p_dst = p_src;														//주소를 복사, 빠름
		}
	}
	timer.stop();

	std::cout << "---------------------------------------" << std::endl;
	std::cout << "연산에 걸린 시간 : " << timer.getTimeSec() << "s" << std::endl;
	std::cout << "원본 사이즈 : " << src.size() << std::endl;
	std::cout << "원본 타입 : " << src.type() << std::endl;
	std::cout << "결과 사이즈 : " << dst.size() << std::endl;
	std::cout << "결과 타입 : " << dst.type() << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	cv::imshow("LENNA", src);
	cv::imshow("DST", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void a()
{
	const cv::Mat src = cv::imread("side.png");
	if (src.empty())
	{
		return;
	}
	cv::CascadeClassifier classifier("haarcascade_profileface.xml");
	if (classifier.empty())
	{
		std::cerr << "haarcascade_profileface open fail" << std::endl;
	}

	std::vector<cv::Rect> faces;
	classifier.detectMultiScale(src, faces);

	for (cv::Rect rc : faces)
	{
		cv::rectangle(src, rc, cv::Scalar(255, 0, 255), 2);
	}

	cv::imshow("SRC", src);

	cv::waitKey();
	cv::destroyAllWindows();
}

