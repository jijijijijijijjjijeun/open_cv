#pragma once
#include <opencv2/opencv.hpp>   

static const cv::String model = "res10_300x300_ssd_iter_140000_fp16.caffemodel";
static const cv::String config = "deploy.prototxt";

void  detect_dnn_face()
{
    cv::VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cerr << "Camera is not move" << std::endl;
        return;
    }
    cv::dnn::Net net = cv::dnn::readNet(model, config);
    if (net.empty())
    {
        std::cerr << "Net open fail" << std::endl;
    }
    cv::Mat frame;
    cv::namedWindow("FRAME");
    while (true)
    {
        capture >> frame;
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), \
            cv::Scalar(104, 177, 123));
        net.setInput(blob);
        cv::Mat result = net.forward();
        cv::Mat detect(result.size[2], result.size[3], CV_32FC1, result.ptr < float>());
        for (int i = 0; i < detect.rows; ++i)
        {
            float confidence = detect.at<float>(i, 2);    //정확도
            if (confidence < 0.7) break;
            int x1 = cvRound(detect.at<float>(i, 3) * frame.cols);
            int y1 = cvRound(detect.at<float>(i, 4) * frame.rows);
            int x2 = cvRound(detect.at<float>(i, 5) * frame.cols);
            int y2 = cvRound(detect.at<float>(i, 6) * frame.rows);

            cv::rectangle(frame, cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)), cv::Scalar(0, 0, 255));
            cv::String label = cv::format("Face confidence : %5.3f", confidence);
            cv::putText(frame, label, cv::Point(x1, y1 - 1), \
                cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0));
        }
        cv::imshow("FRAME", frame);
        if (cv::waitKey(10) == 27) break;
    }
}
void  detect_dnn_face1()
{
    cv::VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cerr << "Camera is not move" << std::endl;
        return;
    }
    cv::dnn::Net net = cv::dnn::readNet(model, config);
    if (net.empty())
    {
        std::cerr << "Net open fail" << std::endl;
    }
    cv::Mat frame;
    cv::namedWindow("FRAME");
    int capture_counter = 0;
    while (true)
    {
        capture >> frame;
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), \
            cv::Scalar(104, 177, 123));
        net.setInput(blob);
        cv::Mat result = net.forward();
        cv::Mat detect(result.size[2], result.size[3], CV_32FC1, result.ptr < float>());
        for (int i = 0; i < detect.rows; ++i)
        {
            float confidence = detect.at<float>(i, 2);    //정확도
            if (confidence < 0.7) break;
            int x1 = cvRound(detect.at<float>(i, 3) * frame.cols);
            int y1 = cvRound(detect.at<float>(i, 4) * frame.rows);
            int x2 = cvRound(detect.at<float>(i, 5) * frame.cols);
            int y2 = cvRound(detect.at<float>(i, 6) * frame.rows);

            cv::rectangle(frame, cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)), cv::Scalar(0, 0, 255));
            cv::String label = cv::format("Face confidence : %5.3f", confidence);
            cv::putText(frame, label, cv::Point(x1, y1 - 1), \
                cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0));
        }
        cv::imshow("FRAME", frame);
        int key = cv::waitKey(10);
        if (key == 27)  // ESC 키를 누르면 종료
            break;
        else if (key == 13)  // 엔터 키를 누르면 캡쳐 모드
        {
            // 현재 프레임을 파일로 저장
            std::string filename = "C:/code/CPP/OpenCv/opencv_project/face/captured_frame_" + std::to_string(capture_counter) + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "Captured frame saved to: " << filename << std::endl;
            capture_counter++;
        }
    }
}