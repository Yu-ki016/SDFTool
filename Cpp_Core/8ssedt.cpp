//#include "SDL/sdl.h"
#include <math.h>
#include <pybind11/pybind11.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<filesystem>

//#define WIDTH  256
//#define HEIGHT 256

struct Point
{
	int dx, dy;

	int DistSq() const { return dx * dx + dy * dy; }
};

struct Grid
{
	//Point grid[HEIGHT][WIDTH];
	std::vector<std::vector<Point>> grid;
	// 构造函数，初始化二维数组大小
	Grid(int width, int height) : grid(height, std::vector<Point>(width)) {}
};

Point inside = { 0, 0 };
Point empty = { 9999, 9999 };
//Grid grid1, grid2;

Point Get(Grid& g, int x, int y)
{
	int width = int(g.grid[0].size());
	int heigh = int(g.grid.size());

	// OPTIMIZATION: you can skip the edge check code if you make your grid 
	// have a 1-pixel gutter.
	if (x >= 0 && y >= 0 && x < width && y < heigh)
		return g.grid[y][x];
	else
		return empty;
}

void Put(Grid& g, int x, int y, const Point& p)
{
	g.grid[y][x] = p;
}

void Compare(Grid& g, Point& p, int x, int y, int offsetx, int offsety)
{
	Point other = Get(g, x + offsetx, y + offsety);
	other.dx += offsetx;
	other.dy += offsety;

	if (other.DistSq() < p.DistSq())
		p = other;
}

void GenerateDistanceField(Grid& g)
{
	int width = int(g.grid[0].size());
	int heigh = int(g.grid.size());
	// Pass 0
	for (int y = 0; y < heigh; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Point p = Get(g, x, y);
			Compare(g, p, x, y, -1, 0);
			Compare(g, p, x, y, 0, -1);
			Compare(g, p, x, y, -1, -1);
			Compare(g, p, x, y, 1, -1);
			Put(g, x, y, p);
		}

		for (int x = width - 1; x >= 0; x--)
		{
			Point p = Get(g, x, y);
			Compare(g, p, x, y, 1, 0);
			Put(g, x, y, p);
		}
	}

	// Pass 1
	for (int y = heigh - 1; y >= 0; y--)
	{
		for (int x = width - 1; x >= 0; x--)
		{
			Point p = Get(g, x, y);
			Compare(g, p, x, y, 1, 0);
			Compare(g, p, x, y, 0, 1);
			Compare(g, p, x, y, -1, 1);
			Compare(g, p, x, y, 1, 1);
			Put(g, x, y, p);
		}

		for (int x = 0; x < width; x++)
		{
			Point p = Get(g, x, y);
			Compare(g, p, x, y, -1, 0);
			Put(g, x, y, p);
		}
	}
}

cv::Mat Color2Gray(cv::Mat& image) {
	using namespace cv;
	Mat grayImage;
	if (image.channels() > 1) {
		cvtColor(image, grayImage, COLOR_BGR2GRAY);
	}
	else {
		grayImage = image;
	}
	return grayImage;
}

std::string GenerateSDF(const std::string& folderPath, const std::string& name)
{
	using namespace cv;
	std::string filePath = folderPath + "/" + name;
	// 在这里执行你的处理逻辑，这里仅示范打印
	std::cout << "Path: " << filePath << std::endl;
	Mat image = imread(filePath);
	if (image.empty()) {
		std::cout << "Can not open file:" << name << std::endl;
		return "";
	}
	Mat grayImage = Color2Gray(image);

	int width = grayImage.cols;
	int height = grayImage.rows;

	Grid grid1(width, height);
	Grid grid2(width, height);
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int pixel = grayImage.at<uchar>(y, x);

			// Points inside get marked with a dx/dy of zero.
			// Points outside get marked with an infinitely large distance.
			if (pixel < 128)
			{
				Put(grid1, x, y, inside);
				Put(grid2, x, y, empty);
			}
			else {
				Put(grid2, x, y, inside);
				Put(grid1, x, y, empty);
			}
		}
	}

	GenerateDistanceField(grid1);
	GenerateDistanceField(grid2);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Calculate the actual distance from the dx/dy
			int dist1 = (int)(sqrt((double)Get(grid1, x, y).DistSq()));
			int dist2 = (int)(sqrt((double)Get(grid2, x, y).DistSq()));
			int dist = dist2 - dist1;

			// Clamp and scale it, just for display purposes.
			//int c = dist * 3 + 128;
			int c = dist * 1.0 + 128;
			//c = dist1;
			if (c < 0) c = 0;
			if (c > 255) c = 255;

			grayImage.at<uchar>(y, x) = c;
		}
	}

	// 指定保存文件夹路径和文件名
	std::string outputFolder = folderPath + "/output/";
	if (std::filesystem::create_directory(outputFolder)) {
		std::cout << "Creat folder:" << outputFolder << std::endl;
	}

	if (imwrite(outputFolder + name, grayImage)) {
		std::cout << "Image Save Successfully:" << outputFolder + name << std::endl;
	}
	else {
		std::cout << "Image Save Failed" << std::endl;
	}
	return outputFolder;
}

