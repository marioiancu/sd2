#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

void generateOperations(int numHeaps, int numInserts, int numDeletes, int numUnions) {
    srand(time(0));

    std::ofstream file;
    file.open("mergeheap.in");

    file << numHeaps << " " << numInserts + numDeletes + numUnions << std::endl;

    for (int i = 0; i < numInserts; i++) {
        int heapNum = rand() % numHeaps + 1;
        int value = rand() % 100000; 
        file << "1 " << heapNum << " " << value << std::endl;
    }

    for (int i = 0; i < numDeletes; i++) {
        int heapNum = rand() % numHeaps + 1;
        file << "2 " << heapNum << std::endl;
    }

    for (int i = 0; i < numUnions; i++) {
        int heapNum1 = rand() % numHeaps + 1;
        int heapNum2;
        do {
            heapNum2 = rand() % numHeaps + 1;
        } while (heapNum1 == heapNum2);
        file << "3 " << heapNum1 << " " << heapNum2 << std::endl;
    }

    file.close();
}

int main() {
    //modificate in funție de numarul de operații dorite
    int numHeaps = 50;
    int numInserts = 50000;
    int numDeletes = 2500;
    int numUnions = 2;

    generateOperations(numHeaps, numInserts, numDeletes, numUnions);

    return 0;
}