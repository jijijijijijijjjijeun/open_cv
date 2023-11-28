#pragma once
#include"opencv2/opencv.hpp" // iostream포함

void sobel_edge()	//p.313 : 인물의 가장자리(엣지)만 추출하기
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat dx;		//x 미분
	cv::Mat dy;		//y 미분
	cv::Sobel(src, dx, CV_32FC1, 1, 0);
	cv::Sobel(src, dy, CV_32FC1, 0, 1);
	cv::imshow("SRC", src); 
	cv::imshow("DX", dx);		//세로성분이 잘 보임(가로(x축)의 변호가 없으면 0)
	cv::imshow("DY", dy);		//가로성분이 잘 보임

	cv::Mat mag;
	cv::magnitude(dx, dy, mag);		//magnitude = 루트(dx^2 + dy^2)
	mag.convertTo(mag, CV_8UC1);
	cv::imshow("MAG", mag);			//x성분과 y성분의 합 => 잘보임

	cv::Mat real_edge;
	real_edge = mag > 80;		//임계값(Threshold) ; 150 -> 100 -> 80 값을 조절하면서  best엣지 찾기
	cv::imshow("REAL_EDGE", real_edge);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void canny_edge()
{
	cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat dst1;	//1 : 2 비율
	cv::Mat dst2;	//1 : 3 비율

	cv::Canny(src, dst1, 50, 100, 3, true); //true => L2(정확함)
	cv::Canny(src, dst2, 50, 150, 3, true);

	cv::imshow("SRC", src);
	cv::imshow("DST1", dst1);
	cv::imshow("DST2", dst2);

	cv::waitKey(0);
	cv::destroyAllWindows();

}

void hough_lines()	//p.324
{
	cv::Mat src = cv::imread("building.jpg", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat edge;
	cv::Canny(src, edge, 50, 150);

	std::vector<cv::Vec2f>lines;	//pi
	cv::HoughLines(edge, lines, 1, CV_PI / 180, 250);

	cv::Mat dst;		//화면에 찍어줄거
	cv::cvtColor(edge, dst, cv::COLOR_GRAY2BGR);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float r = lines[i][0];
		float t = lines[i][1];
		double cos_t = cos(t), sin_t = sin(t);
		double x0 = r * cos_t, y0 = r * sin_t;
		double alpha = 1000;

		cv::Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));		//암기
		cv::Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));		//암기
		cv::line(dst, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}										//red
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void hough_lines_segments()
{
	cv::Mat src = cv::imread("building.jpg", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::Mat edge;
	cv::Canny(src, edge, 50, 150);

	std::vector<cv::Vec4i>lines;	
	cv::HoughLinesP(edge, lines, 1, CV_PI / 180, 160, 50, 5);
	//5pixel이 떨어져 있어도 직선으로 인식
	cv::Mat dst;		
	cv::cvtColor(edge, dst, cv::COLOR_GRAY2BGR);

	for (cv::Vec4i i : lines)
	{
		cv::line(dst, cv::Point(i[0], i[1]), cv::Point(i[2], i[3]), cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}										
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void hough_circles()
{
	cv::Mat src = cv::imread("coins2.png", cv::IMREAD_GRAYSCALE);
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);

	std::vector<cv::Vec3f>circles;
	cv::Mat blurred;	//원본을 좀 더 잘 찾기 위해 즐러처리(잡음제거)
	cv::blur(src, blurred, cv::Size(3, 3));	//3 * 3

	cv::Mat dst;
	cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);

	cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, 38, 197, 63);

	for (auto&& circle : circles)
	{
		cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
		int radius = cvRound(circle[2]);
		cv::circle(dst, center, radius, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}