#ifndef speedLimit_H
#define speedLimit_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

class speedLimit{
	public:
		speedLimit(){};
		std::vector<cv::Vec3f> findCircles(cv::Mat imgSrc);
		cv::Mat extractSignRoi(cv::Mat imgSrc, cv::Vec3f circle);
		std::vector<cv::Mat> signRec(cv::Mat imgSrc);
		bool checkRoi(cv::Mat imgSrc);
		void saveTem(std::string imgTxt);
		std::vector<cv::Mat> readTem(); 
		int recognizeDigit(cv::Mat imgSrc, std::vector<cv::Mat> templates, int threshold);
		~speedLimit(){};
};

#endif
