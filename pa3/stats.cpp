
#include "stats.h"

stats::stats(PNG & im){

    sumHueX.resize(im.width(), vector<double> (im.height(),0));
    sumHueY.resize(im.width(), vector<double> (im.height(),0));
    sumSat.resize(im.width(), vector<double> (im.height(),0));
    sumLum.resize(im.width(), vector<double> (im.height(),0));
    hist.resize(im.width(), vector<vector<int>> (im.height(), vector<int>(36, 0)));

    for (int x = 0; x < (int) im.width(); x++)
        for (int y = 0; y < (int) im.height(); y++) {
            HSLAPixel * pix = im.getPixel(x,y);

                if (x == 0 && y == 0) {
                    sumHueX[x][y] = cos(pix->h * PI/180);
                    sumHueY[x][y] = sin(pix->h * PI/180);
                    sumSat[x][y] = pix->s;
                    sumLum[x][y] = pix->l;

                    hist[y][x][(pix->h)/10] = 1;
                }

                else if (x == 0) {
                    sumHueX[x][y] = sumHueX[x][y-1] + cos(pix->h * PI/180);
                    sumHueY[x][y] = sumHueY[x][y-1] + sin(pix->h * PI/180);
                    sumSat[x][y] = sumSat[x][y-1] + pix->s;
                    sumLum[x][y] = sumLum[x][y-1] + pix->l;

                    for (int k = 0; k < 36; k++)
                        hist[x][y][k] = hist[x][y-1][k];
                    hist[x][y][(pix->h)/10] += 1;

                }

                else if (y == 0) {
                    sumHueX[x][y] = sumHueX[x-1][y] + cos(pix->h * PI/180);
                    sumHueY[x][y] = sumHueY[x-1][y] + sin(pix->h * PI/180);
                    sumSat[x][y] = sumSat[x-1][y] + pix->s;
                    sumLum[x][y] = sumLum[x-1][y] + pix->l;

                    for (int k = 0; k < 36; k++)
                        hist[x][y][k] = hist[x-1][y][k];
                    hist[x][y][(pix->h)/10] += 1;
                }

                else {
                    sumHueX[x][y] = sumHueX[x-1][y] + sumHueX[x][y-1] - sumHueX[x-1][y-1] + cos(pix->h * PI/180);
                    sumHueY[x][y] = sumHueY[x-1][y] + sumHueY[x][y-1] - sumHueY[x-1][y-1] + sin(pix->h * PI/180);
                    sumSat[x][y] = sumSat[x-1][y] + sumSat[x][y-1] - sumSat[x-1][y-1] + pix->s;
                    sumLum[x][y] = sumLum[x-1][y] + sumLum[x][y-1] - sumLum[x-1][y-1] + pix->l;

                    for (int k = 0; k < 36; k++)
                        hist[x][y][k] = hist[x-1][y][k] + hist[x][y-1][k] - hist[x-1][y-1][k];
                    hist[x][y][(pix->h)/10] += 1;
                }
        }
}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr) {
    return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}


HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr) {
    long area = rectArea(ul, lr);

    int xs = ul.first;
    int ys = ul.second;
    int xe = lr.first;
    int ye = lr.second;

    double totalSumHueX = sumHueX[xe][ye];
    double totalSumHueY = sumHueY[xe][ye];
    double totalSumSat = sumSat[xe][ye];
    double totalSumLum = sumLum[xe][ye];


    if (ys > 0) {
        totalSumHueX -= sumHueX[xe][ys-1];
        totalSumHueY -= sumHueY[xe][ys-1];
        totalSumSat -= sumSat[xe][ys-1];
        totalSumLum -= sumLum[xe][ys-1];
    }

    if (xs > 0) {
        totalSumHueX -= sumHueX[xs-1][ye];
        totalSumHueY -= sumHueY[xs-1][ye];
        totalSumSat -= sumSat[xs-1][ye];
        totalSumLum -= sumLum[xs-1][ye];
    }

    if (xs > 0 && ys > 0) {
        totalSumHueX += sumHueX[xs-1][ys-1];
        totalSumHueY += sumHueY[xs-1][ys-1];
        totalSumSat += sumSat[xs-1][ys-1];
        totalSumLum += sumLum[xs-1][ys-1];
    }

    double avgHue;
    if (totalSumHueX == 0)
    	avgHue = 90;
    else
    	avgHue = atan2(totalSumHueY, totalSumHueX) * 180 / PI;
    double averageSat = totalSumSat/area;
    double averageLum = totalSumLum/area;

    while (avgHue < 0)
        avgHue += 360;

    return HSLAPixel(avgHue, averageSat, averageLum, 1);
}


vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
    vector<int> newHist;
    newHist.resize(36, 0);
    int xs = ul.first;
    int ys = ul.second;
    int xe = lr.first;
    int ye = lr.second;

    for (int k=0; k<35; k++) {
        newHist[k] = hist[xe][ye][k];
        if (ys > 0)
            newHist[k] -= hist[xe][ys-1][k];
        if (xs > 0)
            newHist[k] -= hist[xs-1][ye][k];
        if (xs > 0 && ys > 0)
            newHist[k] += hist[xs-1][ys-1][k];
    }

    return newHist;
}


double stats::entropy(vector<int> & distn, int area){
    double entropy = 0.;
    for (int k = 0; k < 36; k++)
        if (distn[k] > 0 )
            entropy -= ((double) distn[k]/ area) * log2((double) distn[k]/area);

    return entropy;
}


vector<int> stats::addVector(vector<int> a, vector<int> b) {
    vector<int> sum;
    sum.resize(36, 0);

    for (int k = 0; k < (int) a.size(); k++)
        sum[k] = a[k] + b[k];

    return sum;
}


double stats::entropy(pair<int,int> ul, pair<int,int> lr) {
    vector<int> ansHist;
    ansHist.resize(36, 0);;
    int area = 0;

    int width = sumHueX.size();
    int height = 0;
    if (width> 0)
        height = sumHueX[0].size();

    int xs = ul.first;
    int ys = ul.second;
    int xe = lr.first;
    int ye = lr.second;

    if (xe >= xs && ye >= ys) {
        ansHist = addVector(ansHist, buildHist(ul, lr));
        area = rectArea(ul, lr);
    }

    else if (xe < xs && ye >= ys) {
    	ansHist = addVector(ansHist, buildHist(ul, pair<int,int>(width-1, ye)));
        ansHist = addVector(ansHist, buildHist(pair<int,int>(0, ys), lr));
        area = rectArea(ul, pair<int,int>(width-1, ye)) + rectArea(pair<int,int>(0, ys), lr);
    }

    else if (xe >= xs && ye < ys) {
    	ansHist = addVector(ansHist, buildHist(ul, pair<int,int>(xe, height-1)));
        ansHist = addVector(ansHist, buildHist(pair<int,int>(xs, 0), lr));
        area = rectArea(ul, pair<int,int>(xe, height-1)) + rectArea(pair<int,int>(xs, 0), lr);
    }

    else if (xe < xs && ye < ys){
        ansHist = addVector(ansHist, buildHist(pair<int,int>(0, 0), lr));
        ansHist = addVector(ansHist, buildHist(pair<int,int>(xs, 0), pair<int,int>(width-1, ye)));
        ansHist = addVector(ansHist, buildHist(pair<int,int>(0, ys), pair<int,int>(xe, height-1)));
        ansHist = addVector(ansHist, buildHist(ul, pair<int,int>(width-1, height-1)));

        area = rectArea(pair<int,int>(0, 0), lr) 
        	 + rectArea(pair<int,int>(xs, 0), pair<int,int>(width-1, ye))
         	 + rectArea(pair<int,int>(0, ys), pair<int,int>(xe, height-1))
         	 + rectArea(ul, pair<int,int>(width-1, height-1));
    }

    return entropy(ansHist, area);
}
