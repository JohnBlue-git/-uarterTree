/*
Auther: John Blue
Time: 2022/1
Platform: ATOM with MinGw and g++compiler
Object: Quadtree
本次作業要試建立四元樹之資料結構，將附件(Point_HW2.txt)之點資料讀入程式中，並建立四元
搜尋樹，並輸入點座標，透過四元樹搜尋離該點最近的點。如圖一所示，將平面分割為四等分，若
區域內有一個以上的點，則繼續將該區域分為四等分，直到所有區域只有一個點為止。
Version:
	v1: tree as the quarter node
	v2: tree control the quarter node
*/

#include <iostream>
#include <iomanip>

////// Point.h

class Point
{
private:
	// var
	float x;
	float y;
public:
	// constructor
	Point(): x(0), y(0) {}
	Point(float a, float b): x(a), y(b) {}
	Point(const Point& copy);
	~Point();
	// function
	void Set_data(float a, float b) { x = a; y = b; }// reset data
	// operation
	float operator[](int i) const;// index operator for x,y
	Point& operator=(const Point& p);// assignment operator
	Point operator+(const Point& p) const;// addition operator
	// friend function
	friend std::istream& operator>>(std::istream& os, Point& t);// create new Point
	friend std::ostream& operator<<(std::ostream& os, const Point& t);// show Point
};

////// Point.cpp

Point::Point(const Point& copy) {
	x = copy.x;
	y = copy.y;
}

Point::~Point() {
	x = 0;
	y = 0;
}

float Point::operator[](int i) const {
	if (i == 0) {
		return x;
	}
	else if (i == 1) {
		return y;
	}
	else {
		std::cout << "Index out of list" << "\n" << std::endl;
		return - 1;
	}
}

Point& Point::operator=(const Point& p) {
	// check
	if (this == &p) {
		return *this;
	}
	// assign
	x = p.x;
	y = p.y;
	// return
	return *this;
}

Point Point::operator+(const Point& p) const {
	// point to return
	Point sum = Point();
	// addiction
	sum.x = x + p.x;
	sum.y = y + p.y;
	// return
	return sum;
}

std::istream& operator>>(std::istream& os, Point& t) {
	os >> t.x >> t.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Point& t) {
	os << std::fixed << std::setprecision(3) << " (x,y) =  (" << t.x << "," << t.y << ")";
	return os;
}

////// QuadtreeNode.h

class QuadtreeNode
{
private:
	// var
	const Point* data;
	QuadtreeNode* nextNode[4];// the order of Node as [0,1,2,3] = [++,-+,--,++] */
	const Point seperate_point;
	const float size;
public:
	// constructor
	//QuadtreeNode();
	QuadtreeNode(const Point* a, const Point& b, float c);
	QuadtreeNode(const QuadtreeNode& q);
	~QuadtreeNode();
	// function
	unsigned int which(const QuadtreeNode* pt, const Point& p) const;// finding which tree the point belong
	const Point sep_point(const QuadtreeNode* pt, unsigned int& i) const;// creating seperation point for new tree
	bool InsertPoint(const Point* p);// inserting one by one
	const Point& FindClosestPoint(const Point& p) const;// finding which tree the user define points belong
	void show() const;// show the whole tree
};

////// QuadtreeNode.cpp

QuadtreeNode::QuadtreeNode(const Point* a, const Point& b, float c) : data(a), seperate_point(b), size(c)
{// :data(&a) to assign value to tree structaul, :data(&a) means data = &a, even data is const work
	// data, separation size have been assigned
	// next default as NULL
	for (unsigned int i = 0; i < 4; i++) {
		nextNode[i] = 0;
	}
}

QuadtreeNode::QuadtreeNode(const QuadtreeNode& Q) : data(Q.data), seperate_point(Q.seperate_point), size(Q.size) {
	for (unsigned int i = 0; i < 4; i++) {
		if (Q.nextNode[i] != 0) {
			nextNode[i] = new QuadtreeNode(*Q.nextNode[i]);
		}
		else if (Q.nextNode[i] == 0) {
			continue;
		}
	}
}

QuadtreeNode::~QuadtreeNode() {
	data = 0;
	seperate_point.Point::~Point();
	for (unsigned int i = 0; i < 4; i++) {
		if (nextNode[i] != 0) {
			(*nextNode[i]).QuadtreeNode::~QuadtreeNode();
			nextNode[i] = 0;
		}
	}
}

