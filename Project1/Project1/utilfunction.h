#pragma once
#include <api/baseapi.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <time.h>	 

using namespace std;
using namespace cv;

class utilopencv {
public:
	utilopencv();
	virtual ~utilopencv();

	double MatAve(Mat src);

	vector<vector<Point>> HoughLinesP(Mat img, Mat &dst, int thresh);

	Mat HoughLines1(Mat img1, Mat &img2, int thres = 100);


	//img1 : ������ ������ gray�̹���
	//img2 : ����� ������ ���� �̹���
	//thres : ������ ���� �Ӱ谪
	//��ȯ�� : ���ǿ� �´� 0��° ��ǥ��
	Rect Label(Mat img, Mat &dst, int thresh = 0);

	//warptrans
	//img_org : ������ �̹���
	//img_dst : ���� �� Mat
	//vector<Point> : �簢�� ��ǥ�� ���� �� ��ǥ, 4���� Point�� �ʿ�
	//���� : Top+Left, Top+Right, Bottom+Left, Bottom+Right
	//�ڵ� ��ó : http://miatistory.tistory.com/5
	void warptrans(Mat img_org, Mat &img_dst, vector<Point> qurd);

	//OCRtext
	//�̹����� �Է��ϸ� �ؽ�Ʈ�� ����մϴ�
	//GRAYSCALE �̹����� �Է��ϴ°��� ��õ�մϴ�.
	//OCRtext : ���� �� �̹���
	//str : ��� �� ���ڿ�
	string OCRtext(Mat img, string &str);

	//resize
	//img : �Է� �� �̹���
	//dst : ��� �� �̹���
	//size : ��� �� �̹����� ��
	//(2�� : size = 2, 0.5�� : size = 0.5 )
	Mat resize(Mat img, Mat &dst, double size);


	//img1 : ���������� ������ �̹���
	//img2 : �������� �����ų �̹���
	//iterations : �ݺ�Ƚ��
	//size : Ŀ���� ũ��
	//type : �������������� Ÿ��
	//		MORPH_OPEN : open����
	//		MORPH_CLOSE : close����
	void morphology(Mat img1, Mat &img2, int iterations = 1, int size = 1, int type = MORPH_CLOSE);

};