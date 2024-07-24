#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

template <class T>
class MatNode
{

public:
	T data;
	MatNode<T>* next;

	MatNode()
	{
		data = 0;
		next = NULL;
	}

	MatNode(T d, MatNode* n)
	{
		data = d;
		next = n;
	}

	MatNode(T d)
	{
		data = d;
		next = NULL;
	}
};

template <class T>
class SLinkedList {

public:

	MatNode<T>* head;


	SLinkedList()
	{
		head = NULL;
	}

	void insert(T d) {
		MatNode <T>* n = new MatNode<T>;
		n->data = d;
		n->next = NULL;
		if (head == NULL)
		{
			head = n;
		}
		else
		{
			MatNode <T>* temp = new MatNode<T>;
			temp = head;
			while (temp->next)
			{
				temp = (temp->next);
			}
			temp->next = n;
		}
	}
	void print()
	{
		MatNode<T>* temp;
		temp = head;
		while (temp)
		{
			cout << temp->data << " ";
			if (temp->data == -1)
				cout << endl;
			temp = temp->next;
		}
		cout << endl;
	}
};

int main()
{
	//string path("C:/Assignment 1 files/mIMD063.BMP");
	//string Kpath("C:/Assignment 1 files/IMD063.BMP");
	//string Gpath("C:/Assignment 1 files/IMD063_lesion.BMP");
	string path("C:\\Assignment 1 files\\Segmented Outputs\\mIMD040.BMP");
	string Kpath("C:\\Assignment 1 files\\Original Images\\IMD040.BMP");
	string Gpath("C:\\Assignment 1 files\\Ground Truths\\IMD040_lesion.BMP");
	Mat img = imread(path);
	
	float current = 0;
	float rows = img.rows;
	float cols = img.cols;
	float** imgmat = new float* [rows];
	for (int i = 0; i < rows; i++)
	{
		imgmat[i] = new float[cols] {0};
	}

	/////////////////////////task 1 start///////////////////////////
	float x = 0, y = 0, z = 0;
	bool lcheck = false;
	float changelab = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			x = img.at<Vec3b>(i, j)[0];
			y = img.at<Vec3b>(i, j)[1];
			z = img.at<Vec3b>(i, j)[2];
			imgmat[i][j] = (x + y + z) / 3;
		}
	}
	int label = 1;
	int neighbours = 0;
	bool foundNeighbour = false;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (imgmat[r][c] != 0)
			{
				neighbours = 0;
				foundNeighbour = false;
				if (c != 0)
				{
					if (imgmat[r][c - 1] != 0)
						neighbours++;
				}

				if (r != 0)
				{
					if (imgmat[r - 1][c] != 0)
						neighbours++;
					if (c != 0)
					{
						if (imgmat[r - 1][c - 1] != 0)
							neighbours++;
					}
				}

				if ((r != 0) && (c != cols - 1))
				{
					if (imgmat[r - 1][c + 1] != 0)
						neighbours++;
				}

				if (neighbours == 0)
				{
					imgmat[r][c] = label;
					label++;
				}

				else if (neighbours == 1)
				{
					if (c != 0)
					{
						if (imgmat[r][c - 1] != 0)
							imgmat[r][c] = imgmat[r][c - 1];
					}

					if (r != 0)
					{
						if (imgmat[r - 1][c] != 0)
							imgmat[r][c] = imgmat[r - 1][c];

						if (c != 0)
						{
							if (imgmat[r - 1][c - 1] != 0)
								imgmat[r][c] = imgmat[r - 1][c - 1];
						}

						if (c != cols - 1)
						{
							if (imgmat[r - 1][c + 1] != 0)
								imgmat[r][c] = imgmat[r - 1][c + 1];
						}
					}
				}

				else if (neighbours > 1)
				{
					if (c != 0)
					{
						if (imgmat[r][c - 1] != 0)
						{
							imgmat[r][c] = imgmat[r][c - 1];
						}
					}

					if (r != 0)
					{
						if ((imgmat[r][c] == 255) && (imgmat[r - 1][c] != 0))
							imgmat[r][c] = imgmat[r - 1][c];
						else if ((imgmat[r][c] != 0) && (imgmat[r][c] != 255))
						{
							if ((imgmat[r - 1][c] != 0) && (imgmat[r - 1][c] != imgmat[r][c]))
							{
								changelab = imgmat[r - 1][c];
								for (int i = 0; i < rows; i++)
								{
									for (int j = 0; j < cols; j++)
									{
										if (imgmat[i][j] == changelab)
											imgmat[i][j] = imgmat[r][c];
									}
								}
							}
						}
						if (c != 0)
						{
							if ((imgmat[r][c] == 255) && (imgmat[r - 1][c - 1] != 0))
								imgmat[r][c] = imgmat[r - 1][c - 1];
							else if ((imgmat[r][c] != 0) && (imgmat[r][c] != 255))
							{
								if ((imgmat[r - 1][c - 1] != 0) && (imgmat[r - 1][c - 1] != imgmat[r][c]))
								{
									changelab = imgmat[r - 1][c - 1];
									for (int i = 0; i < rows; i++)
									{
										for (int j = 0; j < cols; j++)
										{
											if (imgmat[i][j] == changelab)
												imgmat[i][j] = imgmat[r][c];
										}
									}
								}
							}
						
						}

						if (c != cols - 1)
						{
							if ((imgmat[r][c] == 255) && (imgmat[r - 1][c + 1] != 0))
								imgmat[r][c] = imgmat[r - 1][c + 1];
							else if ((imgmat[r][c] != 0) && (imgmat[r][c] != 255))
							{
								if ((imgmat[r - 1][c + 1] != 0) && (imgmat[r - 1][c + 1] != imgmat[r][c]))
								{
									changelab = imgmat[r - 1][c + 1];
									for (int i = 0; i < rows; i++)
									{
										for (int j = 0; j < cols; j++)
										{
											if (imgmat[i][j] == changelab)
												imgmat[i][j] = imgmat[r][c];
										}
									}
								}
							}
						}

					}
				}
			}
		}
	}

	int* LabelCount = new int[label] {0};

	for (int l = 0; l < label - 1; l++)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (imgmat[i][j] == l + 1)
					LabelCount[l]++;
			}
		}
	}
	int majcount = LabelCount[0];
	int majLabel = 1;
	int* anotherLabelArray = new int[label];
	for (int i = 0; i < label - 1; i++)
	{
		if (LabelCount[i] > majcount)
		{
			majcount = LabelCount[i];
			majLabel = i + 1;
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (imgmat[i][j] == majLabel)
			{
				imgmat[i][j] = 255;
				img.at<Vec3b>(i, j)[0] = 255;
				img.at<Vec3b>(i, j)[1] = 255;
				img.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				imgmat[i][j] = 0;
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}

	/*imshow("Sahi output de do please", img);
	waitKey(0);*/
	/////////////////////////task 1 end///////////////////////////
	 


	/////////////////////////task 2 start/////////////////////////
	Mat kimg = imread(Kpath);
	imshow("Original Images", kimg);
	moveWindow("First OpenCV Application", 0, 45);
	waitKey(0);

	float krows = kimg.rows;
	float kcols = kimg.cols;
	float** kimgmat = new float* [krows];
	for (int i = 0; i < krows; i++)
	{
		kimgmat[i] = new float[kcols] {0};
	}

	for (int i = 0; i < krows; i++)
	{
		for (int j = 0; j < kcols; j++)
		{
			x = kimg.at<Vec3b>(i, j)[0];
			y = kimg.at<Vec3b>(i, j)[1];
			z = kimg.at<Vec3b>(i, j)[2];
			kimgmat[i][j] = (x + y + z) / 3;
		}
	}
	bool check = true;
	float center1 = kimgmat[0][0];
	float center2 = kimgmat[0][0];
	int var1 = 2, var2 = 4;
	while (center1 == center2)
	{
		center2 = kimgmat[var1][var2];
		var1 += 2;
		var2 += 3;
	}
	float** kmean = new float* [krows];
	for (int i = 0; i < krows; i++)
		kmean[i] = new float[kcols] {0};
	float sub1 = 0, sub2 = 0, sum1 = 0, sum2 = 0, total1 = 0, total2 = 0, avg1 = 0, avg2 = 0;

	while (check) {
		for (int r = 0; r < krows; r++)
		{
			for (int c = 0; c < kcols; c++)
			{
				sub1 = center1 - kimgmat[r][c];
				if (sub1 < 0)
					sub1 = sub1 * -1;
				sub2 = center2 - kimgmat[r][c];
				if (sub2 < 0)
					sub2 = sub2 * -1;
				if (sub1 < sub2)
				{
					kmean[r][c] = 1;
					sum1 += kimgmat[r][c];
					total1++;
				}
				if (sub2 < sub1)
				{
					kmean[r][c] = 2;
					sum2 += kimgmat[r][c];
					total2++;
				}
			}
		}
		avg1 = sum1 / total1;
		avg2 = sum2 / total2;
		if ((center1 == avg1) && (center2 == avg2))
			check = false;
		else
		{
			center1 = avg1;
			center2 = avg2;
			sum1 = 0;
			sum2 = 0;
			total1 = 0;
			total2 = 0;
		}
	}
	if (avg1 > avg2)
	{

		for (int r = 0; r < krows; r++)
			for (int c = 0; c < kcols; c++)
			{
				if (kmean[r][c] == 1)
					kimgmat[r][c] = 0;
				if (kmean[r][c] == 2)
					kimgmat[r][c] = 255;
			}
	}

	if (avg2 > avg1)
	{

		for (int r = 0; r < krows; r++)
			for (int c = 0; c < kcols; c++)
			{
				if (kmean[r][c] == 1)
					kimgmat[r][c] = 255;
				if (kmean[r][c] == 2)
					kimgmat[r][c] = 0;
			}
	}

	for (int i = 0; i < krows; i++)
	{
		for (int j = 0; j < kcols; j++)
		{
			kimg.at<Vec3b>(i, j)[0] = kimgmat[i][j];
			kimg.at<Vec3b>(i, j)[1] = kimgmat[i][j];
			kimg.at<Vec3b>(i, j)[2] = kimgmat[i][j];
		}
	}

	imshow("Task 2", kimg);
	moveWindow("First OpenCV Application", 0, 45);
	waitKey(0);
	/////////////////////////task 2 end///////////////////////////
	
	//namedWindow("First OpenCV Application", WINDOW_AUTOSIZE);
	imshow("Task 1", img);
	moveWindow("First OpenCV Application", 0, 45);
	waitKey(0);
	destroyAllWindows();
	/////////////////////////task 3 start/////////////////////////
	Mat Gimg = imread(Gpath);

	float Grows = Gimg.rows;
	float Gcols = Gimg.cols;
	float** Gimgmat = new float* [Grows];
	for (int i = 0; i < Grows; i++)
	{
		Gimgmat[i] = new float[Gcols] {0};
	}

	x = 0; y = 0; z = 0;

	for (int i = 0; i < Grows; i++)
	{
		for (int j = 0; j < Gcols; j++)
		{
			x = Gimg.at<Vec3b>(i, j)[0];
			y = Gimg.at<Vec3b>(i, j)[1];
			z = Gimg.at<Vec3b>(i, j)[2];
			Gimgmat[i][j] = (x + y + z) / 3;
		}
	}


	float tp = 0, fp = 0, fn = 0;

	for (int r = 0; r < Grows; r++)
		for (int c = 0; c < Gcols; c++)
		{
			if ((Gimgmat[r][c] == imgmat[r][c]) && (imgmat[r] [c] != 0))
				tp++;
			if ((Gimgmat[r][c] == 0) && (imgmat[r][c] != 0))
				fp++;
			if ((Gimgmat[r][c] != 0) && (imgmat[r][c] == 0))
				fn++;
		}
	float dice1 = 0;
	dice1 = (tp * 2) / (fn + (tp * 2) + fp);
	cout << "Dice coefficient for task1: " << dice1 << endl;

	tp = 0; fp = 0; fn = 0;

	for (int r = 0; r < Grows; r++)
		for (int c = 0; c < Gcols; c++)
		{
			if ((Gimgmat[r][c] == imgmat[r][c]) && (kimgmat[r][c] != 0))
				tp++;
			if ((Gimgmat[r][c] == 0) && (kimgmat[r][c] != 0))
				fp++;
			if ((Gimgmat[r][c] != 0) && (kimgmat[r][c] == 0))
				fn++;
		}
	float dice2 = 0;
	dice2 = (tp * 2) / (fn + (tp * 2) + fp);
	cout << "Dice coefficient for task2: " << dice2 << endl << endl;
	/////////////////////////task 3 end///////////////////////////



	/////////////////////////task 4 start/////////////////////////
	cout << "Linked List output for task 4: " << endl << endl;
	
	SLinkedList<int> list;
	list.head = NULL;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (imgmat[r][c] != 0 && c == 0)
			{
				list.insert(c);
			}
			if (c != 0)
			{
				if ((imgmat[r][c - 1] == 0) && imgmat[r][c] != 0)
				{
					list.insert(c);
				}
			}
			if (c < cols - 1)
			{
				if ((imgmat[r][c + 1] == 0) && imgmat[r][c] != 0)
				{
					list.insert(c);
				}
			}
			if (c == cols - 1)
			{
				if ((imgmat[r][c] == 1))
				{
					list.insert(c);
				}
				list.insert(-1);
			}
		}
	}

	list.print();

	/////////////////////////task 4 end//////////////////////////

	return 0;
}