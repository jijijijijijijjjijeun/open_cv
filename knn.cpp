#pragma once
#include"opencv2/opencv.hpp" // iostream포함
static cv::Ptr<cv::ml::KNearest> train_knn()//연결할수 있는 함수	//리턴타입 : cv::Ptr<cv::ml::KNearest>
{			//Ptr 클래스			//20 *20 = 400	
	cv::Mat digits = cv::imread("digits.png",cv::IMREAD_GRAYSCALE);
	//이진화 해야하는데 할필요 없음 왜? 이미 이진화 되어있음 (전처리 되었음)

	if (digits.empty())
	{
		return nullptr;		//특수한 포인터 , 스마트 포인터(c++ 11버전 부터 발표) : 자동적으로 heap에서 사라짐
	}						//개발자가 굳이 delete를 안해도 된다.

	cv::Mat train_images;	//train이미지를 담을
	cv::Mat train_labels;	//정답, target, label
	for (int i = 0; i < 50; i++)		//1000픽셀 가로
	{
		for (int j = 0; j < 100; j++)		//20픽셀 -> 100번
		{
			cv::Mat roi = digits(cv::Rect(j * 20, i * 20, 20, 20));		//넓이 20, 높이 20
			cv::Mat roi_float;	//roi을 float타입으로 바꿔줘야함
			roi.convertTo(roi_float, CV_32FC1);		///open CV 데이터 타입으로 변환
			//내적을 해야함		//20*20(2차원)을 한줄인 벡터(1차원)로 만들어야함 평탕화, flatting
			cv::Mat roi_flatten = roi_float.reshape(1, 1);	//1차원 형태로 만들어줘야함 400*1  
			//이미지를 넣어줌
			train_images.push_back(roi_flatten);	//입력데이터
			train_labels.push_back(i / 5);			//5번째 마다 이미지를 정답에다가 넣어줌(숫자마다 5줄씩 있음)
		}	
	}
	cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();		//싱귤러	
	knn->train(train_images, cv::ml::ROW_SAMPLE, train_labels);		//클래스 객체 하나 만들어, 훈련이 된거임(정답표)
	return knn;					//밑으로 내려가는 방식
}
static cv::Point point_Prev(-1, -1);		//마우스를 떼면 (-1,-1)로 초기화
static void on_mouse(int event, int x, int y, int flag, void* user_data)
{
	//받을 이미지 하나 필요
	cv::Mat image = *(static_cast<cv::Mat*>(user_data));
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		point_Prev = cv::Point(x, y);
	}
	else if (event == cv::EVENT_LBUTTONUP)
	{
		point_Prev = cv::Point(x, y);
	}
	else if (event == cv::EVENT_MOUSEMOVE && (flag & cv::EVENT_FLAG_LBUTTON))
	{
		cv::line(image, point_Prev, cv::Point(x, y), cv::Scalar::all(255), 30, cv::LINE_AA, 0);
		point_Prev = cv::Point(x, y);	//	초기화						//두께  40
		cv::imshow("DISPLAY", image);
	}
}
void do_knn_maching_learning()
{	//시작하자마자 함출 호출하면서 전처리로 훈련을 한 결과 값을 받는 거임(학습 전처리)
	//일단은 불러와야 함 ml이라는 namespace 안에 KNearest
	cv::Ptr<cv::ml::KNearest> knn(train_knn());	// 훈련해야함 ,return 타입이 Ptr, 훈련된 객체를 받는거임
	if (knn.empty())
	{
		std::cerr << "Train 실패" << std::endl;
		return;
	}
	std::cout << "학습 성공" << std::endl;
	//400*400픽셀 필요
	cv::Mat input_display(400, 400, CV_8UC1);	//grayscale
	cv::namedWindow("DISPLAY", cv::WINDOW_NORMAL);
	cv::imshow("DISPLAY", input_display);
	cv::setMouseCallback("DISPLAY", on_mouse, static_cast<void*>(&input_display));

	while (true)
	{
		int keyboard_key = cv::waitKey();
		if (keyboard_key == 27)	//esc => break
		{
			break;
		}
		else if (keyboard_key == ' ')	//space bar => 마우스 입력상태로 만들기
		{	//입력 전처리
			cv::Mat image_resize;	//사용자가 화면으로 부터 받은 입력 픽셀값을 모델에 맏기 20*20픽셀오 바꿔줘야함
			cv::Mat image_flaot;	//CV_32FC1으로 바꾸기 위함 Mat
			cv::Mat image_flatten;	//사용지로 부터 받은 이미지를 flatten해서 (학습된 모델에서 입력값으로 주기 위해 flatten)
			cv::Mat resouce;		//학습된 결과가 저장된 행렬
			cv::resize(input_display, image_resize, cv::Size(20, 20), 0, 0, cv::INTER_AREA);
			image_resize.convertTo(image_flaot, CV_32FC1);	//이미지 타입을 바꾸기 위해
			image_flatten = image_flaot.reshape(1, 1);	//1 * 400 픽셀 평탄화
			knn->findNearest(image_flatten, 5, resouce);	//	k = 3, 5, 7, 9(7이 best)
			std::cout << cvRound(resouce.at<float>(0, 0)) << std::endl;
			input_display.setTo(0);	//화면을 검게 초기화
			cv::imshow("DISPLAY", input_display);
		}
	}

	cv::destroyAllWindows();
}