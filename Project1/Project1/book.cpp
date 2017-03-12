#include <api/baseapi.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <time.h>	

using namespace std;
using namespace cv;

void warptrans(Mat,Mat&, vector<Point>);
void morphology(Mat, Mat &, int, int, int);
Mat resize(Mat, Mat&, double);

Rect Label(Mat img, Mat &dst, int threshw, int threshh) {
	Mat img_gray, img_color, img_binary;
	img_gray = img;
	GaussianBlur(img_gray, img_gray, Size(3, 3), 0);
	adaptiveThreshold(img, img_binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 5, 2);

	cvtColor(img_gray, img_color, COLOR_GRAY2BGR);

	Mat img_labels, stats, centroids;
	int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
		stats, centroids, 8, CV_32S);


	////�󺧸��� �̹����� Ư�� ���� �÷��� ǥ�����ֱ�  
	//for (int y = 0; y<img_labels.rows; ++y) {

	//	int *label = img_labels.ptr<int>(y);
	//	Vec3b* pixel = img_color.ptr<Vec3b>(y);


	//	for (int x = 0; x < img_labels.cols; ++x) {
	//		if (label[x] == 3) {
	//			pixel[x][2] = 0;
	//			pixel[x][1] = 255;
	//			pixel[x][0] = 0;
	//		}
	//	}
	//}

	{

	}
	vector<Point> pt1;
	vector<Point> pt2;
	cout << stats << endl;
	for (int i = 1; i < numOfLables; i++) {
		cout << stats.at<int>(i, 4) << endl;

	}

	//�󺧸� �� �̹����� ���� ���簢������ �ѷ��α� 
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);
		if (width < threshw)
			continue;
		if (height < threshh)
			continue;
		pt1.push_back(Point(left, top));
		pt2.push_back(Point(left + width, top + height));


		Point pp = stats.at<int>(j);
		cout << "p1: " << pt1[j - 1];
		cout << " p2: " << pt2[j - 1] << endl;
		rectangle(img_color, Point(left, top), Point(left + width, top + height),
			Scalar(0, 0, 255), 1); 
		putText(img_color, to_string(j), Point(left + 20, top + 20),
			FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
		//imshow("labeling", img_org);
	}
	Rect roi(pt1[0], pt2[0]);

	dst = img_color;
	return roi;
}
Rect Label(Mat img, Mat &dst,int thresh = 0 ) {
	Mat img_gray, img_color, img_binary;

	img_gray = img;
	GaussianBlur(img_gray, img_gray, Size(3, 3), 0);
	adaptiveThreshold(img, img_binary, 255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,5,2);
 
	cvtColor(img_gray, img_color, COLOR_GRAY2BGR);

	Mat img_labels, stats, centroids;
	int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
		stats, centroids, 8, CV_32S);


	////�󺧸��� �̹����� Ư�� ���� �÷��� ǥ�����ֱ�  
	//for (int y = 0; y<img_labels.rows; ++y) {

	//	int *label = img_labels.ptr<int>(y);
	//	Vec3b* pixel = img_color.ptr<Vec3b>(y);


	//	for (int x = 0; x < img_labels.cols; ++x) {
	//		if (label[x] == 3) {
	//			pixel[x][2] = 0;
	//			pixel[x][1] = 255;
	//			pixel[x][0] = 0;
	//		}
	//	}
	//}

	{

	}
	vector<Point> pt1;
	vector<Point> pt2;
	cout << stats << endl;
	for (int i = 1; i < numOfLables; i++) {
		cout << stats.at<int>(i, 4) << endl;

	}

	int vlev=0;
	int gvalue=0;


	//�󺧸� �� �̹����� ���� ���簢������ �ѷ��α� 
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);
		if (width*height < thresh)
			continue;
		pt1.push_back(Point(left, top));
		pt2.push_back(Point(left + width, top + height));

		
		Point pp = stats.at<int>(j);
		cout << "p1: " << pt1[j - 1];
		cout << " p2: " << pt2[j - 1] << endl;
		rectangle(img_color, Point(left, top), Point(left + width, top + height),
			Scalar(0, 0, 255), 1);

		putText(img_color, to_string(j), Point(left + 20, top + 20),
			FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
	}
	 
	Rect roi(pt1[0],pt2[0]);

	dst = img_color;
	return roi;
}



