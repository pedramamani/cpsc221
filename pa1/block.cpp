#include "block.h"

// From im, grabs the vertical strip of pixels whose upper left corner
// is at position (column,0) and whose width is width.
void Block::build(PNG & im, int column, int width) {
	int height = (int) im.height();
	vector<HSLAPixel> vert;

	for (int i = column; i < column + width; i++) {
		for (int j = 0; j < height; j++)
			vert.push_back(*im.getPixel(i, j));
		data.push_back(vert);
		vert.clear();
	}
}

// Draws the current block at position (column,0) in im.
void Block::render(PNG & im, int column) const {
   for (int i = column; i < column + width(); i++)
	   for (int j = 0; j < height(); j++)
		   *im.getPixel(i, j) = data[i-column][j];
}

// Changes the saturation of every pixel in the block to 0, 
// removing the color and leaving grey.
void Block::greyscale() {
	for (int i = 0; i < width(); i++)
		for (int j = 0; j < height(); j++)
			data[i][j].s = 0;
}

// Returns the width of the current block.
int Block::width() const {
	return data.size();
}

// Returns the height of the current block.
int Block::height() const {
	if (!data.empty())
		return data[0].size();
	else
		return 0;
}
