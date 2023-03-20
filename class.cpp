#include <iostream>
#include <fstream>
#include "class.h"

using namespace std;

graph::~graph(){

  for (int i = 0; i < edges; i++){
      delete[] Edge[i];
  }
  delete[] Edge;

  //delete[] Value;
  delete[] Column_index;
  delete[] Row_indexation;

  //delete[] T_Value;
  delete[] T_Column_index;
  delete[] T_Row_indexation;

  delete[] Pow_Value;
  delete[] Pow_Column_index;
  delete[] Pow_Row_indexation;

  delete[] R_Value;
  delete[] R_Column_index;
  delete[] R_Row_indexation;
}

graph::graph(){ //ввод дуг

    Edge = new int *[edges];
    for (int i = 0; i < edges; i++){
        Edge[i] = new int [2];
    }
    bool in_way; //способ ввода

    cout << "how do you want to insert the data?\n0 - from console,\t1 - from file\n";
    cin >> in_way;
    if (in_way){
        ifstream fin;
        fin.open("../Practice_2021/input.txt");
        if (fin.is_open()){
            for (int i = 0; i < edges; i++){
                fin >> Edge[i][0] >> Edge[i][1];
                cout << "your edge " << i << ": " << Edge[i][0] << " " << Edge[i][1] << "\n";
            }
        }
        else{
            cout << "can't open file\n";
        }

        fin.close();
    }
    else{
        cout << "insert edges (couples of numbers)\n";
        for (int i = 0; i < edges; i++){ //ввод и сортировка дуг
            cout << "insert edge number " << i << ": ";
            cin >> Edge[i][0] >> Edge[i][1];
        }
    }
}

void graph::adjacency_matrix_crs(){ //рассчет матрицы смежности
    int temp_0, temp_1; //нужны для сортировки массива дуг

    for (int i = 0; i < edges; i++){ //сортировка дуг для матрицы достижимости
        for (int j = i; j > 0; j--){
            if (Edge[j][0] < Edge[j-1][0]){
                temp_0 = Edge[j][0];
                Edge[j][0] = Edge[j-1][0];
                Edge[j-1][0] = temp_0;
                if (Edge[j][1] != Edge[j-1][1]){
                    temp_1 = Edge[j][1];
                    Edge[j][1] = Edge[j-1][1];
                    Edge[j-1][1] = temp_1;
                }
            }
            else if ((Edge[j][0] == Edge[j-1][0]) & (Edge[j][1] < Edge[j-1][1])){
                temp_1 = Edge[j][1];
                Edge[j][1] = Edge[j-1][1];
                Edge[j-1][1] = temp_1;
            }
        }
    }

    Column_index = new int[edges];
    Row_indexation = new int[nodes+1] {};

    for (int i = 0; i < edges; i++){ //рассчет матрицы смежности
        Column_index[i] = Edge[i][1];
        Row_indexation[Edge[i][0]+1]++;
        cout << Edge[i][0] << "\t" << Edge[i][1] << "\n";
    }

    for (int i = 1; i < nodes + 1; i++){
        Row_indexation[i] += Row_indexation[i-1];
    }

    pow_edges = edges; //приравнивание матрицы Pow к матрице смежности
    Pow_Value = new int [pow_edges];
    Pow_Column_index = new int [pow_edges];
    Pow_Row_indexation = new int [nodes+1];

    for (int i = 0; i < nodes + 1; i++)
        Pow_Row_indexation[i] = Row_indexation[i];

    for (int i = 0; i < edges; i++){
        Pow_Value[i] = 1;
        Pow_Column_index[i] = Column_index[i];
    }
}

