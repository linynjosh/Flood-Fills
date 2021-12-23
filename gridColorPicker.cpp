#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    /* Your code here! */
    border_color = gridColor;
    grid_spacing = gridSpacing;
}

HSLAPixel gridColorPicker::operator()(int x, int y)
{
    /* Your code here! */
    if (x % grid_spacing == 0 || y % grid_spacing == 0) {
        return border_color;
    } else {
        HSLAPixel white_fills;
        white_fills.h = 0.; 
        white_fills.s = 0.; 
        white_fills.l = 1.;
        return white_fills;
    }
}
