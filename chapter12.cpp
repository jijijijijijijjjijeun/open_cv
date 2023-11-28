#pragma once
#include"opencv2/opencv.hpp" // iostream포함

void labeling_basic()
{
	uchar data[] = {
		0,0,1,1,0,0,0,0,
		1,1,1,1,0,0,1,0,
		1,1,1,1,0,0,0,0,
		0,0,0,0,0,1,1,0,
		0,0,0,1,1,1,1,0,
		0,0,0,1,0,0,1,0,
		0,0,1,1,1,1,1,0,
		0,0,0,0,0,0,0,0,
	};

	cv::Mat src = cv::Mat(8, 8, CV_8UC1, data) * 255;

	cv::Mat labels;

	int cnt = cv::connectedComponents(src, labels);

	std::cout << "src : " << std::endl << src << std::endl;
	std::cout << "labels : " << std::endl << labels << std::endl;
	std::cout << "number of labels : " << cnt << std::endl;		//4 : 배경까지 포함

}

void labeling_stats()
{
	cv::Mat src = cv::imread("keyboard.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat binary_image;
	cv::threshold(src, binary_image, 0.0, 255.0, cv::THRESH_BINARY | cv::THRESH_OTSU);

	cv::Mat labels, stats, centroids;
	int numberoflables = cv::connectedComponentsWithStats(binary_image, labels, stats, centroids);
	std::cout << "-----------------------------------------" << std::endl << numberoflables << std::endl;
	//numberoflables = 38
	cv::Mat dst;
	cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);

	for (int i = 1; i < numberoflables; i++)
	{
		int* p = stats.ptr<int>(i);

		if (p[4] < 20)
		{
			continue;
		}
		cv::rectangle(dst, cv::Rect(p[0], p[1], p[2], p[3]), cv::Scalar(0, 255, 255), 2);
	}

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();

}

void contours_hier()	//p.395
{
	cv::Mat src = cv::imread("contours.bmp", cv::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		return;
	}
	std::vector<std::vector<cv::Point>>contours;
	//std::vector<cv::Vec4i>hierarchy;

	cv::findContours(src, contours, cv::RETR_LIST,cv::CHAIN_APPROX_NONE);
	cv::Mat dst;
	cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);
	int index{ 0 };
	for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); it != contours.end(); ++it)
	{
		cv::Scalar c(rand() % 255, rand() & 255, rand() & 255);
		cv::drawContours(dst, contours, index++, c, 2);
	}
	cv::imshow("SRC", src);
	cv::imshow("DSt", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void setLabel(cv::Mat& src, const std::vector<cv::Point>& pts, const cv::String& label)
{
	cv::Rect rc = boundingRect(pts);
	cv::rectangle(src, rc, cv::Scalar(0, 0, 255), 1);
	cv::putText(src, label, rc.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
}

void find_polygon()
{
	cv::Mat src1 = cv::imread("ppt4.png", cv::IMREAD_COLOR);
	cv::Mat src;
	cv::resize(src1, src, cv::Size(512, 512));

	if (src.empty())
	{
		return;
	}
	cv::Mat gray_scale;
	cv::cvtColor(src, gray_scale, cv::COLOR_BGR2GRAY);

	cv::Mat binary_image;
	cv::threshold(gray_scale, binary_image, 0.0, 255.0, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	cv::imshow("SRC", src);
	cv::imshow("GRAY", gray_scale);
	cv::imshow("BINARY", binary_image);

	//외곽선들의 포인터를 저장하는 벡터자료형이 필요함

	std::vector<std::vector<cv::Point>>contours;
	cv::findContours(binary_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	for (std::vector<cv::Point>& pts : contours)
	{
		if (contourArea(pts) < 50)
		{
			continue;
		}
		std::vector<cv::Point> approx;
		cv::approxPolyDP(pts, approx, cv::arcLength(pts, true) * 0.02, true);
		int vtc = (int)approx.size();
		if (vtc == 1)
		{
			setLabel(src, pts, "dot");
		}
		else if (vtc == 2)
		{
			setLabel(src, pts, "line");
		}
		else if (vtc == 3)
		{
			setLabel(src, pts, "triangle");
		}
		else if (vtc == 4)
		{
			setLabel(src, pts, "rectangle");
		}
		else if (vtc == 5)
		{
			setLabel(src, pts, "pentagon");
		}
		
		else if (vtc > 5)		//원 	(vtc는 꼭짓점 개수)
		{
			 if (vtc == 10)
			{
				setLabel(src, pts, "star");
			}
			double len = cv::arcLength(pts, true);
			double area = contourArea(pts);
			double ratio = 4.0 * CV_PI * area / (len * len);
			if (ratio > 0.8)
			{
				setLabel(src, pts, "circle");
			}
		}
	}

	cv::imshow("DST", src);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void detect_face()
{

	cv::Mat src = cv::imread("bts.png");
	
	if (src.empty())
	{
		std::cout << "파일 없음" << std::endl;
		return;
	}

	cv::CascadeClassifier classifier("haarcascade_frontalface_default.xml");

	if (classifier.empty())
	{
		std::cout << "Haar 필터 계수들이 없음" << std::endl;
		return;
	}
	std::vector<cv::Rect>faces;
	classifier.detectMultiScale(src, faces);

	for (cv::Rect rc: faces)		//auto&& face : faces
	{
		cv::rectangle(src, rc, cv::Scalar(255, 0, 255), 2);
	}
	cv::imshow("SRC", src);

	cv::waitKey(0);
	cv::destroyAllWindows();
}