void graph::t_adjacency_matrix_crs(){ //рассчет транспонированной матрицы смежности

    T_Column_index = new int[edges];
    T_Row_indexation = new int[nodes + 1] {};
    int temp_0, temp_1; //нужны для сортировки массива дуг

    for (int i = 0; i < edges; i++){ //сортировка дуг для транспонированной матрицы
        for (int j = i; j > 0; j--){
            if (Edge[j][1] < Edge[j-1][1]){
                temp_1 = Edge[j][1];
                Edge[j][1] = Edge[j-1][1];
                Edge[j-1][1] = temp_1;
                if (Edge[j][0] != Edge[j-1][0]){
                    temp_0 = Edge[j][0];
                    Edge[j][0] = Edge[j-1][0];
                    Edge[j-1][0] = temp_0;
                }
            }
            else if ((Edge[j][1] == Edge[j-1][1]) & (Edge[j][0] < Edge[j-1][0])){
                temp_0 = Edge[j][0];
                Edge[j][0] = Edge[j-1][0];
                Edge[j-1][0] = temp_0;
            }
        }
    }

    for (int i = 0; i < edges; i++){ //рассчет транспонированной матрицы смежности
        T_Column_index[i] = Edge[i][0];
        T_Row_indexation[Edge[i][1]+1]++;
    }

    for (int i = 1; i < nodes+1; i++){
       T_Row_indexation[i] += T_Row_indexation[i-1];
    }
}

void graph::multiplication_matrix_crs(){ //умножение матрицы Pow на транспонированную матрицу смежности

    bool *Flag = new bool [nodes]; //нужен для избежания повторений во время подсчета значений
    for (int i = 0; i < nodes; i++){
        Flag[i] = 1;
    }
    int temp_counter = 0; //нужен для подсчета количества значений в новой матрице (для определенияя длин массивов)

    //определение длин массивов
    for (int i = 0; i < nodes; i++){ //переход на новую строку
        for (int j = Pow_Row_indexation[i]; j < Pow_Row_indexation[i+1]; j++){ //переход к новому элементу
            for (int k = 0; k < nodes; k++){ //переход на новую строку в транспонированной матрице
                for (int l = T_Row_indexation[k]; l < T_Row_indexation[k+1]; l++){ //переход к новому элементу в транспонированной матрице
                    if ((Pow_Column_index[j] == T_Column_index[l]) & (Flag[k])){
                        temp_counter++;
                        Flag[k] = 0;
                    }
                }
            }
        }
        for (int j = 0; j < nodes; j++)
            Flag[j] = 1;
    }


    int *Temp_Value = new int [pow_edges]; //создание временной матрицы
    int *Temp_Column_index = new int [pow_edges];
    int *Temp_Row_indexation = new int [nodes+1];

    for (int i = 0; i < pow_edges; i++){ //приравнивание временной матрицы к матрице, возведенной в степень
        Temp_Value[i] = Pow_Value[i];
        Temp_Column_index[i] = Pow_Column_index[i];
    }
    for (int i = 0; i < nodes+1; i++)
        Temp_Row_indexation[i] = Pow_Row_indexation[i];

    delete[] Pow_Value; //удаление старой матрицы, возведенной в степень (Pow_Row_indexation не удаляется, потому что количество элементов массива не меняется)
    delete[] Pow_Column_index;

    pow_edges = temp_counter; //обновление количества дуг матрицы, возведенной в степень

    Pow_Value = new int [pow_edges]; //создание новой матрицы, возведеденной в степень
    Pow_Column_index = new int [pow_edges];

    //умножение
    int index_counter = 0; //считает количество значений в строке
    int temp_v, temp_c; //нужны для сортировки

    Pow_Row_indexation[0] = 0;
    for (int i = 0; i < nodes; i++){ //переход на новую строку
        for (int j = Temp_Row_indexation[i]; j < Temp_Row_indexation[i+1]; j++){ //переход к новому элементу
            for (int k = 0; k < nodes; k++){ //переход на новую строку в транспонированной матрице
                for (int l = T_Row_indexation[k]; l < T_Row_indexation[k+1]; l++){ //переход к новому элементу в транспонированной матрице
                    if (Temp_Column_index[j] == T_Column_index[l]){
                        if (Flag[k]){
                            Pow_Value[index_counter] = Temp_Value[j]; // = Pow_Value[j] * 1
                            Pow_Column_index[index_counter] = k; // номер столбца совпадает с номером строки транспонированной матрицы
                            index_counter++;
                            Flag[k] = 0;
                        }
                        else{
                            for (int m = Pow_Row_indexation[i]; m <= index_counter; m++){
                                if (Pow_Column_index[m] == k)
                                    Pow_Value[m] += Temp_Value[j];
                            }
                        }
                    }
                }
            }
        }
        for (int j = 0; j < nodes; j++)
            Flag[j] = 1;
        Pow_Row_indexation[i+1] = index_counter;
        for (int j = Pow_Row_indexation[i]; j < Pow_Row_indexation[i+1]; j++){ //сортировка внутри строки
            for (int k = j; k > Pow_Row_indexation[i]; k--){
                if (Pow_Column_index[k] < Pow_Column_index[k-1]){
                    temp_c = Pow_Column_index[k];
                    temp_v = Pow_Value[k];
                    Pow_Column_index[k] = Pow_Column_index[k-1];
                    Pow_Value[k] = Pow_Value[k-1];
                    Pow_Column_index[k-1] = temp_c;
                    Pow_Value[k-1]= temp_v;
                }
            }
        }
    }
    Pow_Row_indexation[nodes+1] = index_counter;

    delete[] Temp_Value; //удаление временной матрицы
    delete[] Temp_Column_index;
    delete[] Temp_Row_indexation;
}

