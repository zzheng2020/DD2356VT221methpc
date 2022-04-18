#include <iostream>
#include <stdio.h>
#include <math.h>
#include <algorithm>

using namespace std;

// double t[] = {0.00508285, 0.00506592, 0.00506711, 0.00501704, 0.00506711};
double t[] = {0.01234007, 0.01195502, 0.01229692, 0.01158404, 0.01184011};

int main() {
    double sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += t[i];
    }

    cout << "avg == " << sum / 5.0 << endl;

    double avg = sum / 5.0;
    double dif = 0;
    for (int i = 0; i < 5; i++) {
        dif += (t[i] - avg) * (t[i] - avg);
    }
    cout << sqrt( dif / 5.0 ) << endl;
    return 0;
}