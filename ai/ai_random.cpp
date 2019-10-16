//Sukūrė: Džiugas Juozapavičius
//Tai yra bandomasis AI. Jis juda atsitiktinai, nors nesitaiko į tą patį langelį daugiau nei vieną kartą.

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){

    cout << "0 ";
            
    int table [100]  = {0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 3, 3, 3, 0, 4, 4, 4,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 5, 5};

    for (int i=0; i<100; i++) {
        cout << table [i] << " ";
    }
    int moves [100];
    for (int i=0; i<100; i++)  {
        moves [i] = i;
    }
    int moveCount = 100;
    while (true) {
        srand (time(NULL));
        int currentMove = rand() % moveCount;
        int targetY = (moves [currentMove] / 10);
        int targetX = (moves [currentMove] - 10 * targetY);

        cout << targetX + 1 << " " << targetY + 1 << " ";
        char status = '0';
        cin >> status;
        if (status == '4') {
            return 0;
        }
        moves [currentMove] = 100;
        moveCount--;
        sort (moves, moves + 100);
    }
}
