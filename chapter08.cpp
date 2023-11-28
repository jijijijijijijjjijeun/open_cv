#pragma once
#include"opencv2/opencv.hpp" // iostream포함

static void on_mouse(int event, int x, int y, int flags, void* userdata);

void affine_transform()		//p.280
{
	cv::Mat src = cv::imread("tekapo.bmp");
	if (src.empty())
	{
		return;
	}
	std::cout << src.size() << std::endl;		//640 * 480

	cv::Point2f srcPts[3];
	cv::Point2f dstPts[3];

	srcPts[0] = cv::Point2f(0, 0);
	srcPts[1] = cv::Point2f(src.cols - 1.0f, 0.0f);
	srcPts[2] = cv::Point2f(src.cols - 1.0f, src.rows - 1.0f);

	dstPts[0] = cv::Point2f(50.0f, 50.0f);
	dstPts[1] = cv::Point2f(src.cols - 100.0f, 100.0f);
	dstPts[2] = cv::Point2f(src.cols - 50.0f, src.rows - 50.0f);

	cv::Mat M = getAffineTransform(srcPts, dstPts);

	cv::Mat dst;
	cv::warpAffine(src, dst, M, cv::Size());

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);
	std::cout << M << std::endl;

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void affine_translation()
{
	cv::Mat src = cv::imread("tekapo.bmp");
	cv::Mat M = cv::Mat_<float>({ 2,3 }, { 2.0f,0.0f,60.0f,0.0f,2.0f,60.0f });	//2  0  60
	cv::Mat dst;																//0  2  60
	cv::warpAffine(src, dst, M, cv::Size());
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void affine_shear()		//p.286
{
	cv::Mat src = cv::imread("tekapo.bmp");
	float m = 0.0f;
	std::cout << "기울기를 입력하세요 : ";
	std::cin >> m;
	cv::Mat M = cv::Mat_<float>({ 2,3 }, { 1.0f,m,0.0f,0.0f,1.0f,0.0f });	//1  m  0
	cv::Mat dst;															//0  1  0
	cv::warpAffine(src, dst, M, cv::Size(cvRound(src.cols + src.rows * m),src.rows));
	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void affine_resize()		//p.289
{
	cv::Mat src = cv::imread("rose.bmp");
	if (src.empty())
	{
		std::cerr << "파일이 없습니다." << std::endl;
		return;
	}

	cv::imshow("SRC", src);
	cv::Mat dst1, dst2, dst3, dst4;

	cv::resize(src, dst1, cv::Size(), 4.0, 4.0, cv::INTER_NEAREST);
	cv::imshow("DST1", dst1(cv::Rect(400,500,400,400)));

	cv::resize(src, dst2, cv::Size(), 4.0, 4.0);
	cv::imshow("DST2", dst2(cv::Rect(400, 500, 400, 400)));

	cv::resize(src, dst3, cv::Size(), 4.0, 4.0, cv::INTER_CUBIC);		//축소할때 좋다
	cv::imshow("DST3", dst3(cv::Rect(400, 500, 400, 400)));

	cv::resize(src, dst4, cv::Size(), 4.0, 4.0, cv::INTER_LANCZOS4);	//확대 할 때 가장 좋음
	cv::imshow("DST4", dst4(cv::Rect(400, 500, 400, 400)));

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void affine_roation()		//p.293
{
	cv::Mat src = cv::imread("tekapo.bmp");
	if (src.empty())
	{
		return;
	}
	cv::Point2f center(src.cols / 2, src.rows / 2);
	int angle;
	std::cout << "angle : ";
	std::cin >> angle;
	cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);

	cv::Mat dst;
	cv::warpAffine(src, dst, M, cv::Size());

	cv::imshow("SRC", src);
	cv::imshow("DST", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void affine_flip()
{
	cv::Mat src{cv::imread("eastsea.bmp")};
	if (src.empty())
	{
		return;
	}
	cv::imshow("SRC", src);
	cv::Mat dst1;
	cv::Mat dst2;
	cv::Mat dst3;
	cv::flip(src, dst1, 1);			//	1:좌우대칭
	cv::imshow("DST_1", dst1);
	cv::flip(src, dst2, 0);			//	0:상하대칭
	cv::imshow("DST_2", dst2);
	cv::flip(src, dst3, -1);		//	-1:좌우, 상하 대칭
	cv::imshow("DST_3", dst3);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

static int count = 0;
static cv::Point2f dstQuad[4];
static cv::Point2f srcQuad[4];
static cv::Mat src;

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
   if (event == cv::EVENT_LBUTTONDOWN)
   {
      if (count < 4)
      {
         srcQuad[count++] = cv::Point2f(x, y);
         cv::circle(src, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);
         cv::imshow("SRC", src);
      }
      if (count == 4)
      {
         int width = 200;
         int height = 300;
         dstQuad[0] = cv::Point2f(0, 0);
         dstQuad[1] = cv::Point2f(width - 1, 0);
         dstQuad[2] = cv::Point2f(width - 1, height - 1);
         dstQuad[3] = cv::Point2f(0, height - 1);
         cv::Mat M = cv::getPerspectiveTransform(srcQuad, dstQuad);
         cv::Mat dst;
         cv::warpPerspective(src, dst, M, cv::Size(width, height));
         cv::imshow("DST", dst);
      }
   }
}

void perspective()
{
	src = cv::imread("card.bmp");
	if (src.empty())
	{
		return;
	}
	cv::namedWindow("SRC");
	cv::imshow("SRC", src);
	cv::setMouseCallback("SRC", on_mouse);
	
	cv::waitKey(0);
	cv::destroyAllWindows();
}