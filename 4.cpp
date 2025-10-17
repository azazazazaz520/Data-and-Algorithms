#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <tuple>
using namespace std;

// 使用CSR格式存储稀疏矩阵
struct SparseMatrix
{
    int m;                   // 矩阵维度
    vector<double> values;   // 非零元素值
    vector<int> col_indices; // 列索引
    vector<int> row_ptr;     // 行指针
};

// 读取稀疏矩阵
SparseMatrix readSparseMatrix()
{
    int m, k;
    scanf("%d %d", &m, &k);

    SparseMatrix matrix;
    matrix.m = m;

    // 临时存储所有非零元素
    vector<tuple<int, int, double>> elements(k);
    for (int i = 0; i < k; i++)
    {
        int row, col;
        double value;
        scanf("%d %d %lf", &row, &col, &value);
        elements[i] = make_tuple(row, col, value);
    }

    // 按行排序
    sort(elements.begin(), elements.end(),
         [](const tuple<int, int, double> &a, const tuple<int, int, double> &b)
         {
             if (get<0>(a) != get<0>(b))
                 return get<0>(a) < get<0>(b);
             return get<1>(a) < get<1>(b);
         });

    // 构建CSR格式
    matrix.row_ptr.resize(m + 1, 0);
    matrix.values.resize(k);
    matrix.col_indices.resize(k);

    int current_row = -1;
    for (int i = 0; i < k; i++)
    {
        int row = get<0>(elements[i]);
        int col = get<1>(elements[i]);
        double val = get<2>(elements[i]);

        // 更新行指针
        while (current_row < row)
        {
            current_row++;
            matrix.row_ptr[current_row] = i;
        }

        matrix.values[i] = val;
        matrix.col_indices[i] = col;
    }

    // 设置最后一行指针
    for (int i = current_row + 1; i <= m; i++)
    {
        matrix.row_ptr[i] = k;
    }

    return matrix;
}

// 读取向量b
vector<double> readVector(int m)
{
    vector<double> b(m);
    for (int i = 0; i < m; i++)
    {
        scanf("%lf", &b[i]);
    }
    return b;
}

// 高斯-赛德尔迭代求解
vector<double> gaussSeidel(const SparseMatrix &A, const vector<double> &b,
                           double tolerance = 0.1, int max_iterations = 1000)
{
    int m = A.m;
    vector<double> x(m, 0.0); // 初始解设为0
    vector<double> x_old(m, 0.0);

    for (int iter = 0; iter < max_iterations; iter++)
    {
        x_old = x;

        // 高斯-赛德尔迭代
        for (int i = 0; i < m; i++)
        {
            double sum = 0.0;
            double diag = 0.0;

            // 遍历第i行的非零元素
            for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; j++)
            {
                int col = A.col_indices[j];
                if (col == i)
                {
                    diag = A.values[j]; // 对角线元素
                }
                else
                {
                    sum += A.values[j] * x[col];
                }
            }

            if (diag != 0.0)
            {
                x[i] = (b[i] - sum) / diag;
            }
        }

        // 检查收敛条件（计算残差）
        double residual = 0.0;
        for (int i = 0; i < m; i++)
        {
            double Ax_i = 0.0;
            for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; j++)
            {
                Ax_i += A.values[j] * x[A.col_indices[j]];
            }
            residual += (Ax_i - b[i]) * (Ax_i - b[i]);
        }
        residual = sqrt(residual);

        if (residual < tolerance)
        {
            break;
        }
    }

    return x;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    // 读取输入
    SparseMatrix A = readSparseMatrix();
    vector<double> b = readVector(A.m);

    // 求解方程组
    vector<double> x = gaussSeidel(A, b);

    // 输出结果，提高精度
    cout << fixed << setprecision(10);
    for (double val : x)
    {
        cout << val << endl;
    }

    return 0;
}