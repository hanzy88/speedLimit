
#include "../include/speedLimit.h"

#define log_test fprintf(stderr, "file: %s, line: %d, function: %s\n", __FILE__, __LINE__, __FUNCTION__)

namespace az{

std::vector<cv::Vec3f> speedLimit::findCircles(cv::Mat imgSrc)
{
	cv::Mat img;
	cv::Mat gray;
	std::vector<cv::Vec3f> circles;

	cv::medianBlur(imgSrc, img, 5);
	cv::cvtColor(imgSrc, gray, CV_BGR2GRAY);

	cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, 20, 100, 30, 10, 50);
	return circles;
}

cv::Mat speedLimit::extractSignRoi(cv::Mat imgSrc, cv::Vec3f circle)
{
	int x = cvRound(circle[0]);
	int y = cvRound(circle[1]);
	int r = cvRound(circle[2]);
	
	int rn = int(r - (r / 5));
	cv::Rect rect(x - rn, y-rn, 2 * rn, 2 * rn);
	cv::Mat roi = imgSrc(rect);
	return roi;
}

std::vector<cv::Mat> speedLimit::signRec(cv::Mat imgSrc)
{
	std::vector<cv::Vec3f> circles;
	circles = findCircles(imgSrc);
	std::vector<cv::Mat> rois;
	

	if(!circles.empty())
	{
		for(size_t i = 0;i <circles.size();i++)
		{
			cv::Mat roi = extractSignRoi(imgSrc, circles[i]);
			rois.push_back(roi);
		}
	}
	return rois;
}

bool speedLimit::checkRoi(cv::Mat imgSrc)
{
	cv::Mat gray;

	cv::cvtColor(imgSrc, gray, CV_BGR2GRAY);

	cv::Mat kern = (cv::Mat_<char>(3,3) << 0, -1 , 0,-1, 5,-1, 0, -1, 0);

	cv::Mat dstImg;
	cv::filter2D(imgSrc,dstImg,-1,kern);
	
	cv::Mat thresh;
	cv::threshold(dstImg, thresh, 60, 255, cv::THRESH_BINARY);

	int cRoi = int(thresh.at<unsigned char>(0, thresh.cols/2));
	if(cRoi != 0)
	{
		return false;
	}
	
	return true;
}

void speedLimit::saveTem(std::string imgPath)
{
	std::vector<cv::String> imgFile;
	cv::glob(imgPath, imgFile);

	FILE* pp = fopen("tem.txt", "wb");
	int imgNum = imgFile.size();
	fprintf(pp, "%d\n", imgNum);

	for(int i = 0;i < imgNum;i++){
		cv::Mat imgSrc = cv::imread(imgFile[i]);

		cv::Mat gray;
		cv::cvtColor(imgSrc, gray, CV_BGR2GRAY);

		cv::Mat gausOut;
		cv::GaussianBlur(gray, gausOut, cv::Size(3, 3), 0, 0);
		
		cv::Mat dst;
		cv::adaptiveThreshold(gausOut, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 3);

		size_t rows = dst.rows;
		size_t cols = dst.cols;

		unsigned char tmp;
		
		
		fprintf(pp, "%zu %zu\n", rows, cols);
		for(size_t row = 0; row < rows;row++)
		{
			for(size_t col = 0; col < cols;col++)
			{
				tmp = dst.at<unsigned char>(row, col);
				fprintf(pp, "%hhu ", tmp);
			}
			fprintf(pp, "\n");
		}
	}
	fclose(pp);
}

std::vector<cv::Mat> speedLimit::readTem()
{
	FILE * fq = fopen("tem.txt", "rt+");

	int imgNum;
	fscanf(fq, "%d", &imgNum);
	std::vector<cv::Mat> digits;

	for (int i = 0; i < imgNum; ++i)
	{
		size_t rows, cols;
		int nbr = fscanf(fq, "%zu %zu", &rows, &cols);

		CvSize imgsize;
		imgsize.height = rows;
		imgsize.width = cols;

		unsigned char imgmat[rows][cols];
		for(size_t row = 0; row < rows;row++)
		{
			for(size_t col = 0; col < cols;col++)
			{
				unsigned char tmp;
				int nbr = fscanf(fq, "%hhu ", &tmp);
				imgmat[row][col] = tmp;

			}
		}

		IplImage *src = cvCreateImage(imgsize, IPL_DEPTH_8U, 1);
		src->imageData = (char*)imgmat;

		cv::Mat digit;
		digit = cv::cvarrToMat(src);
		digits.push_back(digit);
	}
	return digits;
}

int speedLimit::recognizeDigit(cv::Mat imgSrc, std::vector<cv::Mat> templates, int threshold = 55)
{
	cv::Mat gray;
	cv::cvtColor(imgSrc, gray, CV_BGR2GRAY);
	
	double bestMatch = 0;
	int bestDigit = 0;
	for(size_t i = 0;i < templates.size();i++)
	{
		cv::Mat temp = templates[i];
		if(imgSrc.rows < temp.rows)
		{
			cv::resize(temp, temp, cv::Size(imgSrc.rows, imgSrc.cols), (0, 0), (0, 0), cv::INTER_LINEAR);
		}else
		{
			cv::resize(imgSrc, imgSrc, cv::Size(temp.rows, temp.cols), (0, 0), (0, 0), cv::INTER_LINEAR);
		}

		cv::Mat gausOut;
		cv::GaussianBlur(gray, gausOut, cv::Size(3, 3), 0, 0);
		
		cv::Mat dst;
		cv::adaptiveThreshold(gausOut, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 3);

		cv::Mat result;
		cv::matchTemplate(dst, temp, result, cv::TM_CCOEFF_NORMED); 
		double minVal = -1;
		double maxVal;
		cv::Point minLoc;
		cv::Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

		if(maxVal > bestMatch)
		{
			bestMatch = maxVal;
			bestDigit = i;
		}
	}
	if((bestMatch * 100) >= threshold)
	{
		return bestDigit;
	}

	return 0;
}

}
