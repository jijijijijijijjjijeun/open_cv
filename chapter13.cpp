#pragma once
#include"opencv2/opencv.hpp" // iostreamÆ÷ÇÔ

void template_matching()	//p.407
{
	cv::Mat image = cv::imread("circuit.bmp",cv::IMREAD_COLOR);
	cv::Mat patch_image = cv::imread("crystal.bmp", cv::IMREAD_COLOR);
	if (image.empty() or patch_image.empty())
	{
		std::cout << "file not found" << std::endl;
		return;
	}
	cv::Mat resource;
	cv::matchTemplate(image, patch_image, resource, cv::TM_CCOEFF_NORMED);
	cv::Mat resource_norm;
	cv::normalize(resource, resource_norm, 0.0, 255.0, cv::NORM_MINMAX, CV_8UC1);
	double max_value{ 0.0 };
	cv::Point max_location;
	cv::minMaxLoc(resource, nullptr, static_cast<double*>(&max_value), nullptr, static_cast<cv::Point*>(&max_location));

	std::cout << "-------------------------------------" << std::endl << "max value : " << max_value << std::endl;
	cv::rectangle(image, cv::Rect(max_location.x, max_location.y, patch_image.cols,patch_image.rows), cv::Scalar(0, 0, 255), 2);
	cv::imshow("IMAGE", image); 
	cv::imshow("PATCH", patch_image);
	cv::imshow("RESOLUTION", resource_norm);

	cv::waitKey(0);
	cv::destroyAllWindows();
}