#pragma once
#include"opencv2/opencv.hpp" // iostream포함

static void on_hue_changed(int, void*);

void color_inverse()		//p.337
{
	cv::Mat src = cv::imread("butterfly.jpg", cv::IMREAD_COLOR);

	if (src.empty())
	{
		return;
	}

	//cv::Mat dst = cv::Scalar(255, 255, 255) - src;	//14 ~ 28 대체
	cv::Mat dst(src.rows, src.cols, src.type());	//src.type()은 src 이미지의 데이터 타입과 채널 수를 유지하도록 합니다.
	std::cout << "------------------------------" << src.type() << std::endl;		//16

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255) - src.at<cv::Vec3b>(i, j);	//21 ~ 26 대체
			//cv::Vec3b& p1 = src.at<cv::Vec3b>(i, j);
			//cv::Vec3b& p2 = dst.at<cv::Vec3b>(i, j);

			//p2[0] = 255 - p1[0];	//Bule
			//p2[1] = 255 - p1[1];	//Green
			//p2[2] = 255 - p1[2];	//Red
		}
	}

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void color_grayscale()
{
	cv::Mat src = cv::imread("butterfly.jpg");

	if (src.empty())
	{
		return;
	}

	cv::Mat dst;
	cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();

}

void color_split()	// p.344
{
	cv::Mat src = cv::imread("candies.png");

	if (src.empty())
	{
		return;
	}
	std::vector<cv::Mat> bgr_planes;
	cv::split(src, bgr_planes);

	cv::imshow("SRC", src);
	cv::imshow("B_plane", bgr_planes[0]);	//blue성분 값이 크기 때문에  밝은 흰색으로 표시
	cv::imshow("G_plane", bgr_planes[1]);	//green성분 값이 크기 때문에  밝은 흰색으로 표시
	cv::imshow("R_plane", bgr_planes[2]);	//red성분 값이 크기 때문에  밝은 흰색으로 표시
	//노란색의 경우 빨,녹색의 성분이 크기에 b에서는 어둡게 g,r에서는 밝게 표시

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void color_equalizerHist()		//p.348
{	//색감은 유지한채 명암비를 높이는 히스토그램 평활화(명암비 증대)
	cv::Mat src = cv::imread("pepper.bmp");

	if (src.empty())
	{
		return;
	}

	cv::Mat src_ycrcb;
	cv::cvtColor(src, src_ycrcb, cv::COLOR_BGR2YCrCb);	//src를 YCrCb 색공간으로 변경하여 src_ycrcb에 저장

	std::vector<cv::Mat> ycrcb_planes;		//split : 1채널 행렬을 여러개로 변환 할 때
	cv::split(src_ycrcb, ycrcb_planes);		//src_ycrcb영상의 채널을 분리하여 ycrcb_planes에 저장

	cv::equalizeHist(ycrcb_planes[0], ycrcb_planes[0]);		//0 => Y채널(밝기, 휘도)
	 														//히스토그램 평활화(명암비 증대)
	cv::Mat dst_ycrcb;									
	cv::merge(ycrcb_planes, dst_ycrcb);		//ycrcb_planes벡터에 들어있는 3영상을 합쳐서 dst_ycrcb영상을 생성함 

	cv::Mat dst;			//dst_ycrcb영상의 색공산을 BGR로 변환하여 dst에 저장
	cv::cvtColor(dst_ycrcb, dst, cv::COLOR_YCrCb2BGR);	

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

static int lower_bound = 40;	//트랙바 위치 저장변수(H)
static int upper_bound = 80;	//트랙바 위치 저장변수(H)
static cv::Mat src;
static cv::Mat src_hsv;
static cv::Mat mask;

void on_hue_changed(int, void*)		//트랙바 콜백 함수
{
	cv::Scalar lowerb(lower_bound, 100, 0);		//채도(S) 100 ~ 255는 고정
	cv::Scalar upperb(upper_bound, 255, 255);	//명도(V) 0 ~ 255는 고정		,H는 트랙바로 범위 변경
	cv::inRange(src_hsv, lowerb, upperb, mask);	//HSV색성분 범위 : lowerb ~ upperb 인 픽셀만 흰색으러 설정한 마스크 생성

	cv::imshow("mask", mask);	//ex) H(색상)가 40~80(녹색)인 부분만 흰색으로 출력되는 마스크 생성
}								//ex) H(색상)가 100~140(파란색)인 부분만 흰색으로 출력되는 마스크 생성

void using_inRange()
{
	src = cv::imread("candies.png");

	if (src.empty())
	{
		return;
	}
	cv::cvtColor(src, src_hsv, cv::COLOR_BGR2HSV);	//HSV색 공간으로 변환하여 src_hsv에 저장

	cv::imshow("SRC", src);

	cv::namedWindow("mask");
	cv::createTrackbar("Lower Hue", "mask", &lower_bound, 179, on_hue_changed);	//트랙바 생성
	cv::createTrackbar("Uper Hue", "mask", &upper_bound, 179, on_hue_changed);	//트랙바 생성
	on_hue_changed(0, 0);		//프로그램 시작시 정상출력되도록 트랙바 콜백함수 강제 호출

	cv::waitKey(0);
	cv::destroyAllWindows();
}