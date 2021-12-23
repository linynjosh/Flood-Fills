
#include "stats.h"

stats::stats(PNG & im){
    /* your code here! */
    int width = im.width();
    int height = im.height();
    sumHueX.resize(height);
    sumHueY.resize(height);
    sumSat.resize(height);
    sumLum.resize(height);
    hist.resize(height);

    for(int i=0; i<height; i++){
        sumHueX[i].resize(width);
        sumHueY[i].resize(width);
        sumSat[i].resize(width);
        sumLum[i].resize(width);
        //2*pi * (x / 360)
        for(int j=0; j<width; j++){
            sumHueX[i][j] = cos(2*PI*(im.getPixel(j,i)->h / 360));
            sumHueY[i][j] = sin(2*PI*(im.getPixel(j,i)->h / 360));
            sumSat[i][j] = im.getPixel(j, i)->s;
            sumLum[i][j] = im.getPixel(j, i)->l;
            if(i>0){
                sumHueX[i][j] += sumHueX[i-1][j];
                sumHueY[i][j] += sumHueY[i-1][j];
                sumSat[i][j] += sumSat[i-1][j];
                sumLum[i][j] += sumLum[i-1][j];
            }
            if(j>0){
                sumHueX[i][j] += sumHueX[i][j-1];
                sumHueY[i][j] += sumHueY[i][j-1];
                sumSat[i][j] += sumSat[i][j-1];
                sumLum[i][j] += sumLum[i][j-1];
            }
            if(i>0 && j>0){
                sumHueX[i][j] -= sumHueX[i-1][j-1];
                sumHueY[i][j] -= sumHueY[i-1][j-1];
                sumSat[i][j] -= sumSat[i-1][j-1];
                sumLum[i][j] -= sumLum[i-1][j-1];
            }
        }
    }

    for(int i=0; i<height; i++){
        hist[i].resize(width);
        for(int j=0; j<width; j++){
            hist[i][j].resize(36);
            for(int k=0; k<36; k++){
                hist[i][j][k] = 0;
                if(i>0){
                    hist[i][j][k] += hist[i-1][j][k];
                }
                if(j>0){
                    hist[i][j][k] += hist[i][j-1][k];
                }
                if(i>0 && j>0){
                    hist[i][j][k] -= hist[i-1][j-1][k];
                }
                
                if(k*10 <= (int)im.getPixel(j, i)->h 
                    && (int)im.getPixel(j, i)->h < (k+1)*10){
                    hist[i][j][k] += 1;
                }  
            }
        }
    }

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    /* your code here */
    int width = lr.first - ul.first + 1;
    int height = lr.second - ul.second + 1;
    return (long)(width * height);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    /* your code here */
    double area = (double)rectArea(ul, lr);
    double sumHX, sumHY, sumS, sumL;
    
    sumHX = sumHueX[lr.second][lr.first];
    sumHY = sumHueY[lr.second][lr.first];
    sumS = sumSat[lr.second][lr.first];
    sumL = sumLum[lr.second][lr.first];

    if(ul.second > 0){
        sumHX -= sumHueX[ul.second-1][lr.first];
        sumHY -= sumHueY[ul.second-1][lr.first];
        sumS -= sumSat[ul.second-1][lr.first];
        sumL -= sumLum[ul.second-1][lr.first];
    }
    if(ul.first > 0){
        sumHX -= sumHueX[lr.second][ul.first-1];
        sumHY -= sumHueY[lr.second][ul.first-1];
        sumS -= sumSat[lr.second][ul.first-1];
        sumL -= sumLum[lr.second][ul.first-1];
    }
    if(ul.second>0 && ul.first>0){
        sumHX += sumHueX[ul.second-1][ul.first-1];
        sumHY += sumHueY[ul.second-1][ul.first-1];
        sumS += sumSat[ul.second-1][ul.first-1];
        sumL += sumLum[ul.second-1][ul.first-1];
    }

    double avg_hue = toDeg(atan2((sumHY/area), (sumHX/area)));
    if(avg_hue < 0)
        avg_hue += 360.0;

    HSLAPixel pixel = HSLAPixel(avg_hue, sumS/area, sumL/area, 1.0);

    return pixel;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn;

    /* using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distn.
    *  You will use distn to compute the entropy over the rectangle.
    *  if any bin in the distn has frequency 0, then do not add that 
    *  term to the entropy total. see .h file for more details.
    */

    /* my code includes the following lines:
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    */
    
    return 1.0;
    // return  -1 //* entropy;
}
