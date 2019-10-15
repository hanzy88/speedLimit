#include <iostream>
#include "include/speedLimit.h"

int main(int argc, char* argv[])
{
	cv::Mat src_img;
	src_img = cv::imread(argv[1], 1);

	if(argv[1] == NULL)
	{
		fprintf(stderr, "The image should input for speedlimit, as ./speedLimit 1.jpg\n");
		exit(1);
	}

	az::speedLimit process;
	
	if(argv[2] != NULL)
	{
		fprintf(stderr, "Input the file of template image, init the tem.txt\n");		
		process.saveTem(argv[2]);
	}

	std::vector<cv::Mat> rois;
	rois = process.signRec(src_img);

	std::vector<cv::Mat> out = process.readTem();
	
	
	for(int i = 0;i < rois.size();i++){
		cv::namedWindow("readDigit");
		cv::imshow("readDigit", rois[i]);
		cv::waitKey();
	}
	
	
	for(size_t i = 0;i < rois.size();i++){
		cv::Mat roi = rois[i];
		
		cv::Mat cRoi = roi.clone();
		if(!process.checkRoi(cRoi))
		{
			continue;
		}
		
		int num = process.recognizeDigit(roi, out, 55);
		if(num == 0)
		{
			printf("The traffic sign of the speed limit is 100.\n");
		}else if(num == 1)
		{
			printf("The traffic sign of the speed limit is 50 \n");
		}else if(num == 2)
		{
			printf("The traffic sign of the speed limit is 70 \n");
		}else if(num == 3)
		{
			printf("The traffic sign of the speed limit is 80 \n");
		}
	}
	return 0;
}
