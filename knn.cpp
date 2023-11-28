#pragma once
#include"opencv2/opencv.hpp" // iostream����
static cv::Ptr<cv::ml::KNearest> train_knn()//�����Ҽ� �ִ� �Լ�	//����Ÿ�� : cv::Ptr<cv::ml::KNearest>
{			//Ptr Ŭ����			//20 *20 = 400	
	cv::Mat digits = cv::imread("digits.png",cv::IMREAD_GRAYSCALE);
	//����ȭ �ؾ��ϴµ� ���ʿ� ���� ��? �̹� ����ȭ �Ǿ����� (��ó�� �Ǿ���)

	if (digits.empty())
	{
		return nullptr;		//Ư���� ������ , ����Ʈ ������(c++ 11���� ���� ��ǥ) : �ڵ������� heap���� �����
	}						//�����ڰ� ���� delete�� ���ص� �ȴ�.

	cv::Mat train_images;	//train�̹����� ����
	cv::Mat train_labels;	//����, target, label
	for (int i = 0; i < 50; i++)		//1000�ȼ� ����
	{
		for (int j = 0; j < 100; j++)		//20�ȼ� -> 100��
		{
			cv::Mat roi = digits(cv::Rect(j * 20, i * 20, 20, 20));		//���� 20, ���� 20
			cv::Mat roi_float;	//roi�� floatŸ������ �ٲ������
			roi.convertTo(roi_float, CV_32FC1);		///open CV ������ Ÿ������ ��ȯ
			//������ �ؾ���		//20*20(2����)�� ������ ����(1����)�� �������� ����ȭ, flatting
			cv::Mat roi_flatten = roi_float.reshape(1, 1);	//1���� ���·� ���������� 400*1  
			//�̹����� �־���
			train_images.push_back(roi_flatten);	//�Էµ�����
			train_labels.push_back(i / 5);			//5��° ���� �̹����� ���信�ٰ� �־���(���ڸ��� 5�پ� ����)
		}	
	}
	cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();		//�ַ̱�	
	knn->train(train_images, cv::ml::ROW_SAMPLE, train_labels);		//Ŭ���� ��ü �ϳ� �����, �Ʒ��� �Ȱ���(����ǥ)
	return knn;					//������ �������� ���
}
static cv::Point point_Prev(-1, -1);		//���콺�� ���� (-1,-1)�� �ʱ�ȭ
static void on_mouse(int event, int x, int y, int flag, void* user_data)
{
	//���� �̹��� �ϳ� �ʿ�
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
		point_Prev = cv::Point(x, y);	//	�ʱ�ȭ						//�β�  40
		cv::imshow("DISPLAY", image);
	}
}
void do_knn_maching_learning()
{	//�������ڸ��� ���� ȣ���ϸ鼭 ��ó���� �Ʒ��� �� ��� ���� �޴� ����(�н� ��ó��)
	//�ϴ��� �ҷ��;� �� ml�̶�� namespace �ȿ� KNearest
	cv::Ptr<cv::ml::KNearest> knn(train_knn());	// �Ʒ��ؾ��� ,return Ÿ���� Ptr, �Ʒõ� ��ü�� �޴°���
	if (knn.empty())
	{
		std::cerr << "Train ����" << std::endl;
		return;
	}
	std::cout << "�н� ����" << std::endl;
	//400*400�ȼ� �ʿ�
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
		else if (keyboard_key == ' ')	//space bar => ���콺 �Է»��·� �����
		{	//�Է� ��ó��
			cv::Mat image_resize;	//����ڰ� ȭ������ ���� ���� �Է� �ȼ����� �𵨿� ���� 20*20�ȼ��� �ٲ������
			cv::Mat image_flaot;	//CV_32FC1���� �ٲٱ� ���� Mat
			cv::Mat image_flatten;	//������� ���� ���� �̹����� flatten�ؼ� (�н��� �𵨿��� �Է°����� �ֱ� ���� flatten)
			cv::Mat resouce;		//�н��� ����� ����� ���
			cv::resize(input_display, image_resize, cv::Size(20, 20), 0, 0, cv::INTER_AREA);
			image_resize.convertTo(image_flaot, CV_32FC1);	//�̹��� Ÿ���� �ٲٱ� ����
			image_flatten = image_flaot.reshape(1, 1);	//1 * 400 �ȼ� ��źȭ
			knn->findNearest(image_flatten, 5, resouce);	//	k = 3, 5, 7, 9(7�� best)
			std::cout << cvRound(resouce.at<float>(0, 0)) << std::endl;
			input_display.setTo(0);	//ȭ���� �˰� �ʱ�ȭ
			cv::imshow("DISPLAY", input_display);
		}
	}

	cv::destroyAllWindows();
}