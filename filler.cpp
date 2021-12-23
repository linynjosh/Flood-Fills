/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
//#include "filler.h"
#include <utility>

animation filler::fillSolidDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                 double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    solidColorPicker a(fillColor);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGridDFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gridColorPicker a(gridColor, gridSpacing);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGradientDFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gradientColorPicker a(fadeColor1, fadeColor2, radius, x, y);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillRainDFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{	/**
     *  Our code here!
     */

    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
   }

animation filler::fillSolidBFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                 double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    
    solidColorPicker a(fillColor);
	return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGridBFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gridColorPicker a(gridColor, gridSpacing);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGradientBFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gradientColorPicker a(fadeColor1, fadeColor2, radius, x, y);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}
animation filler::fillRainBFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    /**
     * Our code here!
     */
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}





template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       double tolerance, int frameFreq)
{
	 /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. 
     *
     * To begin the algorithm, you simply:
     * a) place the given point in the ordering structure, 
     * marking it processed (the way you mark it is a design 
     * decision you'll make yourself).
     *
     * b) We have a choice to either change the color, if appropriate, 
     * when we * add the point to the OS, or when we take it off. 
     * In our test cases, * we have assumed that you will change 
     * the color when a point is added to the structure. 
     *
     * c) Add the first image to the animation (it's convenient to 
     * think of this as the 0th image). 
     *
     * THEN, until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        a.    add its unprocessed neighbors whose color values are 
     *              within (or equal to) tolerance distance from the 
     *              center's color, to the ordering structure, set their
     *              new color, and mark them as processed.
     *        b.    if it is an appropriate frame, send the current 
     *              PNG to the animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore 
     *        neighboring pixels in some order.
     *
     *        While the order in which you examine neighbors does 
     *        not matter for a proper fill, you must use the same 
     *        order as we do for your animations to come out like 
     *        ours! The order you should put neighboring pixels 
     *        **ONTO** the queue or stack is as follows:
     *        RIGHT(+x), DOWN(+y), LEFT(-x), UP(-y). 
     *        IMPORTANT NOTE: *UP* here means towards the top of 
     *        the image, so since an image has smaller y coordinates 
     *        at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction. 
     *
     *        To reiterate, when you are exploring 
     *        (filling out) from a given pixel, you must first try 
     *        to fill the pixel to
     *        it's RIGHT, then the one DOWN from it, then to the LEFT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then 
     *        again after the 8th pixel, etc.  You must only add 
     *        frames for the number of pixels that have been filled, 
     *        not the number that have been checked. So if frameFreq 
     *        is set to 1, a pixel should be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the 
     *        fill, and it will be the one we test against.
     */
    OrderingStructure<pair<int, int>> processed_points = OrderingStructure<pair<int,int>>();
    animation anime = animation();
    vector<vector<bool>> processed_tracker;
    int frame_tracker = 0;
    
    for (int i = 0; i < img.height(); i++) {
        vector<bool> row;
        for (int j = 0; j < img.width(); j++) {
            row.push_back(false);
        }
        processed_tracker.push_back(row); 
    }

    HSLAPixel* first_pixel = img.getPixel(x,y);
    HSLAPixel orig_pixel = HSLAPixel(first_pixel->h, first_pixel->s, first_pixel->l, first_pixel->a);
    *first_pixel = fillColor(x,y);
    pair<int,int> first_point = {x,y};
    processed_points.add(first_point);
    processed_tracker[first_point.second][first_point.first] = true;
    anime.addFrame(img);

    while (!processed_points.isEmpty()) {
        pair<int,int> processed_point = processed_points.remove();
        pair<int,int> right = {processed_point.first + 1, processed_point.second};
        pair<int,int> down = {processed_point.first, processed_point.second + 1};
        pair<int,int> left = {processed_point.first - 1, processed_point.second};
        pair<int,int> up = {processed_point.first, processed_point.second - 1};
        HSLAPixel* right_pixel = img.getPixel(processed_point.first + 1, processed_point.second);
        HSLAPixel* down_pixel = img.getPixel(processed_point.first, processed_point.second + 1);
        HSLAPixel* left_pixel = img.getPixel(processed_point.first - 1, processed_point.second);
        HSLAPixel* up_pixel = img.getPixel(processed_point.first, processed_point.second - 1);
  
        if (right.first < img.width() && !processed_tracker[right.second][right.first] && right_pixel->dist(orig_pixel) <= tolerance) {
            *right_pixel = fillColor(right.first, right.second);
            processed_points.add(right);
            processed_tracker[right.second][right.first] = true;
            frame_tracker += 1;
            if (frame_tracker == frameFreq) {
                anime.addFrame(img);
                frame_tracker = 0;
            }
        }

        if (down.second < img.height() && !processed_tracker[down.second][down.first] && down_pixel->dist(orig_pixel) <= tolerance) {
            *down_pixel = fillColor(down.first, down.second);
            processed_points.add(down);
            processed_tracker[down.second][down.first] = true;
            frame_tracker += 1;
            if (frame_tracker == frameFreq) {
                anime.addFrame(img);
                frame_tracker = 0;
            }
        }

        if (left.first < img.width() && !processed_tracker[left.second][left.first] && left_pixel->dist(orig_pixel) <= tolerance) {
            *left_pixel = fillColor(left.first, left.second);
            processed_points.add(left);
            processed_tracker[left.second][left.first] = true;
            frame_tracker += 1;
            if (frame_tracker == frameFreq) {
                anime.addFrame(img);
                frame_tracker = 0;
            }
        }

        if (up.second < img.height() && !processed_tracker[up.second][up.first] && up_pixel->dist(orig_pixel) <= tolerance) {
            *up_pixel = fillColor(up.first, up.second);
            processed_points.add(up);
            processed_tracker[up.second][up.first] = true;
            frame_tracker += 1;
            if (frame_tracker == frameFreq) {
                anime.addFrame(img);
                frame_tracker = 0;
            }
        }
    }
    anime.addFrame(img);
    return anime;
}
    