void graph::addition_matrix_crs(){ //здесь складываются матрицы "Pow" и "R"

    int temp_edges = r_edges;
    int *Temp_Value = new int [temp_edges]; //создание временной матрицы
    int *Temp_Column_index = new int [temp_edges];
    int *Temp_Row_indexation = new int [nodes+1];

    for (int i = 0; i < temp_edges; i++){ //приравнивание временной матрицы к старой матрице достижимости
        Temp_Value[i] = R_Value[i];
        Temp_Column_index[i] = R_Column_index[i];
    }
    for (int i = 0; i < nodes+1; i++){
        Temp_Row_indexation[i] = R_Row_indexation[i];
    }

    delete[] R_Value; //удаление старой матрицы достижимости (R_Row_indexation не удаляется, потому что количество элементов в массиве не меняется)
    delete[] R_Column_index;
    R_Row_indexation[0] = 0;

    int edges_counter = r_edges + pow_edges; //нужен для подсчета количества элеметов в новой матрице
    int edges_in_row_counter = 0; //нужен для подсчета повторяющихся ребер в строке
    int temp_row_difference, pow_row_difference; //нужны для рассчета разности элементов Row_indexation[i+1] и Row_indexation[i]
    int pow_last_index; /*нужен для уменьшения количества итераций цикла (при наличии более чем одного элемента в строке первой матрицы (R),
следующий элемент (любой, после первого) будет сравниваться не со всеми элементами второй матрицы, а с последующими после совпадения)*/

    //цикл рассчета количества значений (кажется, работает)
    for (int i = 0; i < nodes; i++){ //переход на новую строку
        pow_last_index = Pow_Row_indexation[i];
        for (int j = Temp_Row_indexation[i]; j < Temp_Row_indexation[i+1]; j++){ //переход к новому элементу
            for (int k = pow_last_index; k < Pow_Row_indexation[i+1]; k++){ //переход к новому элементу в транспонированной матрице
                if (Temp_Column_index[j] == Pow_Column_index[k]){
                    edges_counter--;
                    edges_in_row_counter++;
                    pow_last_index = k;
                    break;
                }
            }
        }
        temp_row_difference = Temp_Row_indexation[i+1] - Temp_Row_indexation[i];
        pow_row_difference = Pow_Row_indexation[i+1] - Pow_Row_indexation[i];
        R_Row_indexation[i+1] = R_Row_indexation[i] + temp_row_difference + pow_row_difference - edges_in_row_counter;
        edges_in_row_counter = 0;
    }
    r_edges = edges_counter;

    R_Value = new int [r_edges];
    R_Column_index = new int [r_edges];

    int temp_index_counter = 0, pow_index_counter = 0; //счетики индексов временной матрицы и матрицы, возведенной в степень
    int temp_row_counter = 0, pow_row_counter = 0; //счетчики строк
    for (int i = 0; i < r_edges; i++){ //просто сделать прибавление элемента с меньшим индексом
        while ((temp_index_counter >= Temp_Row_indexation[temp_row_counter+1]) | (pow_index_counter >= Pow_Row_indexation[pow_row_counter+1])){
            if (temp_index_counter >= Temp_Row_indexation[temp_row_counter+1]){
                temp_row_counter++;
            }
            else{
                pow_row_counter++;
            }
        }
        if (temp_row_counter == pow_row_counter){ //если оба элемента на одной строке
            if (Temp_Column_index[temp_index_counter] == Pow_Column_index[pow_index_counter]){
                R_Value[i] = Temp_Value[temp_index_counter] + Pow_Value[pow_index_counter];
                R_Column_index[i] = Temp_Column_index[temp_index_counter];
                pow_index_counter++;
                temp_index_counter++;
            }
            else if (Temp_Column_index[temp_index_counter] < Pow_Column_index[pow_index_counter]){
                R_Value[i] = Temp_Value[temp_index_counter];
                R_Column_index[i] = Temp_Column_index[temp_index_counter];
                temp_index_counter++;
            }
            else{ //if (Temp_Column_index[temp_index_counter] > Pow_Column_index[pow_index_counter])
                R_Value[i] = Pow_Value[pow_index_counter];
                R_Column_index[i] = Pow_Column_index[pow_index_counter];
                pow_index_counter++;
            }
        }
        else if (temp_row_counter < pow_row_counter){ //если элементы на разных строках
            R_Value[i] = Temp_Value[temp_index_counter];
            R_Column_index[i] = Temp_Column_index[temp_index_counter];
            temp_index_counter++;
        }
        else{ //if (temp_row_counter > pow_row_counter)
            R_Value[i] = Pow_Value[pow_index_counter];
            R_Column_index[i] = Pow_Column_index[pow_index_counter];
            pow_index_counter++;
        }
    }

    if (temp_edges == r_edges){
        for (int i = 0; i < r_edges; i++){
            repeat = 0;
            if (Temp_Column_index[i] != R_Column_index[i]){
                repeat = 1;
                break;
            }
        }

        for (int i = 0; i < nodes+1; i++) { //если значения массивов R_Column_index и R_Row_indexation не изменяются, то матрица достижимости найдена
            repeat = 0;
            if (Temp_Row_indexation[i] != R_Row_indexation[i]){
                repeat = 1;
                break;
            }
        }
    }

    if (!repeat){
        delete[] R_Value;
        delete[] R_Column_index;


        R_Value = new int [r_edges];
        R_Column_index = new int [r_edges];

        for (int i = 0; i < r_edges; i++){
            R_Value[i] = Temp_Value[i];
            R_Column_index[i] = Temp_Column_index[i];
        }

        for (int i = 0; i < nodes+1; i++){
            R_Row_indexation[i] = Temp_Row_indexation[i];
        }
    }

    delete[] Temp_Value; //удаление временной матрицы
    delete[] Temp_Column_index;
    delete[] Temp_Row_indexation;
}

