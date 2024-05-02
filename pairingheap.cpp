#include<iostream>
#include<fstream>
#include<vector>

using namespace std;


ifstream f("mergeheap.in");
ofstream g("mergeheap.out");

int n, q;     

int opt;
int x, y;  


class Node {
public:
    int value;
    Node* child;
    Node* sibling;

    Node() {
        child = NULL;
        sibling = NULL;
        value = 0;
    }

    Node(int valoare) {
        value = valoare;
        child = NULL;
        sibling = NULL;
    }

    Node(int valoare, Node* copil, Node* frate) {
        value = valoare;
        child = copil;
        sibling = frate;
    }
};

class PairingHeap {

public:
    Node* root;

    PairingHeap() {
        root = NULL;
    }

    PairingHeap(int x) {                                        //constructor folositor pentru a face merge-uri cu un singur element
        Node* n = new Node(x);
        root = n;
    }

    bool empty() {
        return (root == NULL);
    }

    void mergeWith(PairingHeap heap2) {                         // metoda folosita pentru merging, poate fi practic folosita si pentru insert-uri,
                                                                // deoarece putem merge-ui cu un heap cu un singur element
        if (heap2.empty()) {
            return;
        }

        if (this->empty()) {

            root = heap2.root;
            return;
        }


        if (this->root->value >= heap2.root->value) {
            heap2.root->sibling = this->root->child;            // adaugam heap2 la lista de subheapuri 
            this->root->child = heap2.root;
                                                            
        }
        else {
            this->root->sibling = heap2.root->child;            // intai adaugam la heap2 obiectul curent in lista de subheapuri ale lui heap2
            heap2.root->child = this->root;
            this->root = heap2.root;                            
       
        }


    }

    void insert(int val) {
        PairingHeap heap2 = PairingHeap(val);

        this->mergeWith(heap2);

        
    }

    int getMax() {
        return root->value;
    }

    void heapify(vector<int> listaElemente) {              // metoda care primeste o lista de elemente si formeaza in obiectul curent un pairing heap care le contine

        int lungime = listaElemente.size();

        if (lungime) {                                      
            Node* n = new Node(listaElemente[0]);
            this->root = n;

            for (int i = 1; i < lungime; ++i) {
                this->insert(listaElemente[i]);
            }
        }
    }

    void twoPassMerge() {

 

        if (root->child == NULL) {                                   // daca exista un singur nod(radacina), heapul ramane gol
            root = NULL;

 
            
            return;
        }

        if (root->child->sibling == NULL) {
            root = root->child;                                     // daca exista un singur subheap, devine noul heap principal
            


            return;
        }

        vector<PairingHeap> pairs;
        int nrPairs = 0;

        Node* current = this->root->child;
        Node* currentSibling = this->root->child->sibling;
        Node* nextSibling = currentSibling->sibling;        // variabila necesara pentru a nu pierde legatura catre urmatorul frate, daca exista
       
        do {                                                        // acesta este primul pass, care cupleaza subheapurile cate doua (ultimul poate ramane necuplat)     
            
            pairs.push_back(PairingHeap());                         

            PairingHeap siblingHeap = PairingHeap();

            pairs[nrPairs].root = current;
           
             
            siblingHeap.root = currentSibling;
             
            pairs[nrPairs].mergeWith(siblingHeap); 
                      
            if (nextSibling == NULL) {
                    
                ++nrPairs;
                break;
            }
            
            current = nextSibling;
            
            if (current->sibling == NULL) {
                ++nrPairs;

                pairs.push_back(PairingHeap());
                pairs[nrPairs].root = current;
                
                ++nrPairs;
                break;
            }

            currentSibling = current->sibling;
            nextSibling = currentSibling->sibling;

            ++nrPairs;
        } while (current != NULL);

        
        this->root = pairs[nrPairs - 1].root;                         // la al doilea pass luam de la dreapta la stanga perechile si le tot merge-uim succesiv
                                                                    // luand mereu rezultatul ultimului merge si urmatorul pair din stanga 
        
        for (int i = nrPairs - 2; i >= 0; --i) {

            this->mergeWith(pairs[i]);

        }
        
    }

    void deleteMax() {                                              // pentru a sterge maximul, folosim un two-pass merge/two-pass pairing
        if (!this->empty()) {
            this->twoPassMerge();
        }
    }


};

vector<PairingHeap> Heap;

int main() {

    f >> n >> q;

    for (int i = 0; i <= n; ++i) {
        Heap.push_back(PairingHeap());
    }

    for (int i = 0; i < q; ++i) {

        f >> opt;

        if (opt == 2) {
            f >> x;
            g <<Heap[x].getMax()<<'\n';
            Heap[x].deleteMax();

        }

        if (opt == 1) {
            f >> x >> y;
            Heap[x].insert(y);

        }

        if (opt == 3) {
            f >> x >> y;
       
            Heap[x].mergeWith(Heap[y]);

            Heap[y].root = NULL;

       }
    }


    return 0;
}