//OCR�� �ؽ�Ʈ�� ����մϴ�.
//img : �ؽ�Ʈ�� ��� �� �̹�������
//str : �ؽ�Ʈ�� ���� �� �̹��� ����
string OCRtext(Mat img, string &str) {
	Mat img_org = img;
	char *outText;
	//    const char *path = ".\\tessdata";  
	char *name;

	string data;
	char* result;

	if (img.type() != 0) {
		cvtColor(img, img, CV_BGR2GRAY);
	}
	Scalar lowerb(170, 170, 170);
	Scalar upperb(255, 255, 255);

	inRange(img, lowerb, upperb, img);
	img = ~img;
	Rect mark;
	mark = Label(img, img, 1000);
	if (mark.area() == 0) {
		cout << "Fail is Mark detected " << endl;
		return "NULL" ;
	}
	//imshow("bookimg", img);
	imshow("markmark", img_org(mark));
	string cutname;
	cutname += "outmat/";
	cutname += to_string(time(NULL));
	cutname += ".jpg";
	imwrite(cutname,img_org(mark));
	img = img_org(mark);
	img = ~img;
	if (img.type() != 0) {
		cvtColor(img, img, CV_BGR2GRAY);
	}


	resize(img, img, 2);
	tesseract::TessBaseAPI api;
	str += "--------------------------------------\n";

	api.Init("../libdata/tessdata", "kor+eng", tesseract::OEM_DEFAULT);
	//api.Init(NULL, "kor+eng", tesseract::OEM_DEFAULT); 
	api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	api.SetImage((uchar*)img.data, img.cols, img.rows, 1, img.cols);
	api.Recognize(0);
	str += api.GetUTF8Text();
	str += "--------------------------------------\n";
	cout << str << endl;
	return str;


}

//warptrans
//img_org : ������ �̹���
//img_dst : ���� �� Mat
//vector<Point> : �簢�� ��ǥ�� ���� �� ��ǥ, 4���� Point�� �ʿ�
//���� : Top+Left, Top+Right, Bottom+Left, Bottom+Right
void warptrans(Mat img_org,Mat &img_dst, vector<Point> qurd)
{
	Point TopLeft = Point(qurd[0]);
	Point TopRight = Point(qurd[1]);
	Point BottomLeft = Point(qurd[2]);
	Point BottomRight = Point(qurd[3]);

	vector<Point> rect;
	rect.push_back(TopLeft);   
	rect.push_back(TopRight);    
	rect.push_back(BottomRight);   
	rect.push_back(BottomLeft);   

	double w1 = sqrt(pow(BottomRight.x - BottomLeft.x, 2)
		+ pow(BottomRight.x - BottomLeft.x, 2));
	double w2 = sqrt(pow(TopRight.x - TopLeft.x, 2)
		+ pow(TopRight.x - TopLeft.x, 2));

	double h1 = sqrt(pow(TopRight.y - BottomRight.y, 2)
		+ pow(TopRight.y - BottomRight.y, 2));
	double h2 = sqrt(pow(TopLeft.y - BottomLeft.y, 2)
		+ pow(TopLeft.y - BottomLeft.y, 2));

	double maxWidth = (w1 < w2) ? w1 : w2;
	double maxHeight = (h1 < h2) ? h1 : h2;

	Point2f src[4], dst[4];
	src[0] = Point2f(TopLeft.x, TopLeft.y);
	src[1] = Point2f(TopRight.x, TopRight.y);
	src[2] = Point2f(BottomRight.x, BottomRight.y);
	src[3] = Point2f(BottomLeft.x, BottomLeft.y);

	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(maxWidth - 1, 0);
	dst[2] = Point2f(maxWidth - 1, maxHeight - 1);
	dst[3] = Point2f(0, maxHeight - 1);

	Mat transformMatrix;
	transformMatrix = getPerspectiveTransform(src, dst);

	Mat srcFrame, dstFrame;
	srcFrame = img_dst;
	warpPerspective(img_org, img_dst, transformMatrix, Size(maxWidth, maxHeight));
	return ;
}
 

