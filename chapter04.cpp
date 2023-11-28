#pragma once
#include"opencv2/opencv.hpp" // iostream포함

using namespace std;
using namespace cv;

static void on_mouse(int , int , int , int , void*);

static void on_level_changed(int, void*);

void show_camera()
{
	cv::VideoCapture cap(0);	//default my webcamera
	if (!cap.isOpened())
	{
		std::cout << "Camear does not exist" << std::endl;
		return;
	}
	std::cout << "Hight : " << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));	//double -> round
	std::cout << "Width : " << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	cv::Mat frame;
	while (1)
	{
		cap >> frame;
		if (frame.empty())
		{
			std::cout << "frame has a problem" << std::endl; 
			break;
		}
		cv::imshow("CAMERA", ~frame);
		if (cv::waitKey(10) == 27)
		{
			break;
		}
	}
	cap.release();
	cv::destroyAllWindows();
}

void show_movie()
{
	cv::VideoCapture cap("stopwatch.avi");
	if (!cap.isOpened())
	{
		std::cerr << "No Camera" << std::endl;
		return;
	}
	std::cout << "Hight : " << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT)) << std::endl;
	std::cout << "Width : " << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH)) << std::endl;
	std::cout << "count : " << cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT)) << std::endl;
	double fps = cap.get(cv::CAP_PROP_FPS);
	int  delay = cvRound(1000 / fps);

	cv::Mat frame;
	cv::Mat inversed;
	while (1)
	{
		cap >> frame; 
		inversed = ~frame;
		if (frame.empty())
		{
			break;
		}
		cv::imshow("CAMERA", frame);
		cv::imshow("INVERSED", inversed);
		if (cv::waitKey(delay)==27)
		{
			break;
		}
	}cap.release();
	cv::destroyAllWindows();
}

void show_video_record()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cerr << "카메라가 없습니다." << std::endl;
		return;
	}
	int hight = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));

	std::cout << hight << std::endl;
	std::cout << width << std::endl;

	double fps = cap.get(cv::CAP_PROP_FPS);
	int delay = cvRound(1000 / fps);
	cv::Mat frame;
	int fourcc = cv::VideoWriter::fourcc('X', '2', '6', '4');//4개의 코덱	p.140
	cv::VideoWriter output("out-put.mp4", fourcc, fps, cv::Size(width, hight));
	if (!output.isOpened())
	{
		std::cerr << "파일이 문제가 있습니다." << std::endl;
		return;
	}
	while (1)
	{
		cap >> frame;
		if (frame.empty())
		{
			std::cerr << "프레임이 적절하게 만들어지지 못합니다." << std::endl;
			return;
		}
		output << frame;
		cv::imshow("CAMREA", frame);
		if (cv::waitKey(delay) == 27)
		{
			break;
		}
		cv::destroyAllWindows();
	}
}

