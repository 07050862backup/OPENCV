#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <time.h>

using namespace std;
using namespace cv;
using namespace ml;
using namespace cv::dnn;

const String keys =
"{help h usage ? |         | print this message   }"
"{@algo          |0        | 0: KNN, 1:SVM, 2:VGG16        }"
"{@dataset       |0        | 0: hwd-mnist, 1:fashion-mnist }"
"{@param         |0        | 0: KNN(k=3) or SVM(POLY), 1:KNN(k=5) or SVM(RBF) }"
;

string knnModelName = "mnist/knn_knowledge.yml";
string svmModelName = "mnist/svm_knowledge.yml";
string trainImagesFile;
string trainLabelsFile;
string testImagesFile;
string testLabelsFile;
int param = 0;

Mat train_images;
Mat train_labels;
Mat test_images;
Mat test_labels;

clock_t start_train, end_train;
clock_t start_test, end_test;
double train_time, test_time;

//把大端資料轉換為我們常用的小端資料  
uint32_t swap_endian(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

Mat readImages(int opt) {
	int idx = 0;
	ifstream file;
	Mat img;
	if (opt == 0)
	{
		cout << " Training...";
		file.open(trainImagesFile, ios::in | ios::binary);
	}
	else
	{
		cout << " Test...";
		file.open(testImagesFile, ios::in | ios::binary);
	}
	// check file
	if (!file.is_open())
	{
		cout << " File Not Found!";
		return img;
	}
	/*
	byte 0 - 3 : Magic Number(Not to be used)
	byte 4 - 7 : Total number of images in the dataset
	byte 8 - 11 : rows of each image in the dataset
	byte 12 - 15 : cols of each image in the dataset
	*/
	uint32_t magic_number = 0;
	uint32_t number_of_images = 0;
	uint32_t height = 0;
	uint32_t width = 0;
	//讀魔術數
	file.read((char*)&magic_number, sizeof(magic_number));
	magic_number = swap_endian(magic_number);
	if (magic_number != 2051)
	{
		cout << "this is not the mnist image file" << endl;
		return img;
	}
	//讀影像數  
	file.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = swap_endian(number_of_images);
	//讀影像列數、行數  
	file.read((char*)&height, sizeof(height));
	height = swap_endian(height);
	file.read((char*)&width, sizeof(width));
	width = swap_endian(width);
	//讀取影像  
	Mat train_images = Mat(number_of_images, height * width, CV_8UC1);
	cout << " No. of images:" << number_of_images << endl;
	Mat digitImg = Mat::zeros(height, width, CV_8UC1);
	for (int i = 0; i < number_of_images; i++) {
		int index = 0;
		for (int r = 0; r < height; ++r) {
			for (int c = 0; c < width; ++c) {
				unsigned char temp = 0;
				file.read((char*)&temp, sizeof(temp));
				index = r * width + c;
				train_images.at<uchar>(i, index) = (int)temp;
				digitImg.at<uchar>(r, c) = (int)temp;
			}
		}
		//if (i < 100) {
		//    imwrite(format("D:/vcprojects/images/mnist/images/digit_%d.png", i), digitImg);
		//}
	}
	train_images.convertTo(train_images, CV_32FC1);
	return train_images;
}

Mat readLabels(int opt) {
	int idx = 0;
	ifstream file;
	Mat img;
	if (opt == 0)
	{
		cout << " Training...";
		file.open(trainLabelsFile);
	}
	else
	{
		cout << " Test...";
		file.open(testLabelsFile);
	}
	// check file
	if (!file.is_open())
	{
		cout << " File Not Found!";
		return img;
	}
	/*
	byte 0 - 3 : Magic Number(Not to be used)
	byte 4 - 7 : Total number of labels in the dataset
	*/
	uint32_t magic_number = 0;
	uint32_t number_of_labels = 0;
	//讀魔術數
	file.read((char*)&magic_number, sizeof(magic_number));
	magic_number = swap_endian(magic_number);
	if (magic_number != 2049)
	{
		cout << "this is not the mnist label file" << endl;
		return img;
	}
	//讀標籤數  
	file.read((char*)&number_of_labels, sizeof(number_of_labels));
	number_of_labels = swap_endian(number_of_labels);
	cout << " No. of labels:" << number_of_labels << endl;
	//讀取標籤  
	Mat labels = Mat(number_of_labels, 1, CV_8UC1);
	for (long int i = 0; i < number_of_labels; ++i)
	{
		unsigned char temp = 0;
		file.read((char*)&temp, sizeof(temp));
		//printf("temp : %d ", temp);
		labels.at<uchar>(i, 0) = temp;
	}
	labels.convertTo(labels, CV_32SC1);
	return labels;
}

void knnTrain() {

	train_images = train_images / 255.0;
	Ptr<ml::KNearest> knn = ml::KNearest::create();

	int K = (param == 0 ? 3 : 5);
	cout << "KNN: K=" << K << endl;
	knn->setDefaultK(K);
	knn->setIsClassifier(true);
	Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
	knn->train(tdata);
	knn->save(knnModelName);
}

void svmTrain()
{
	train_images = train_images / 255.0;
	// 創建與初始化
	Ptr<cv::ml::SVM> svm = ml::SVM::create();
	if (param == 0) {
		// 設定1
		svm->setType(ml::SVM::C_SVC);
		svm->setKernel(ml::SVM::POLY);
		svm->setDegree(3);
		svm->setGamma(3);
	}
	else {
		svm->setType(ml::SVM::C_SVC);
		svm->setKernel(ml::SVM::RBF);
		svm->setC(0.1);
		svm->setGamma(0.03375);
	}
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 200, FLT_EPSILON));

	// SVM訓練mnist數據集分類
	svm->train(train_images, ml::ROW_SAMPLE, train_labels);
	if (param == 0) {
		printf("SVM: POLY, Deg=%d, Gammma=%f\n", svm->getDegree(), svm->getGamma());
	}
	else {
		printf("SVM: RBF, C=%.2f, Gammma=%f\n", svm->getC(), svm->getGamma());
	}
	svm->save(svmModelName);
}

