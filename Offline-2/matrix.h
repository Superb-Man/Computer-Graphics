#include<bits/stdc++.h>

class Matrix{
    private:
        int row, col;
        double **mat;
        void alloc(int row, int col);
        void dealloc();
    public:
        Matrix();
        Matrix(int row , int col);
        Matrix(const Matrix &m);
        ~Matrix();
        bool set(int row, int col, double val);
        double get(int row, int col);
        void print();
        void convert2D(int row,int col,std::vector<double> &v);
        Matrix operator*(const Matrix &m);
        Matrix operator*(double val);
        Matrix operator=(const Matrix &m);
        Matrix operator*=(const Matrix &m);
        bool operator==(const Matrix &m);
        bool operator!=(const Matrix &m);

        bool makeColumn(int col ,const std::vector<double> &v);
        bool makeRow(int row ,const std::vector<double> &v);
        // friend std::ostream& operator<<(std::ostream &out, const Matrix &m);
        // friend std::istream& operator>>(std::istream &in, Matrix &m);
} ;