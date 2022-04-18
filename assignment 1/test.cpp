#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

// double x[][5] = {
//     {7072.8, 7087.8, 6963.2, 6739.1, 8304.8},
//     {13359.0, 13903.4, 13814.7, 13839.7, 11647.4},
//     {17282.3, 24752.5, 22124.0, 20470.6, 15531.2},
//     {25554.6, 22640.6, 25534.3, 22698.0, 21482.4},
//     {20207.4, 23330.0, 20240.3, 20270.9, 20539.5},
//     {21022.1, 21531.3, 20578.6, 21516.8, 21228.2},
//     {24503.0, 24676.0, 24573.9, 24585.6, 26320.3},
//     {27957.4, 28683.9, 28199.4, 28818.2, 28084.9},
//     {31086.2, 30863.2, 30372.9, 30686.8, 31576.2},
//     {33312.9, 32713.7, 32408.8, 32271.6, 32134.1}
// };

double x[][3] = {
    {222805.0, 213314.9, 213654.5}, // static
    {62.9, 62.3, 62.9},
    {33312.9, 32713.7, 32408.8}
};

vector<double> avg[10], std_dev[10];

int main() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << x[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    for (int i = 0; i < 3; i++) {
        double sum = 0;
        for (int j = 0; j < 3; j++) {
            sum += x[i][j];
        }
        avg[i].push_back(sum / 3.0);
        
        double tmp_avg = sum / 3.0;
        double dif = 0;
        for (int j = 0; j < 3; j++) {
            dif += sqrt( ( (x[i][j] - tmp_avg) * (x[i][j] - tmp_avg) ) / 5.0);
        }
        std_dev[i].push_back(dif);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < avg[i].size(); j++) {
            cout << "avg == " << avg[i][j] << ", std_dev == " << std_dev[i][j] << "\n";
        }
    }


    return 0;
}