void graph::reachability_matrix_crs(){ //расчет матрицы достижимости

    t_adjacency_matrix_crs();

    //объявление матрицы достижимости (суммирование матрицы смежности и единичой матрицы)
    r_edges = edges + nodes; /*программа не поддерживает работу с псевдографами (графами с петлями),
    поэтому на главной диагонали матрицы смежности значений нет. это позволяет нам просто добавить элементы на главную диагональ*/
    R_Row_indexation = new int [nodes+1]; //создание матрицы достижимости
    R_Value = new int [r_edges];
    R_Column_index = new int [r_edges];

    for (int i = 0; i < nodes+1; i++){
        R_Row_indexation[i] = Row_indexation[i] + i;
    }

    bool flag = 1; //показывает, был ли пройден элемент главной диагонали на текущей строке
    int index_counter = 0; //счетчик индексов для матрицы смежности
    int temp_column_index; //переменная, которая заменяет Column_index[index_counter] (нужна в случае, когда index_counter выходит за границы массива)
    for (int i = 0; i < nodes; i++){ //переход на новую строку
        for (int j = R_Row_indexation[i]; j < R_Row_indexation[i+1]; j++){ //переход к новому элементу
            if ((index_counter >= Row_indexation[i+1]) & (flag)){ //выполнение этого условия приведет к выполнению следующего if
                temp_column_index = nodes + 1;
            }
            else{
                temp_column_index = Column_index[index_counter];
            }
            if ((temp_column_index > i) & (flag)){
                R_Value[j] = 1;
                R_Column_index[j] = i;
                flag = 0;
            }
            else{
                R_Value[j] = 1;
                R_Column_index[j] = temp_column_index;
                index_counter++;
            }
        }
        flag = 1;
    }

    cout << "reachability matrix:\n";
    for (int i = 0; i < r_edges; i++)
        cout << R_Value[i] << "\t";
    cout << "\n";
    for (int i = 0; i < r_edges; i++)
        cout << R_Column_index[i] << "\t";
    cout << "\n";
    for (int i = 0; i < nodes+1; i++)
        cout << R_Row_indexation[i] << "\t";
    cout << "\n\n";

    while (repeat){
        multiplication_matrix_crs();
        addition_matrix_crs();
    }
}