//������ ������
//img : ������ ������ �̹���
//dst : ����� ����� ������ �̹���
//size : �ش�Ǵ� ����ŭ ����� ���, Ȯ��
//		1 = 1��(����)
//		2 = 2�� Ȯ��
//		0.2 = 5�� ���
Mat resize(Mat img, Mat &dst, double size) {
	Mat image = img;
	if (size < 1)
		resize(img, dst, Size((double)img.cols * size, (double)img.rows * size), 0, 0, INTER_AREA);
	else if (size >= 1)
		resize(img, dst, Size((double)img.cols * size, (double)img.rows * size), 0, 0, INTER_LINEAR);
	return dst;
}

//img1 : ���������� ������ �̹���
//img2 : �������� �����ų �̹���
//iterations : �ݺ�Ƚ��
//size : Ŀ���� ũ��
//type : �������������� Ÿ��
//		MORPH_OPEN : open����
//		MORPH_CLOSE : close����
void morphology(Mat img1, Mat &img2, int iterations = 1, int size = 1, int type = MORPH_CLOSE) {
	Size size2(size, size);
	Mat rectKernel = getStructuringElement(MORPH_RECT, size2);
	Point anchor(-1, -1);
	morphologyEx(img1, img2, type, rectKernel, anchor, iterations);
}

//img1 : ������ ������ gray�̹���
//img2 : ����� ������ ���� �̹���
//thres : ������ ���� �Ӱ谪
Mat HoughLines1(Mat img1, Mat &img2,int thres = 100) {
	Vec2f params;
	float rho, theta;
	float c, s;
	float x0, y0;
	vector<Vec2f> lines;
	HoughLines(img1, lines, 1, CV_PI / 180, thres);
	cout << "lines.size() = " << lines.size() << endl;

	for (int k = 0; k < lines.size(); k++) {
		params = lines[k];
		rho = params[0];
		theta = params[1];
		
		//draw
		c = cos(theta);
		s = sin(theta);
		x0 = rho * c;
		y0 = rho * s;
		if (acos(s)*180/CV_PI < 75)
			continue;
		Point pt1, pt2;
		
		
		pt1.x = cvRound(x0 + 10000* (-s)) + 5;
		pt1.y = cvRound(y0 + 10000 * (c));
		pt2.x = cvRound(x0 - 10000 * (-s)) + 5;
		pt2.y = cvRound(y0 - 10000 * (c));

		line(img2, pt1, pt2, Scalar(0, 255, 255), 2);
	}
	return img2;

}