unsigned int QuadtreeNode::which(const QuadtreeNode* pt, const Point& p) const {
	// return unsigned int if [0,1,2,3] = [++,-+,--,++]
	if (p[0] >= pt->seperate_point[0]) {
		if (p[1] >= pt->seperate_point[1]) {
			return (unsigned int)0;
		}
		else if (p[1] < seperate_point[1]) {
			return (unsigned int)3;
		}
	}
	else if (p[0] < seperate_point[0]) {
		if (p[1] >= seperate_point[1]) {
			return (unsigned int)1;
		}
		else if (p[1] < seperate_point[1]) {
			return (unsigned int)2;
		}
	}
}

const Point  QuadtreeNode::sep_point(const QuadtreeNode* pt, unsigned int& i) const {
	// create new Point
	Point sep = Point();
	// assgin Point accodingly, [ 50,50 , -50,50 , -50,-50 , 50,-50 ] = [0,1,2,3]
	if (i == 0) {
		sep = pt->seperate_point + Point(pt->size / 2, pt->size / 2);
	}
	else if (i == 1) {
		sep = pt->seperate_point + Point(-pt->size / 2, pt->size / 2);
	}
	else if (i == 2) {
		sep = pt->seperate_point + Point(-pt->size / 2, -pt->size / 2);
	}
	else if (i == 3) {
		sep = pt->seperate_point + Point(pt->size / 2, -pt->size / 2);
	}
	return sep;
}

bool QuadtreeNode::InsertPoint(const Point* p) {
	// start from its root
	QuadtreeNode* pt = this;
	// if root have data, which means its a tree node, we need to further divide this region to seperate 2 points
	if (pt->data != 0) {
		// delete pt->data and let the data assigned to a
		const Point* tmp_root = pt->data;
		pt->data = 0;
		// insert root data
		unsigned int i = QuadtreeNode::which(pt, *tmp_root);
		const Point sep_i = sep_point(pt, i);
		pt->nextNode[i] = new QuadtreeNode(tmp_root, sep_i, pt->size / 2);
		// insert leaf data
		unsigned int j = QuadtreeNode::which(pt, *p);
		if (i != j) {
			const Point sep_j = sep_point(pt, j);
			pt->nextNode[j] = new QuadtreeNode(p, sep_j, pt->size / 2);
		}
		else if (i == j) {
			pt->nextNode[i]->InsertPoint(p);
		}
	}
	// if root don't have data, which means( it's father root for some tree )or( tree of root not yet assigned )
	else if (pt->data == 0) {
		// determine which tree p belong
		unsigned int i = QuadtreeNode::which(pt, *p);
		// insert
		if (pt->nextNode[i] == 0) {
			const Point sep = sep_point(pt, i);
			pt->nextNode[i] = new QuadtreeNode(p, sep, pt->size / 2);
		}
		else {
			pt->nextNode[i]->InsertPoint(p);
		}
	}
	else {
		return false;
	}
	return true;
}

const Point& QuadtreeNode::FindClosestPoint(const Point& p) const {
	// start from its root
	const QuadtreeNode* pt = this;
	// if data exist, which means it's a tree; we can print it out
	if (pt->data != 0) {
		std::cout << "The closest point is " << *(pt->data) << std::endl;
		std::cout << "\n" << std::endl;
		return *(pt->data);
	}
	// other
	else if (pt->data == 0) {
		// if it has next, then go to the next, call ... recursively
		unsigned int i = QuadtreeNode::which(pt, p);
		if (pt->nextNode[i] != 0) {
			pt = pt->nextNode[i];
			pt->FindClosestPoint(p);
		}
		// if it doesn't has next, which means this node don't have points before
		else if (pt->nextNode[i] == 0) {
			std::cout << "There is no near point" << std::endl;
			std::cout << "\n" << std::endl;
			return *(Point*)0;
		}
	}
}

void QuadtreeNode::show() const {
	// start from its root
	const QuadtreeNode* pt = this;
	// if it's the tree, print it out
	if (pt->data != 0) {
		std::cout << *(pt->data) << std::endl;
	}
	// if it's a father, then go to search the tree one by one
	else if (pt->data == 0) {
		// remember the current root
		const QuadtreeNode* headi = pt;
		// find all the four tree
		for (unsigned int i = 0; i < 4; i++) {
			// if is tree leaf, then point to next
			if (pt->nextNode[i] != 0) {
				// point to next
				pt = pt->nextNode[i];
				// call function itself
				pt->show();
				// remember the root since pt->show will lost its root address, and keep going
				pt = headi;
			}
			// if there is no tree leaf, then skip it
			else if (pt->nextNode[i] == 0) {
				continue;
			}
		}
	}
}