void testMnistKNN() {
	Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>(knnModelName);
	start_test = clock();
	float total = test_images.rows;
	float correct = 0;
	Rect rect;
	rect.x = 0;
	rect.height = 1;
	rect.width = (28 * 28);
	for (int i = 0; i < total; i++) {
		int actual = test_labels.at<int>(i);
		rect.y = i;
		Mat oneImage = test_images(rect);
		oneImage = oneImage / 255.0;
		Mat result;
		float predicted = knn->predict(oneImage, result);
		int digit = static_cast<int>(predicted);
		if (digit == actual) {
			correct++;
		}
	}
	end_test = clock();
	printf(" KNN recognize rate : %.2f%% ", correct / total * 100.0);
}

void testMnistSVM() {
	Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>(svmModelName);
	start_test = clock();
	float total = test_images.rows;
	float correct = 0;
	Rect rect;
	rect.x = 0;
	rect.height = 1;
	rect.width = (28 * 28);
	for (int i = 0; i < total; i++) {
		int actual = test_labels.at<int>(i);
		rect.y = i;
		Mat oneImage = test_images(rect);
		oneImage = oneImage / 255.0;
		float predicted = svm->predict(oneImage);
		if ((int)predicted == actual) {
			correct++;
		}
	}
	end_test = clock();
	printf(" SVM recognize rate : %.2f%% ", correct / total * 100.0);
}
void testTFVGG16()
{
	String modelBin = "models/colab_complex_frozen_graph_fashion_mnist_vgg16_e10_b64_adam.pb";  
	Net net = dnn::readNetFromTensorflow(modelBin);
	if (net.empty())
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		exit(-1);
	}

	start_test = clock();
	float total = test_images.rows;
	float correct = 0;
	Rect rect;
	rect.x = 0;
	rect.height = 1;
	rect.width = (28 * 28);
	for (int i = 0; i < total; i++) {
		int actual = test_labels.at<int>(i);
		rect.y = i;
		Mat oneImage = test_images(rect);

		Mat img = oneImage.clone();
		img = img.reshape(1, 28);
		cvtColor(img, img, COLOR_GRAY2BGR);
		Mat inputBlob = blobFromImage(img, 1.0/255, Size(48, 48), Scalar(0, 0, 0), false);

		net.setInput(inputBlob);
		Mat prob = net.forward();
		Point classIdPoint;
		double confidence;

		minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
		int predicted = classIdPoint.x;
		if ((int)predicted == actual) {
			correct++;
		}
		if (i % 1000 == 0)
			cout << "correct=" << correct << ", total=" << i + 1 << endl;
	}
	end_test = clock();
	printf(" VGG16 recognize rate : %.2f%% ", correct / total * 100.0);
}

int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}
	if (parser.get<int>(1) == 0) { // hwd-mnist dataset
		trainImagesFile = "MNIST_data/hwd-mnist/train-images.idx3-ubyte";
		trainLabelsFile = "MNIST_data/hwd-mnist/train-labels.idx1-ubyte";
		testImagesFile = "MNIST_data/hwd-mnist/t10k-images.idx3-ubyte";
		testLabelsFile = "MNIST_data/hwd-mnist/t10k-labels.idx1-ubyte";
		cout << "hwd-mnist dataset" << endl;
	}
	else {   // fashion-mnist dataset
		trainImagesFile = "MNIST_data/fashion-mnist/train-images-idx3-ubyte";
		trainLabelsFile = "MNIST_data/fashion-mnist/train-labels-idx1-ubyte";
		testImagesFile = "MNIST_data/fashion-mnist/t10k-images-idx3-ubyte";
		testLabelsFile = "MNIST_data/fashion-mnist/t10k-labels-idx1-ubyte";
		cout << "fashion-mnist dataset" << endl;
	}

	param = parser.get<int>(2);

	train_images = readImages(0);
	train_labels = readLabels(0);
	printf(" read mnist train dataset successfully...\n");

	test_images = readImages(1);
	test_labels = readLabels(1);
	printf(" read mnist test dataset successfully...\n");

	start_train = clock();
	switch(parser.get<int>(0)){
	case 0:
		knnTrain();
		break;
	case 1:
		svmTrain();
		break;
	}
	end_train = clock();
	train_time = ((double)end_train - (double)start_train) / CLOCKS_PER_SEC;
	printf("training time = %.2f sec\n\n", train_time);

	switch (parser.get<int>(0)) {
	case 0:
		testMnistKNN();
		break;
	case 1:
		testMnistSVM();
		break;
	case 2:
		testTFVGG16();
		break;
	}
	test_time = ((double)end_test - (double)start_test) / CLOCKS_PER_SEC;
	printf("testing time = %.2f sec\n\n", test_time);

	return 0;
}

