
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}


toqutree::~toqutree(){
	clear(root);
}


toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}


toqutree::toqutree(PNG & im, int k) {
	int n = pow(2,k);
	pair<int, int> start(im.width()/2 - n/2, im.height()/2 - n/2);

	PNG subIm = subImage(im, start, k);
	root = buildTree(subIm, k);
}


int toqutree::size() {
	return recSize(root);
}


int toqutree::recSize(Node* sRoot) {
	if (sRoot == NULL)
		return 0;
	return 1 + recSize(sRoot->NW) + recSize(sRoot->NE) + recSize(sRoot->SE) + recSize(sRoot->SW);
}


toqutree::Node * toqutree::buildTree(PNG & im, int k) {
	if (k == 0)
		return new Node(pair<int, int>(0, 0), 0, *im.getPixel(0, 0));
	else if (k == 1) {
		stats* info = new stats(im);
		pair<int, int> ones(1, 1);
		pair<int, int> zeros(0, 0);

		Node* sRoot = new Node(ones, 1, info->getAvg(zeros, ones));
		delete(info);
		
		sRoot->SE = new Node(zeros, 0, *im.getPixel(1, 1));
		sRoot->SW = new Node(zeros, 0, *im.getPixel(0, 1));
		sRoot->NE = new Node(zeros, 0, *im.getPixel(1, 0));
		sRoot->NW = new Node(zeros, 0, *im.getPixel(0, 0));

		return sRoot;
	}

	int n1 = (int) pow(2, k-1);
	int n2 = (int) pow(2, k-2);
	stats* info = new stats(im);
	double minEntropy = avgEntropy(pair<int, int>(n2, n2), k, info);
	double entropy;
	pair<int, int> center(n2, n2);

	for (int x = n2; x < n2+n1; x++)
		for (int y = n2; y < n2+n1; y++) {
			entropy = avgEntropy(pair<int, int>(x, y), k, info);
			if (entropy < minEntropy) {
				minEntropy = entropy;
				center.first = x;
				center.second = y;
			}
		}

	Node* sRoot = new Node(center, k, info->getAvg(pair<int, int>(0, 0), pair<int, int>(2*n1-1, 2*n1-1)));
	delete(info);

	int x0 = center.first;
	int y0 = center.second;
	sRoot->NW = buildTree(subImage(im, pair<int, int>(x0+n1, y0+n1), k-1), k-1);
	sRoot->NE = buildTree(subImage(im, pair<int, int>(x0, y0+n1), k-1), k-1);
	sRoot->SW = buildTree(subImage(im, pair<int, int>(x0+n1, y0), k-1), k-1);
	sRoot->SE = buildTree(subImage(im, pair<int, int>(x0, y0), k-1), k-1);

	return sRoot;
}


double toqutree::avgEntropy(pair<int, int> center, int k, stats* info) {
	double sum = 0;
	int n1 = (int) pow(2, k);
	int n2 = (int) pow(2, k-1);
	int x0 = (center.first)%n1;
	int y0 = (center.second)%n1;

	// cout << "Calculating Average Entropy of " << n1 << "x" << n1 << endl;
	// cout << "entropySE" << info->entropy(pair<int, int>(x0, y0), pair<int, int>((x0+n2-1)%n1, (y0+n2-1)%n1)) << endl;
	// cout << "entropySW" << info->entropy(pair<int, int>((x0+n2)%n1, y0), pair<int, int>((x0-1)%n1, (y0+n2-1)%n1));
	// cout << "entropyNE" << info->entropy(pair<int, int>(x0, (y0+n2)%n1), pair<int, int>((x0+n2-1)%n1, (y0-1)%n1));
	// cout << "entropyNW" << info->entropy(pair<int, int>((x0+n2)%n1, (y0+n2)%n1), pair<int, int>((x0-1)%n1, (y0-1)%n1));

	sum += info->entropy(pair<int, int>(x0, y0), pair<int, int>((x0+n2-1)%n1, (y0+n2-1)%n1));
	sum += info->entropy(pair<int, int>((x0+n2)%n1, y0), pair<int, int>((x0-1)%n1, (y0+n2-1)%n1));
	sum += info->entropy(pair<int, int>(x0, (y0+n2)%n1), pair<int, int>((x0+n2-1)%n1, (y0-1)%n1));
	sum += info->entropy(pair<int, int>((x0+n2)%n1, (y0+n2)%n1), pair<int, int>((x0-1)%n1, (y0-1)%n1));
	return sum/4;
}