void show_draw_lines()	//p.146
{
	cv::Mat canvas(600, 600, CV_8UC(3), cv::Scalar(255, 255, 255));
	cv::line(canvas, cv::Point(50, 50), cv::Point(200, 50), cv::Scalar(0, 0, 255));
	cv::line(canvas, cv::Point(50, 50), cv::Point(200, 100), cv::Scalar(255, 0, 255),10);
	cv::arrowedLine(canvas, cv::Point(50, 200), cv::Point(150, 200), cv::Scalar(0, 255, 0), 2);
	cv::drawMarker(canvas, cv::Point(30, 350), cv::Scalar(0, 0, 255), cv::MARKER_CROSS);
	cv::drawMarker(canvas, cv::Point(100, 350), cv::Scalar(100, 0, 255), cv::MARKER_DIAMOND);
	cv::drawMarker(canvas, cv::Point(200, 350), cv::Scalar(100, 50, 255), cv::MARKER_STAR);
	cv::drawMarker(canvas, cv::Point(300, 350), cv::Scalar(100, 50, 255), cv::MARKER_TILTED_CROSS);
	imshow("CANVAS", canvas);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void show_draw_polygons()
{
	cv::Mat canvas(500, 500, CV_8UC(3), cv::Scalar(255, 255, 255));			//선의 굵기 : 3
	cv::rectangle(canvas, cv::Rect(50, 50, 100, 50), cv::Scalar(0, 0, 255), 3);
	cv::rectangle(canvas, cv::Rect(50, 150, 100, 50), cv::Scalar(0, 0, 100), -1);
																			//내부채색 : -1
	cv::circle(canvas, cv::Point(300, 120), 30, cv::Scalar(255, 255, 0), -1, cv::LINE_AA);
	cv::ellipse(canvas, cv::Point(120, 300), cv::Size(60, 30), 20, 0, 270, cv::Scalar(255, 255, 0), -1, cv::LINE_AA);

	std::vector<cv::Point> points;
	points.push_back(cv::Point(250, 250));
	points.push_back(cv::Point(300, 250));
	points.push_back(cv::Point(300, 300));
	points.push_back(cv::Point(350, 300));
	points.push_back(cv::Point(350, 350));
	points.push_back(cv::Point(250, 350));
	cv::polylines(canvas, points, true, cv::Scalar(255, 0, 255), 5);

	cv::imshow("CANVAS", canvas);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void show_draw_text()
{
	cv::Mat canvas(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
	cv::putText(canvas, "I LOVE YOU", cv::Point(20, 50), cv::FONT_HERSHEY_COMPLEX | cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255));
	cv::imshow("CANVAS", canvas);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void show_draw_center()
{
	cv::Mat canvas(200, 640, CV_8UC3, cv::Scalar(255, 255, 255));
	const cv::String text = "Hello OPEN CV";
	int font_face = cv::FONT_HERSHEY_PLAIN;
	double font_scale = 2.0;
	int thinkness = 1;
	cv::Size size_text = cv::getTextSize(text, font_face, font_scale, thinkness, 0);
	cv::Size canvas_size = canvas.size();

	cv::Point center  ((canvas_size.width - size_text.width) / 2, (canvas_size.height + size_text.height) / 2);		//텍스트 시작 위치 잡기
	cv::putText(canvas, text, center, font_face, font_scale, cv::Scalar(255, 0, 0), thinkness);
	cv::imshow("CANVAS", canvas);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void keyboard_event()
{
	cv::Mat img = cv::imread("lenna.bmp");
	if (img.empty())
	{
		return;
	}
	cv::imshow("LENNA", img);
	while (1)
	{
		int key_value = cv::waitKey(0);
		std::cout << key_value << std::endl;
		if (key_value == 'i' or key_value =='I')
		{
			img = ~img; 
			cv::imshow("INV_LENNA", img);
		}
		else
		{
			if (key_value == 27 or key_value == 'q' or key_value == 'Q')
			{
				std::cout << "종료" << std::endl;
				break;
			}
		}
	}
	//cv::waitKey(0);
	cv::destroyAllWindows();
}

static cv::Mat img;
 void mouse_event()	//p.163
{
	img = cv::imread("Lenna1.png");
	if (img.empty())
	{
		return;
	}
	cv::namedWindow("LENNA");
	cv::setMouseCallback("LENNA", on_mouse);
	cv::imshow("LENNA", img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}   

static cv::Point pt_old;
void on_mouse(int mouse_event, int mouse_x, int mouse_y, int flag, void*)
{
	switch (mouse_event)
	{
	case cv::EVENT_LBUTTONDOWN:	//왼쪽 마우스 눌렀을떄
		pt_old = cv::Point(mouse_x, mouse_y);
		std::cout << "EVENT_LBUTTONDOWN: " << mouse_x << " , " << mouse_y << std::endl;
		break;
	case cv::EVENT_LBUTTONUP:
		pt_old = cv::Point(mouse_x, mouse_y);
		std::cout << "EVENT_LBUTTONUP: " << mouse_x << " , " << mouse_y << std::endl;
		break;
	case cv::EVENT_MOUSEMOVE:
		if (flag & EVENT_FLAG_LBUTTON)		// & = and
		{
			cv::line(img, pt_old, cv::Point(mouse_x, mouse_y), cv::Scalar(0, 255, 255), 2);
			cv::imshow("LENNA", img);
			pt_old = cv::Point(mouse_x, mouse_y);
		}
		break;
	default:
		break;
	}
}

void trackbar_event()
{
	cv::Mat canvas(800, 800, CV_8UC1);
	cv::namedWindow("CANVAS");
	cv::createTrackbar("Level", "CANVAS", 0, 16, on_level_changed, (void*)&canvas);
	cv::imshow("CANVAS", canvas);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void on_level_changed(int pos, void* user_data)
{
	cv::Mat img = *(static_cast<cv::Mat*>(user_data));	//주소로 가서 값찾아옴
	img.setTo(pos * 16);
	cv::imshow("CANVAS", img);
}

void mask_setTo()
{
	cv::Mat src = cv::imread("Lenna1.png");
	cv::Mat mask = cv::imread("mask_smile.bmp", cv::IMREAD_GRAYSCALE);
	if (src.empty() or mask.empty())
	{
		return;
	}
	src.setTo(cv::Scalar(0, 255, 0), mask);
	cv::imshow("LENNA", src);
	cv::imshow("MASK", mask);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void mask_copyTo()
{
	cv::Mat src = cv::imread("airplane.bmp");
	cv::Mat mask = cv::imread("mask_plane.bmp",cv::IMREAD_GRAYSCALE);
	cv::Mat dst = cv::imread("field.bmp");
	cv::imshow("ORIGINAL", dst);
	cv::imshow("SRC", src);
	cv::imshow("MASK", mask);
	if (src.empty() or mask.empty() or dst.empty())
	{
		return;
	}
	src.copyTo(dst, mask);
	cv::imshow("CHANGED", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void time_inverse()		//p.187
{
	cv::Mat img {cv::imread(("lenna.bmp"), cv::IMREAD_GRAYSCALE)};
	if (img.empty())
	{
		return;
	}
	cv::imshow("LENNA", img);
	cv::Scalar totoal_pixel = cv::sum(img)[0];
	std::cout << "총 픽셀은 : " << static_cast<int>(totoal_pixel[0]) << std::endl;
	cv::Scalar mean_pixel = cv::mean(img)[0];
	std::cout << "평균 픽셀은 : " << static_cast<int>(mean_pixel[0]) << std::endl;
	cv::Mat dst{cv::Mat(img.rows, img.cols, img.type())};		//복사 GRAYSCALE로
	double min_vale = 0.0;
	double max_vale = 0.0; 
	cv::Point min_pt;
	cv::Point max_pt;
	cv::minMaxLoc(img, &min_vale, &max_vale, &min_pt, &max_pt);
	std::cout << "min value : " << min_vale << std::endl;
	std::cout << "max value : " << max_vale << std::endl;
	std::cout << "min poition : " << min_pt << std::endl;
	std::cout << "max poition : " << max_pt << std::endl;
	cv::TickMeter tick;
	tick.start();	//시간 시작
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			//dst.at<uchar>(i, j) = img.at<uchar>(i, j);//단순 복사
			dst.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);//반전 복사
		}
	}
	tick.stop();	//시간끝 => 복사되는 시간 체크
	std::cout << "연산에 걸린 시간 : " << tick.getTimeMilli() << std::endl;
	cv::imshow("INVERSE", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}