void SDFLerp(std::string folderPath) {
	using namespace cv;
	// 在这里执行你的处理逻辑，这里仅示范打印
	std::cout << "SDFLerp,Path: " << folderPath << std::endl;
	std::vector<cv::String> fileNames;
	glob(folderPath, fileNames);

	if (fileNames.empty()) {
		std::cout << "No image in folder." << std::endl;
		return ;
	}

	std::vector<cv::Mat> grayImages;

	for (const cv::String& fileName : fileNames) {
		cv::Mat image = cv::imread(fileName);
		if (image.empty()) {
			std::cerr << "Canot load image: " << fileName << std::endl;
		}
		else {
			grayImages.push_back(Color2Gray(image)); // 将加载的图像添加到向量中
		}
	}

	float result = 0;
	if (grayImages.size() > 1) {
		// 创建一个空白图像


		int levelStep = int(256.0 / (int(grayImages.size()) - 1));
		float lerpStep = 0;
		int curTexIndex = 0;
		int nextTexIndex = 1;

		int width = grayImages[0].cols;
		int height = grayImages[0].rows;

		// 指定保存文件夹路径和文件名
		std::string outputFolder = folderPath + "SDF/";
		if (std::filesystem::create_directory(outputFolder)) {
			std::cout << "Create folder:" << outputFolder << std::endl;
		}

		Mat lerpedSDF(height, width, CV_8UC3, Scalar(0, 0, 0));

		for (int i = 0; i < 255; i++) 
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++) {
					
					if (nextTexIndex >= int(grayImages.size())) {
						break;
					}
					float weight = lerpStep / levelStep;
					int curPixel = grayImages[curTexIndex].at<uchar>(y, x);
					int nextPixel = grayImages[nextTexIndex].at<uchar>(y, x);
					int lerpPixel = nextPixel * weight + curPixel * (1 - weight);
					result = lerpPixel < 127 ? 255 : 0;

					lerpedSDF.at<Vec3b>(y, x)[0] = int(result);
					lerpedSDF.at<Vec3b>(y, x)[1] = int(result);
					lerpedSDF.at<Vec3b>(y, x)[2] = int(result);
					
				}

			}
			std::string imageName = "SDF_" + std::to_string(i) + ".png";
			if (imwrite(outputFolder + imageName, lerpedSDF)) {
				std::cout << "Image Save Successfully:" << outputFolder + imageName << std::endl;
			}
			else {
				std::cout << "Image Save Failed" << std::endl;
			}

			lerpStep++;
			if (lerpStep >= levelStep)
			{
				lerpStep = 0;
				curTexIndex++;
				nextTexIndex++;

			}
		}

		result = 0;
		curTexIndex = 0;
		nextTexIndex = 1;
		lerpStep = 0;


		
	}

	return;
}

void SDFLerp2(std::string folderPath) {
	using namespace cv;

	std::cout << "SDFLerp,Path: " << folderPath << std::endl;
	std::vector<cv::String> fileNames;
	glob(folderPath, fileNames);
	//std::cout << "fileNames: " << fileNames[0] << std::endl;
	if (fileNames.empty()) {
		std::cout << "No image in folder." << std::endl;
		return;
	}

	std::vector<cv::Mat> grayImages;

	for (const cv::String& fileName : fileNames) {
		cv::Mat image = cv::imread(fileName);
		if (image.empty()) {
			std::cerr << "Canot load image: " << fileName << std::endl;
		}
		else {
			grayImages.push_back(Color2Gray(image)); // 将加载的图像添加到向量中
		}
	}

	float result = 0;
	if (grayImages.size() > 1) {
		// 创建一个空白图像

		int levelStep = int(256.0 / (int(grayImages.size()) - 1));
		float lerpStep = 0;
		int curTexIndex = 0;
		int nextTexIndex = 1;

		int width = grayImages[0].cols;
		int height = grayImages[0].rows;

		Mat lerpedSDF(height, width, CV_8UC3, Scalar(0, 0, 0));


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++) {
				for (int i = 0; i < 255; i++) {
					if (nextTexIndex >= int(grayImages.size())) {
						break;
					}
					float weight = lerpStep / levelStep;
					int curPixel = grayImages[curTexIndex].at<uchar>(y, x);
					int nextPixel = grayImages[nextTexIndex].at<uchar>(y, x);
					int lerpPixel = curPixel * weight + nextPixel * (1 - weight);
					result += lerpPixel > 127 ? 0 : 1;

					lerpStep++;
					if (lerpStep >= levelStep)
					{
						lerpStep = 0;
						curTexIndex++;
						nextTexIndex++;

					}
				}

				//result /= 255.0;
				//result = 255 - result;
				lerpedSDF.at<Vec3b>(y, x)[0] = int(result);
				lerpedSDF.at<Vec3b>(y, x)[1] = int(result);
				lerpedSDF.at<Vec3b>(y, x)[2] = int(result);

				result = 0;
				curTexIndex = 0;
				nextTexIndex = 1;
				lerpStep = 0;
			}
		}

		// 指定保存文件夹路径和文件名
		std::string outputFolder = folderPath + "SDF/";
		if (std::filesystem::create_directory(outputFolder)) {
			std::cout << "Create folder:" << outputFolder << std::endl;
		}

		if (imwrite(outputFolder + "SDF.png", lerpedSDF)) {
			std::cout << "Image Save Successfully:" << outputFolder + "SDF.png" << std::endl;
		}
		else {
			std::cout << "Image Save Failed" << std::endl;
		}
		return;
	}

	return;
}

//int main() {
//	SDFLerp("E:/creative/project/ToonRender/Python/Test/Texture/5/output/");
//}

PYBIND11_MODULE(SDF_Cpp, m) {
	m.doc() = "pybind11 example plugin"; // optional module docstring

	m.def("GenerateSDF", &GenerateSDF, "输入阈值图的路径，生成SDF图，并返回输出文件夹的绝对路径");
	m.def("SDFLerp", &SDFLerp2, "插值SDF图");
}
