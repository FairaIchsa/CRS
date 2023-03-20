class graph{
public:
    const int NUMBER_IN_GROUP = 8;
    int edges = 5 * (10 + NUMBER_IN_GROUP);  //количество дуг (используется при работе с матрицей смежности и транспонированной матрицей смежности)
    int nodes = 10 * (10 + NUMBER_IN_GROUP);  //количество вершин графа
    int **Edge; //Множество дуг (пары чисел)

    //массивы, описывающие матрицу смежности
    //int *Value; //значения (вообще, это все - единицы)
    int *Column_index; //индексы столбцов
    int *Row_indexation; //индексация строк

    //то же самое, но другое ("T" означает транспонирование)
    //int *T_Value; //вообще, это все - единицы
    int *T_Column_index;
    int *T_Row_indexation;

    //то же самое, но другое ("Pow" означает матрицу, возведенную в степень)
    int pow_edges;
    int *Pow_Value;
    int *Pow_Column_index;
    int *Pow_Row_indexation;

    //то же самое, но другое ("R" означает достижимость)
    bool repeat = 1;
    int number_of_iterarions = 1;
    int r_edges;
    int *R_Value;
    int *R_Column_index;
    int *R_Row_indexation;

    ~graph();
    graph(); //ввод дуг (массив Edges)
    void adjacency_matrix_crs(); //рассчет и вывод матрицы смежности
    void t_adjacency_matrix_crs(); //рассчет транспонированной матрицы смежности
    void multiplication_matrix_crs(); //умножение матрицы на транспонированную матрицу смежности
    void addition_matrix_crs(); //сложение матрицы достижимости с матрицей, полученной умножением
    void reachability_matrix_crs(); //объявление и расчет матрицы достижимости
    void matrix_crs_output(); //вывод
};