int main() {
	//��� �� �̹��� ���
	Mat img_org = imread("Image/resize/008.jpg", IMREAD_COLOR);
	//Mat img_org = imread("Image/resize/001.jpg", IMREAD_COLOR);
	clock_t time;
	time = clock();
	if (img_org.empty())
		return -1;
	resize(img_org, img_org,Size(1920,1080),0,0, INTER_AREA);
	resize(img_org, img_org, 0.5);
	Mat img_gray;

	Mat img_mop;

	//��ü���� �̹����� ��Ȱȭ�� ���� ����
	Mat img_dst;
	{
		int border = 3;	//7x7
		Size ksize(border * 2 + 1, border * 2 + 1);
		boxFilter(img_org, img_dst, -1, ksize);
		//imshow("dst1", img_dst1);

		int d = ksize.width;
		double sigmaColor = 10.0 * 8;
		double sigmaSpace = 10.0 * 8;
		bilateralFilter(img_org, img_dst, d, sigmaColor, sigmaSpace);
	}

	imshow("img_blur", img_dst);
	//��Ÿ �� ������ �����ϱ� ���� �������� ����
	morphology(img_dst, img_dst, 2, 3, MORPH_CLOSE);
	//imshow("blurMop", img_dst);

	cvtColor(img_dst, img_gray, CV_BGR2GRAY);
	//imshow("img_gray", img_gray);

	//��ü���� �̹����� ��踦 ���� �� �������� �������� �⿵ ����
	adaptiveThreshold(img_gray, img_gray, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 5);
	morphology(img_gray, img_mop, 2, 3, MORPH_CLOSE);

	//sobel �������� x�� ��̺� ����
	Mat img_sobel;
	Sobel(img_mop, img_sobel, 0, 1, 0);
	//imshow("sobel1", img_sobel);

	//��̺� �� ���� �⿵ ����
	morphology(img_sobel, img_mop, 2, 3, MORPH_CLOSE);
	//imshow("MORPH_CLOSE2", img_mop);


	//�뷫���� �� ���, �ϳ��� å ���ο� ���� ���� ��ø�ȴ�.
	Mat img_clone(img_org.size(), img_org.type());
	img_clone = HoughLines1(img_mop, img_clone, 180);
	//imshow("lines1", img_clone);

	//��ø�� ������ ������ ����� ���� �������� ����
	morphology(img_clone, img_clone, 8, 3, MORPH_CLOSE);

	//imshow("dilate", img_clone);
	Sobel(img_clone, img_clone, 0, 1, 0, 3, 1, 0, BORDER_CONSTANT);
	
	// �뷫���� ��ġ�� ����
	vector<Point> middlep, bottomp;
	for (int i = 0; i < img_clone.cols; i++) {
		int pos = (i);
		Vec3b* pixel = img_clone.ptr<Vec3b>(img_clone.rows / 2);
		if (pixel[pos][0] == 0 && pixel[pos][1] == 255 && pixel[pos][2] == 255) {
			cout << "middle : " << i << endl;
			circle(img_clone, Point(pos, img_clone.rows / 2), 2, Scalar(255, 255, 0), 3);
			middlep.push_back(Point2f(pos, img_clone.rows / 2));
		}
	}
	for (int i = 0; i < img_clone.cols; i++) {
		int pos = (i);
		Vec3b* pixel = img_clone.ptr<Vec3b>(img_clone.rows-1);
		if (pixel[pos][0] == 0 && pixel[pos][1] == 255 && pixel[pos][2] == 255) {
			cout << "bottom : " << i << endl;
			circle(img_clone, Point(pos, img_clone.rows -1), 2, Scalar(255, 255, 0), 3);
			bottomp.push_back(Point2f(pos, img_clone.rows-1));
		}
	} 


	imshow("dilsob", img_clone);
	waitKey();

	//ofstream outfile("OCRtext.txt", ios::app);
	ofstream outfile("OCRtext.txt");
	Mat img_warp;
	outfile.clear();
	for (int i = 0; i < middlep.size() - 1; i++) {
		vector<Point> qurdp;
		//��ܿ��� ���� �� ��ǥ���� vector qurdp�� ����, ���� : Top+Left, Top+Right, Bottom+Left, Bottom+Right
		qurdp.push_back(middlep[i]);
		qurdp.push_back(middlep[i+1]);
		qurdp.push_back(bottomp[i]);
		qurdp.push_back(bottomp[i+1]);
		//���� �� �̹����� �������� ������ Mat ����, ��ǥ ����
		img_warp.zeros(Size(0,0),CV_16UC3);
		warptrans(img_org, img_warp, qurdp);
		
		string bname="book";
		//imshow���� window �̸��� ��ø�����ʵ��� ���� �߰�
		bname += to_string(i);
		
		string marktext;
		//�̹��� ���Ͽ��� �ؽ�Ʈ�� ���
		cout << bname << endl;

		waitKey();
		OCRtext(img_warp, marktext);
		outfile << marktext << endl;

		imshow(bname, img_warp);
	}
	outfile.close();
	waitKey();
	 
//��ü ����ð� ���
	time = clock() - time;
	cout << "time = " << ((double)time / CLOCKS_PER_SEC) << endl;
	waitKey();
	return 0; 

}


