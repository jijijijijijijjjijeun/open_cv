#include"opencv2/opencv.hpp" // iostream포함
using namespace std;
using namespace cv;

void show_code02_02()
{
	Mat image1{ imread("Lenna1.png",IMREAD_GRAYSCALE) };//"Lenna1.png"라는 이름의 파일 이미지를 읽어오세요. 
	Mat image2{ imread("cc.bmp") };
	Mat image3;
	if (image1.empty() || image2.empty())
	{
		cerr << "files are not found" << endl;
		return;
	}
	image3 = image1.clone();	//사진이 복사 되어 각자 다른 사진을 가르키고 있음
	//image3 = image1;			//주소가 복사가 되어서 image1을 같이 보고 있음
	cout << "몇 차원? " << image1.dims << endl;
	cout << "col :" << image1.cols << endl;
	cout << "row :" << image1.rows << endl;
	namedWindow("Lenna");	//창이름
	imshow("Lenna", image1);
	namedWindow("Hamzzi");	//창이름
	imshow("Hamzzi", image2);
	namedWindow("Clone");	//창이름
	imshow("Clone", image3);
	waitKey(0);
}

void show_code02_03()
{
	//for (int i = 0; i < 10; i++)
	//{
	//	string str = format("temp %d",i);		//string("temp" + i)이렇게 하면 안된다. 형식지정을 해줘야 함
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