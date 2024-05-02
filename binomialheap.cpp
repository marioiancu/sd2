#include <iostream>
#include <fstream>
#include <list>

using namespace std;

const int INF = 2000000001;
const int NMAX = 101;

ifstream fin ( "mergeheap.in" );
ofstream fout ( "mergeheap.out" );

struct node{
    int key, degree;
    node *child, *sibling, *parent;
};

node* new_node( int val ){
    node* temp = new node;
    temp -> key = val;
    temp -> degree = 0;
    temp -> child = temp -> sibling = temp -> parent = NULL;
    return temp;
}

class binomial_heap{

    list < node* > H;

    list < node* > :: iterator get_root(){

        list < node* > :: iterator it, it_max;
        node* vmax = new_node( -INF );

        for( it = H.begin(); it != H.end(); ++it )
            if( (*it) -> key > vmax -> key ){
                vmax = *it;
                it_max = it;
            }

        return it_max;
    }

    void delete_root( node* tree, binomial_heap& heap ){

        if( tree -> degree == 0 ){
            delete tree;
            return;
        }

        node* temp = tree;

        tree -> child -> parent = NULL;
        heap.H.push_front( tree -> child );

        tree = tree -> child;
        while( tree -> sibling ){
            tree -> sibling -> parent = NULL;
            heap.H.push_front( tree -> sibling );
            tree = tree -> sibling;
        }
        delete temp;
    }

    void merge_tree( node* tree1, node* tree2 ){

        if( tree1 -> key < tree2 -> key )
            swap ( *tree1, *tree2 );

        tree2 -> sibling = tree1 -> child;
        tree2 -> parent = tree1;
        tree1 -> child = tree2;
        tree1 -> degree++;

    }

    void adjust(){

        if( H.size() <= 1 ) return;

        list < node* > :: iterator prev;
        list < node* > :: iterator curr;
        list < node* > :: iterator next;
        list < node* > :: iterator temp;

        prev = curr = H.begin();
        curr++;
        next = curr;
        next++;

        while( curr != H.end() ){

            while ( ( next == H.end() || (*next) -> degree > (*curr) -> degree ) && curr != H.end() && (*prev) -> degree == (*curr) -> degree ){

                merge_tree( *curr, *prev );

                temp = prev;

                if( prev == H.begin() ){
                    prev++;
                    curr++;
                    if( next != H.end() )
                        next++;
                }
                else prev--;

                H.erase( temp );

            }

            prev++;
            if( curr != H.end() ) curr++;
            if( next != H.end() ) next++;
        }
    }
public:

    int top(){
        return (*get_root()) -> key;
    }

    void push( int val ){

        node *tree = new_node( val );
        H.push_front( tree );
        adjust();
    }

    void heap_union( binomial_heap& heap2){

        list < node* > :: iterator it1 = H.begin();
        list < node* > :: iterator it2 = heap2.H.begin();

        list < node* > new_heap;

        while( it1 != H.end() && it2 != heap2.H.end() ){
            if( (*it1) -> degree <= (*it2) -> degree ){
                new_heap.push_back( *it1 );
                it1++;
            }
            else{
                new_heap.push_back( *it2 );
                it2++;
            }
        }

        while( it1 != H.end() ){
            new_heap.push_back( *it1 );
            it1++;
        }

        while( it2 != heap2.H.end() ){
            new_heap.push_back( *it2 );
            it2++;
        }

        heap2.H.clear();

        H = new_heap;
        adjust();
    }

    void pop(){

        list < node* > :: iterator root = get_root();

        binomial_heap new_heap;
        delete_root( (*root), new_heap );

        H.erase( root );

        heap_union( new_heap );

    }

    int find_min() {
        node* min_node = nullptr;
        for (node* tree : H) {
            if (min_node == nullptr || tree->key < min_node->key) {
                min_node = tree;
            }
        }
        return min_node ? min_node->key : INF;
    }
    int extract_min() {
        int min_val = find_min();
        if (min_val == INF) return INF;

        node* min_node = nullptr;
        for (node* tree : H) {
            if (tree->key == min_val) {
                min_node = tree;
                break;
            }
        }

        H.remove(min_node);

        binomial_heap children_heap;
        for (node* child = min_node->child; child != nullptr; child = child->sibling) {
          children_heap.H.push_front(child);
        }       
        heap_union(children_heap);

        delete min_node;
        return min_val;
    }

};

int N, M;
binomial_heap Heap[NMAX];

int main()
{
    fin >> N >> M;

    int task, h, x, h1, h2;
    for( int i = 1; i <= M; ++i ){
        fin >> task;

        if( task == 1 ){

            fin >> h >> x;
            Heap[h].push( x );

        }
        if( task == 2 ){

            fin >> h;
            fout << Heap[h].top() << '\n';
            Heap[h].pop();
        }
        if( task == 3 ){

            fin >> h1 >> h2;

            Heap[h1].heap_union( Heap[h2] );
        }
    }

    return 0;
}