PNG & toqutree::subImage(PNG & im, pair<int, int> start, int k) {
	int n = (int) pow(2, k);
	PNG * subIm = new PNG(n,n);
	int x0 = start.first;
	int y0 = start.second;

	for (int y = 0; y < n; y++)
		for (int x = 0; x < n; x++)
			*subIm->getPixel(x, y) = *im.getPixel((x0+x)%(2*n), (y0+y)%(2*n));

	return *subIm;
}


PNG toqutree::render() {
	int n = pow(2, root->dimension);
	PNG im(n,n);
	vector<vector<HSLAPixel*>> pointers;
	vector<HSLAPixel*> col;

	for (int x = 0; x < n; x++) {
		col.clear();
		for (int y = 0; y < n; y++)
			col.push_back(im.getPixel(x,y));
		pointers.push_back(col);
	}

	recRender(root, pointers);
	return im;
}


void toqutree::recRender(Node* sRoot, vector<vector<HSLAPixel*>> & pointers) {

	if (sRoot->SE == NULL) {
		for (auto col : pointers) {
			for (auto pix : col)
				*pix = sRoot->avg;
		}
	}
	else {
		int n = pow(2, sRoot->dimension-1);
		int x0 = sRoot->center.first;
		int y0 = sRoot->center.second;
		vector<HSLAPixel*> col;

		vector<vector<HSLAPixel*>> pointersSE;
		for (int x = x0; x < x0+n; x++) {
			col.clear();
			for (int y = y0; y < y0+n; y++)
				col.push_back(pointers[x%(2*n)][y%(2*n)]);
			pointersSE.push_back(col);
		}
		recRender(sRoot->SE, pointersSE);
		pointersSE.clear();

		vector<vector<HSLAPixel*>> pointersSW;
		for (int x = x0+n; x < x0+2*n; x++) {
			col.clear();
			for (int y = y0; y < y0+n; y++)
				col.push_back(pointers[x%(2*n)][y%(2*n)]);
			pointersSW.push_back(col);
		}
		recRender(sRoot->SW, pointersSW);
		pointersSW.clear();

		vector<vector<HSLAPixel*>> pointersNW;
		for (int x = x0+n; x < x0+2*n; x++) {
			col.clear();
			for (int y = y0+n; y < y0+2*n; y++)
				col.push_back(pointers[x%(2*n)][y%(2*n)]);
			pointersNW.push_back(col);
		}
		recRender(sRoot->NW, pointersNW);
		pointersNW.clear();

		vector<vector<HSLAPixel*>> pointersNE;
		for (int x = x0; x < x0+n; x++) {
			col.clear();
			for (int y = y0+n; y < y0+2*n; y++)
				col.push_back(pointers[x%(2*n)][y%(2*n)]);
			pointersNE.push_back(col);
		}
		recRender(sRoot->NE, pointersNE);
		pointersNE.clear();
	}
}


void toqutree::prune(double tol) {
	pruneNode(root, tol);
}


void toqutree::pruneNode(Node * curNode, double tol){

	if (curNode->SE == NULL)
		return;
	else if (withinTol(curNode, curNode->avg, tol)) {
		clear(curNode->NW);
		clear(curNode->NE);
		clear(curNode->SE);
		clear(curNode->SW);
	}
	else {
		pruneNode(curNode->NW, tol);
		pruneNode(curNode->NE, tol);
		pruneNode(curNode->SE, tol);
		pruneNode(curNode->SW, tol);
	}
}


bool toqutree::withinTol (Node * curNode, HSLAPixel avg, double tol) {
	if (curNode->SE == NULL)
		return avg.dist(curNode->avg) <= tol;
	return withinTol(curNode->NW, avg, tol) && withinTol(curNode->NE, avg, tol) 
		&& withinTol(curNode->SE, avg, tol) && withinTol(curNode->SW, avg, tol);
}


void toqutree::clear(Node * & curr){
	if (curr != NULL) {
		clear(curr->NW);
		clear(curr->NE);
		clear(curr->SE);
		clear(curr->SW);

		delete curr;
		curr = NULL;
	}
}


toqutree::Node * toqutree::copy(const Node * other) {
	if (other != NULL) {
			Node* sRoot = new Node(other->center, other->dimension, other->avg);
			sRoot->NW = copy(other->NW);
			sRoot->NE = copy(other->NE);
			sRoot->SW = copy(other->SW);
			sRoot->SE = copy(other->SE);
			return sRoot;
	}
	return NULL;
}
