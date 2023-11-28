#pragma once
#include"opencv2/opencv.hpp" // iostream포함

static void on_throld(int position, void* user_data)
{
	cv::Mat src = *(static_cast<cv::Mat*>(user_data));
	cv::Mat dst;
	cv::threshold(src, dst, position, 255.0, cv::THRESH_BINARY);
	cv::imshow("DST", dst);
}

void using_threshold()
{
	cv::Mat src = cv::imread("neutrophils.png", cv::IMREAD_GRAYSCALE);
	if (src.empty()) return;

	cv::namedWindow("SRC");
	cv::namedWindow("DST");
	cv::imshow("SRC", src);
	cv::createTrackbar("Threshold", "DST", 0, 255, on_throld, (void*)&src);
	cv::setTrackbarPos("Threshold", "DST", 128);
	cv::waitKey();
	cv::destroyAllWindows();
}

static void on_trackbar(int position, void* user_data)
{
	cv::Mat src = *(static_cast<cv::Mat*>(user_data));
	int block_size = position;
	if (position % 2  == 0 )
	{
		--block_size;
	}
	if (block_size < 3)
	{
		block_size = 3;
	}
	cv::Mat dst;
	cv::adaptiveThreshold(src, dst, 255.0, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, block_size, 5.0);
	cv::imshow("DST", dst);
}

void adaptie_threshold()
{
	cv::Mat src = cv::imread("sudoku.jpg", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::namedWindow("DST");
	cv::createTrackbar("Blocksize", "DST", nullptr, 200, on_trackbar, (void*)&src);
	cv::setTrackbarPos("Blocksize", "DST", 11);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void erode_dilate()		//p.373
{
	cv::Mat src{cv::imread("milkdrop.bmp", cv::IMREAD_GRAYSCALE)};
	if (src.empty())
	{
		return;
	}
	cv::Mat binary_image;
	cv::threshold(src, binary_image, 0.0, 255.0, cv::THRESH_BINARY | cv::THRESH_OTSU);

	cv::Mat dst_erode;
	cv::Mat dst_dilate;
	cv::erode(binary_image, dst_erode, cv::Mat());		//3*3 - 침식
	for (int i = 0; i < 5; i++)
	{
		cv::erode(dst_erode, dst_erode, cv::Mat());		//3*3 - 침식
		cv::imshow("ERODE", dst_erode);
		cv::waitKey(0);
	}
	cv::dilate(binary_image, dst_dilate, cv::Mat());		//3*3 - 팽창
	for (int i = 0; i < 5; i++)
	{
		cv::dilate(dst_dilate, dst_dilate, cv::Mat());		//3*3 - 팽창
		cv::imshow("DILATE", dst_dilate);
		cv::waitKey(0);
	}
	
	cv::imshow("SRC", src);
	cv::imshow("BINARY", binary_image);
	
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void open_close()
{
	cv::Mat src{cv::imread("milkdrop.bmp", cv::IMREAD_GRAYSCALE)};
	if (src.empty())
	{
		return;
	}

	cv::Mat binary;
	cv::threshold(src, binary, 0.0, 255.0, cv::THRESH_BINARY | cv::THRESH_OTSU);	//이진화

	cv::Mat dst_open, dst_close;
	cv::morphologyEx(binary, dst_open, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 2);
	cv::morphologyEx(binary, dst_close, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 2);

	cv::imshow("SRC", src);
	cv::imshow("BIN", binary);
	cv::imshow("Opening", dst_open);
	cv::imshow("Closing", dst_close);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

