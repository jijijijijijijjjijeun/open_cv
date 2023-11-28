#pragma once
#include"opencv2/opencv.hpp" // iostream����

static void on_hue_changed(int, void*);

void color_inverse()		//p.337
{
	cv::Mat src = cv::imread("butterfly.jpg", cv::IMREAD_COLOR);

	if (src.empty())
	{
		return;
	}

	//cv::Mat dst = cv::Scalar(255, 255, 255) - src;	//14 ~ 28 ��ü
	cv::Mat dst(src.rows, src.cols, src.type());	//src.type()�� src �̹����� ������ Ÿ�԰� ä�� ���� �����ϵ��� �մϴ�.
	std::cout << "------------------------------" << src.type() << std::endl;		//16

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255) - src.at<cv::Vec3b>(i, j);	//21 ~ 26 ��ü
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
	cv::imshow("B_plane", bgr_planes[0]);	//blue���� ���� ũ�� ������  ���� ������� ǥ��
	cv::imshow("G_plane", bgr_planes[1]);	//green���� ���� ũ�� ������  ���� ������� ǥ��
	cv::imshow("R_plane", bgr_planes[2]);	//red���� ���� ũ�� ������  ���� ������� ǥ��
	//������� ��� ��,����� ������ ũ�⿡ b������ ��Ӱ� g,r������ ��� ǥ��

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void color_equalizerHist()		//p.348
{	//������ ������ä ��Ϻ� ���̴� ������׷� ��Ȱȭ(��Ϻ� ����)
	cv::Mat src = cv::imread("pepper.bmp");

	if (src.empty())
	{
		return;
	}

	cv::Mat src_ycrcb;
	cv::cvtColor(src, src_ycrcb, cv::COLOR_BGR2YCrCb);	//src�� YCrCb ���������� �����Ͽ� src_ycrcb�� ����

	std::vector<cv::Mat> ycrcb_planes;		//split : 1ä�� ����� �������� ��ȯ �� ��
	cv::split(src_ycrcb, ycrcb_planes);		//src_ycrcb������ ä���� �и��Ͽ� ycrcb_planes�� ����

	cv::equalizeHist(ycrcb_planes[0], ycrcb_planes[0]);		//0 => Yä��(���, �ֵ�)
	 														//������׷� ��Ȱȭ(��Ϻ� ����)
	cv::Mat dst_ycrcb;									
	cv::merge(ycrcb_planes, dst_ycrcb);		//ycrcb_planes���Ϳ� ����ִ� 3������ ���ļ� dst_ycrcb������ ������ 

	cv::Mat dst;			//dst_ycrcb������ �������� BGR�� ��ȯ�Ͽ� dst�� ����
	cv::cvtColor(dst_ycrcb, dst, cv::COLOR_YCrCb2BGR);	

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

static int lower_bound = 40;	//Ʈ���� ��ġ ���庯��(H)
static int upper_bound = 80;	//Ʈ���� ��ġ ���庯��(H)
static cv::Mat src;
static cv::Mat src_hsv;
static cv::Mat mask;

void on_hue_changed(int, void*)		//Ʈ���� �ݹ� �Լ�
{
	cv::Scalar lowerb(lower_bound, 100, 0);		//ä��(S) 100 ~ 255�� ����
	cv::Scalar upperb(upper_bound, 255, 255);	//��(V) 0 ~ 255�� ����		,H�� Ʈ���ٷ� ���� ����
	cv::inRange(src_hsv, lowerb, upperb, mask);	//HSV������ ���� : lowerb ~ upperb �� �ȼ��� ������� ������ ����ũ ����

	cv::imshow("mask", mask);	//ex) H(����)�� 40~80(���)�� �κи� ������� ��µǴ� ����ũ ����
}								//ex) H(����)�� 100~140(�Ķ���)�� �κи� ������� ��µǴ� ����ũ ����

void using_inRange()
{
	src = cv::imread("candies.png");

	if (src.empty())
	{
		return;
	}
	cv::cvtColor(src, src_hsv, cv::COLOR_BGR2HSV);	//HSV�� �������� ��ȯ�Ͽ� src_hsv�� ����

	cv::imshow("SRC", src);

	cv::namedWindow("mask");
	cv::createTrackbar("Lower Hue", "mask", &lower_bound, 179, on_hue_changed);	//Ʈ���� ����
	cv::createTrackbar("Uper Hue", "mask", &upper_bound, 179, on_hue_changed);	//Ʈ���� ����
	on_hue_changed(0, 0);		//���α׷� ���۽� ������µǵ��� Ʈ���� �ݹ��Լ� ���� ȣ��

	cv::waitKey(0);
	cv::destroyAllWindows();
}