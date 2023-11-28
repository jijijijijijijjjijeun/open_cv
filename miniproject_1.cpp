#pragma once
#include"opencv2/opencv.hpp" // iostream����

void detect_edge_by_mouse()
{
	static cv::Rect box;
	cv::Mat img{cv::imread("Lenna1.png")};

	if (img.empty())
	{
		std::cout<<"���� ����"<<std::endl;
		return;
	}
	cv::namedWindow("Edge",cv::WINDOW_NORMAL);		//WINDOW_NORMAL => âũ�Ⱑ ���� ����
	static bool isFinish{ false };
	static bool makeDraw{ false };

	auto click_mouse{ [](int event, int x, int y, int , void*) 
	{	//�����Լ�
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
				box.width = box.width * -1;	//������ �Ǿ� ������ ������� ����� �������
			}
			if (!box.height < 0)
			{
				box.y = box.y + box.height;
				box.height = box.height * -1;	//������ �Ǿ� ������ ������� ����� �������
			}
			isFinish = true;		//�ѹ��� �ϰ� ����
		}
	} };
	cv::setMouseCallback("Edge", click_mouse, (void*)&img);
	while (!isFinish)
	{
		cv::Mat temporary {img.clone()};		//������ ������ �̹���
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
	//ȭ�鿡 �簢��(�ڽ�) �׷��� �� => R.O.I => �� ������ū edge �׷��ֱ�(Ű����Ʈ)
	cv::Mat gray_scale;
	cv::cvtColor(img, gray_scale, cv::COLOR_BGR2GRAY);
	//������ �ٲ�
	cv::Mat patch_area{gray_scale(box)};	//�ڽ� �����ŭ�� ������ ������ش� (�ڽ� ������ Matrix)
	cv::Canny(patch_area, gray_scale, 30, 30 * 2.5, 3);	//threshold
	cv::imshow("Edge Detection", ~gray_scale);	//�÷������� ��İ� ����
	//������ ������ ���Ѽ� ������� �������ؼ��� ~gray_scale
	cv::waitKey();
	cv::destroyAllWindows();
}