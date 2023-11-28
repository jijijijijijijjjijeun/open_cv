#include"opencv2/opencv.hpp" // iostream����
using namespace std;
using namespace cv;

void show_code02_02()
{
	Mat image1{ imread("Lenna1.png",IMREAD_GRAYSCALE) };//"Lenna1.png"��� �̸��� ���� �̹����� �о������. 
	Mat image2{ imread("cc.bmp") };
	Mat image3;
	if (image1.empty() || image2.empty())
	{
		cerr << "files are not found" << endl;
		return;
	}
	image3 = image1.clone();	//������ ���� �Ǿ� ���� �ٸ� ������ ����Ű�� ����
	//image3 = image1;			//�ּҰ� ���簡 �Ǿ image1�� ���� ���� ����
	cout << "�� ����? " << image1.dims << endl;
	cout << "col :" << image1.cols << endl;
	cout << "row :" << image1.rows << endl;
	namedWindow("Lenna");	//â�̸�
	imshow("Lenna", image1);
	namedWindow("Hamzzi");	//â�̸�
	imshow("Hamzzi", image2);
	namedWindow("Clone");	//â�̸�
	imshow("Clone", image3);
	waitKey(0);
}

void show_code02_03()
{
	//for (int i = 0; i < 10; i++)
	//{
	//	string str = format("temp %d",i);		//string("temp" + i)�̷��� �ϸ� �ȵȴ�. ���������� ����� ��
	//	cout << str << endl;
	//}
	
	namedWindow("IMAGE");
	for (int i = 0; i < 256; i++)
	{
		Mat image(512, 512, CV_8UC3, Scalar(0,0,i));
		imshow("IMAGE", image);
		waitKey(5);
	}
	waitKey(0);
}