void graph::matrix_crs_output(){

    bool out_way; //способ вывода

    cout << "how do you want to output the result?\n0 - in console,\t1 - in file\n";
    cin >> out_way;

    if(out_way){
        ofstream fout;
        fout.open("../Practice_2021/output.txt");
        fout << "adjacency matrix:\n";
        for (int i = 0; i < edges; i++)
            fout << "1\t";
        fout << "\n";
        for (int i = 0; i < edges; i++)
            fout << Column_index[i] << "\t";
        fout << "\n";
        for (int i = 0; i < nodes+1; i++)
            fout << Row_indexation[i] << "\t";
        fout << "\n\nreachability matrix:\n";
        for (int i = 0; i < r_edges; i++)
            fout << R_Value[i] << "\t";
        fout << "\n";
        for (int i = 0; i < r_edges; i++)
            fout << R_Column_index[i] << "\t";
        fout << "\n";
        for (int i = 0; i < nodes+1; i++)
            fout << R_Row_indexation[i] << "\t";
        fout << "\n";
        fout.close();
    }
    else{
        cout << "adjacency matrix:\n";
        for (int i = 0; i < edges; i++)
            cout << "1\t";
        cout << "\n";
        for (int i = 0; i < edges; i++)
            cout << Column_index[i] << "\t";
        cout << "\n";
        for (int i = 0; i < nodes+1; i++)
            cout << Row_indexation[i] << "\t";
        cout << "\n\nreachability matrix:\n";
        for (int i = 0; i < r_edges; i++)
            cout << R_Value[i] << "\t";
        cout << "\n";
        for (int i = 0; i < r_edges; i++)
            cout << R_Column_index[i] << "\t";
        cout << "\n";
        for (int i = 0; i < nodes+1; i++)
            cout << R_Row_indexation[i] << "\t";
        cout << "\n";
    }
}
