// File:        gridlist.cpp
// Date:        2021-05-20 02:56

#include "block.h"
using namespace std;

// set values in data attribute, based on a dimension x dimension square region
//   with upper-left corner at (left, upper) in the input PNG image
// The orientation of the pixels in the data vector must match the orientation
//   of the pixels in the PNG.
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Build(PNG& im, int upper, int left, int dimension) {
    for (int i = upper; i <= (dimension + upper - 1); i++) {
        vector<HSLAPixel> row;

        for (int j = left; j <= (dimension+ left - 1); j++) {
            HSLAPixel pixel = *im.getPixel(j, i);
            row.push_back(pixel);
        }

        data.push_back(row);
    }
}

// write the pixel colour data fom data attribute into im,
//   with upper-left corner at (left, upper)
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Render(PNG& im, int upper, int left) const {
    for (int i = 0; i < Dimension(); i++) {
        for (int j = 0; j <  Dimension(); j++) {
            HSLAPixel * pixel = im.getPixel(i + left, j + upper);
            *pixel = data[j][i];
        }
    }
}

// Mirror this block's pixel data horizontally (along a vertical axis)
void Block::FlipHorizontal() {
    for (int i = 0; i < Dimension(); i++) {
        std::reverse(data[i].begin(), data[i].end());
    }
}

// Mirror this block's pixel data vertically (along a horizontal axis)
void Block::FlipVertical() {
    std::reverse(data.begin(), data.end());
}

// Return the horizontal (or vertical) size of the data block's image region
int Block::Dimension() const {
    return data.size();
}

// Returns the computed average luminance of the block
// Compute as the sum of each pixel's luminance value, divided by
//   the number of pixels in the block
double Block::AvgLuminance() const {
    double sum = 0;

    for (int i = 0; i <= Dimension() - 1; i++) {
        for (int j = 0; j <= Dimension() - 1; j++) {
            sum += data[i][j].l;
        }
    }

    if (Dimension() == 0) {
        return sum;
    } else {
        return sum / (Dimension()*Dimension());
    }
}
