#pragma once
#include"opencv2/opencv.hpp" // iostream����
using namespace std;
using namespace cv;

void show_code03_07()
{
	Mat img1;
	Mat img2(480, 640, CV_8UC1);
	Mat img3(480, 640, CV_8UC3);
	Mat img4(Size(640, 480), CV_8UC3);

	Mat img5(480, 640, CV_8UC1, Scalar(128));
	Mat img6(480, 640, CV_8UC3, Scalar(128, 128, 128));

	Mat mat1 = Mat::zeros(480, 640, CV_32SC1);
	Mat mat2 = Mat::ones(480, 640, CV_32FC1);
	Mat mat3 = Mat::eye(640, 640, CV_32FC1);

	float data[] = { 1,2,3,4,5,6 };
	Mat mat4(2, 3, CV_32FC1, data);

	Mat mat5 = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat mat6 = Mat_<uchar>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });

	mat4.create(256, 256, CV_8UC3);
	mat5.create(4, 4, CV_32FC1);

	mat4 = Scalar(255, 0, 0);
	mat5.setTo(1.f);

	imshow("MATRIX", mat5);
	waitKey(50000);
}

void show_code03_08()
{
	cv::Mat img1 { cv::imread("Lenna1.png") };

	cv::Mat img2 = img1; //shallow copy �̹����� �ּҸ� ����  �������� �������

	cv::Mat img3 = img1.clone(); //�������� **Ŭ��** �������� ������ �����ʴ´�

	cv::namedWindow("IMAGE1");
	cv::namedWindow("IMAGE2");
	cv::namedWindow("IMAGE3");
	cv::namedWindow("IMAGE4");

	cv::imshow("IMAGE1", img1);
	cv::imshow("IMAGE2", img2);
	cv::imshow("IMAGE3", img3);
	img1 = ~img1;		//��Ʈ�� ����, ���� ���� �ϸ� ���� ������ �ȵǰ� ���� ����� ���� ������ �ȴ�.

	cv::imshow("IMAGE4", img1);
	cv::imshow("IMAGE5", img2);
	cv::imshow("IMAGE6", img3);

	cv::waitKey(0);
}

void show_code03_09()
{
	cv::Mat img1 = cv::imread("cat.bmp");
	if (img1.empty())
	{
		std::cout << "file not found" << std::endl;
		return;
	}																//	����     ����
	std::cout << img1.size() << std::endl;		//������ ����� �˰� ������ 640 * 480
	cv::Mat img2 = img1(cv::Rect(220, 120, 340, 240));
								//(220 ~ 220+120),(340 ~ 340+240)
	cv::imshow("IMAGE1", img1);
	cv::imshow("IMAGE2", img2);
	cv::imshow("IMAGE3", ~img2);	//�� ���� ������ ��ο����� ��ο���� �����
	cv::Mat img3 = img1(cv::Rect(220, 120, 340, 240)).clone();		//clone: ����
	cv::imshow("IMAGE4", img3);
	cv::waitKey(0);
	cv::destroyAllWindows();	//�����ִ� â �� �ν�
}

void show_code03_10()
{										  //�����, uchar
	cv::Mat mat1 = cv::Mat::zeros(10, 10, CV_8UC(1));
	std::cout << mat1 << std::endl;
	uchar value = 0;
	for (int i = 0; i < mat1.rows; i++)
	{
		for (int j = 0; j < mat1.cols; j++)
		{
			++value;
			mat1.at<uchar>(i, j) = value;
		}
	}	
	std::cout << "*=================================================================" << std::endl;
	for (int i = 0; i < mat1.rows; i++)
	{
		uchar* p = mat1.ptr(i);		//���� �����Ϸ��� , ����� ��ǥ�ּҸ� �޾ƿ�
		for (int j = 0; j < mat1.cols; j++)
		{
			//p[j]	=	*(p + j)		
			std::cout << static_cast<int>(p[j]) << std::endl;
		}				//p[j]�� �ƽ�Ű �ڵ尪���� �ν� �ϴ� �ڷ����� ĳ��Ʈ �������
	}
	std::cout << mat1 << std::endl;
	cv::imshow("MAT1", mat1);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void show_code03_11()
{
	cv::Mat img1 = cv::imread("coins.png", cv::IMREAD_UNCHANGED);
	if (img1.empty())
	{
		return;
	}
	std::cout << "�̹��� �� : " << img1.cols << std::endl;				//512
	std::cout << "�̹��� ���� : " << img1.rows << std::endl;				//512
	std::cout << "�̹��� ������ : " << img1.size << std::endl;			//512*512
	std::cout << "�ȼ� �Ѱ� ������ : " << img1.elemSize1() << std::endl;	//1
	std::cout << "ä�� : " << img1.channels() << std::endl;				//3 , 1
	if (img1.type() == CV_8UC1)
	{
		std::cout << "GrayScale" << std::endl;
	}
	else if (img1.type() == CV_8UC3)
	{
		std::cout << "Color" << std::endl;
	}
	else
	{
		std::cout << "PNG 4 ä��" << std::endl;
	}
}

void show_code03_12()
{
	float data[] = { 1.0f,2.0f,3.0f,4.0f/*,5.0f,6.0f,7.0f,8.0f,9.0f*/};
	cv::Mat mat1(2, 2, CV_32FC1, data);
	std::cout << mat1 << std::endl;
	std::cout << "===========================" << std::endl;
	cv::Mat mat2 = mat1.inv();	//���Լ�
	cv::Mat mat3 = mat1 * mat2;
	std::cout << mat2 << std::endl;
	std::cout << "===========================" << std::endl;
	std::cout << mat3 << std::endl;
	std::cout << "===========================" << std::endl;

	cv::Mat mat4 = cv::Mat::eye(512, 512, CV_8UC1) * 255;
	cv::Mat mat5 = mat1.t();
	std::cout << mat5 << std::endl;

	cv::Mat img = cv::imread("Lenna1.png");
	cv::Mat img2 = img.t();		//�̹��� ����
	cv::imshow("Lenna1.png", img2);

	cv::imshow("EYE", mat4);
	cv::waitKey(0);
	cv::destroyAllWindows();
}