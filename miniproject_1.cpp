#pragma once
#include"opencv2/opencv.hpp" // iostream포함

void detect_edge_by_mouse()
{
	static cv::Rect box;
	cv::Mat img{cv::imread("Lenna1.png")};

	if (img.empty())
	{
		std::cout<<"파일 없음"<<std::endl;
		return;
	}
	cv::namedWindow("Edge",cv::WINDOW_NORMAL);		//WINDOW_NORMAL => 창크기가 변경 가능
	static bool isFinish{ false };
	static bool makeDraw{ false };

	auto click_mouse{ [](int event, int x, int y, int , void*) 
	{	//람다함수
		if (event == cv::EVENT_MOUSEMOVE && !isFinish)
		{
			box.width = x - box.x;
			box.height = y - box.y;
		}
		else if (event == cv::EVENT_LBUTTONDOWN && !isFinish)
		{
			makeDraw = true;
			box = cv::Rect(x, y, 0, 0);
		}
		else if (event == cv::EVENT_LBUTTONUP && !isFinish)
		{
			makeDraw = false;
			if (!box.width < 0)
			{
				box.x = box.x + box.width;
				box.width = box.width * -1;	//음수가 되어 영역을 벗어났을때 양수로 만들어줌
			}
			if (!box.height < 0)
			{
				box.y = box.y + box.height;
				box.height = box.height * -1;	//음수가 되어 영역을 벗어났을때 양수로 만들어줌
			}
			isFinish = true;		//한번만 하고 끝냄
		}
	} };
	cv::setMouseCallback("Edge", click_mouse, (void*)&img);
	while (!isFinish)
	{
		cv::Mat temporary {img.clone()};		//원본을 복사한 이미지
		if (makeDraw)		//makeDraw == true
		{
			cv::rectangle(temporary, box, cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("Edge", temporary);
		if (cv::waitKey(20)	==	27)	//20 = 0.02sec
		{
			break;
		}
	}
	//화면에 사각형(박스) 그려준 후 => R.O.I => 그 영역만큰 edge 그려주기(키포인트)
	cv::Mat gray_scale;
	cv::cvtColor(img, gray_scale, cv::COLOR_BGR2GRAY);
	//에지로 바꿔
	cv::Mat patch_area{gray_scale(box)};	//박스 사이즈만큼만 영역을 만들어준다 (박스 사이즈 Matrix)
	cv::Canny(patch_area, gray_scale, 30, 30 * 2.5, 3);	//threshold
	cv::imshow("Edge Detection", ~gray_scale);	//컬러영상이 까맣게 나옴
	//볼때만 반전을 시켜서 흰색으로 보기위해서는 ~gray_scale
	cv::waitKey();
	cv::destroyAllWindows();
}