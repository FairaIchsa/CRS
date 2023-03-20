#include <iostream>
#include "class.h"

using namespace std;

int main()
{
    /* формат ввода: строки в количестве edges. 2 числа на одной строке - дуга графа.
     * первое число - номер начальной вершины, второе - номер конечной вершины (вершины - числа в диапазоне [0;nodes-1]).
     * порядок расположения дуг не важен.*/
    cout << "this programme calculates the adjacency matrix and reachability matrix of oriented graph\n******************************\n\n";
    graph Graph;
    Graph.adjacency_matrix_crs();
    Graph.reachability_matrix_crs();
    Graph.matrix_crs_output();
    return 0;
}

