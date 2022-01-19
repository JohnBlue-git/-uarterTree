/*
Auther: John Blue
Time: 2022/1
Platform: ATOM with MinGw and g++compiler
Object: Test Quadtree
本次作業要試建立四元樹之資料結構，將附件(Point_HW2.txt)之點資料讀入程式中，並建立四元
搜尋樹，並輸入點座標，透過四元樹搜尋離該點最近的點。如圖一所示，將平面分割為四等分，若
區域內有一個以上的點，則繼續將該區域分為四等分，直到所有區域只有一個點為止。
Version:
	v1: tree as the quarter node
	v2: tree control the quarter node
*/

#include <iostream>// basic i/o
#include <iomanip>// fixed and setprecision
#include <sstream>// string
#include <fstream>// file i/o
#include <string>// string

#include "q_v2.h"

int main()
{
	////// get points data
	std::cout << "Open the file ...\n";

	// open file
	const char* file = "Point_HW2.txt";
	std::ifstream ifile(file);

	// search for information about how many points in the file
	unsigned int N = 0;// how many points
	std::string str;
	std::string s;
	std::getline(ifile, str);
	int found = str.find_first_of(":");
	s.append(str.begin() + found + 1, str.begin() + found + 3);
	std::cout << s << std::endl;
	N = std::stoi(s);
	// retrun to first line
	ifile.clear();
	ifile.seekg(0);

	// get data from the file to the points
	std::string line;
	Point* points = new Point[N];
	std::getline(ifile, line);// skip first line
	for (unsigned int i = 0; i < N; i++) {
		ifile >> points[i];// using Point operator>>
	}

	// close file
	ifile.close();

	////// inserting data
	std::cout << "Inserting file data to tree ..." << std::endl;

	// initial seperation point for tree
	Point zero = Point();

	// initial tree
	Quadtree* root = new Quadtree(&points[0], zero, (float)100);
	Quadtree* head = root;// address of the roor
	// inserting one by one
	for (unsigned int i = 1; i < N; i++) {
		root->InsertPoint(&points[i]);
	}

	// show the result
	std::cout << "After inserting all the points into root:" << std::endl;
	root->show();
	std::cout << "\n" << std::endl;

	////// closest point
	std::cout << "Please enter two float for create a new Point: range: (100,-100)\n";

	// user input
	Point user = Point();
	float a = 0;
	float b = 0;

	// cin and check
	bool ck = false;// for checking wether input is float or not
	while (!ck) {
		// ...
		//std::cin.sync();
		//std::cin.clear();
		// ask for a
		std::cout << "a:";
		// if something wrong then give a signal to quit the main function
		if (!(std::cin >> a)) {
			std::cin.sync(); std::cin.clear(); std::cin.ignore();
			std::cout << "That's not a float" << std::endl;
			ck = false;
			continue;
		}
		// ask for b
		std::cout << "b:";
		// if something wrong then give a signal to quit the main function
		if (!(std::cin >> b)) {
			std::cin.sync(); std::cin.clear(); std::cin.ignore();
			std::cout << "That's not a float" << std::endl;
			ck = false;
			continue;
		}
		// out of range?
		/*
		if ((std::abs(a) > 100) || (std::abs(b) > 100)) {
			std::cout << "Out of range" << std::endl;
			ck = false;
			continue;
		}
		*/
		// mark true
		ck = true;
	}

	// reset user
	user.Set_data(a, b);

	// find the closeset point
	std::cout << "FindClosestPoint ...\n";
	root->FindClosestPoint(user);
	std::cout << "\n" << std::endl;

	////// copy
	// copy
	std::cout << "After copy the root:" << std::endl;
	Quadtree* copy = new Quadtree(*root);
	// show
	copy->show();
	// find
	std::cout << "FindClosestPoint..." << std::endl;
	copy->FindClosestPoint(user);
	std::cout << "\n" << std::endl;
	// delete
	copy->~Quadtree();
	delete copy;

	////// delete tree
	delete root;// root->QuadtreeNode::~QuadtreeNode();
}
