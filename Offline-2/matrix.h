#pragma once
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
        std::pair<int,int> getDimension();
        // friend std::ostream& operator<<(std::ostream &out, const Matrix &m);
        // friend std::istream& operator>>(std::istream &in, Matrix &m);
} ;




//Co-Pilot kichu likhte dicche na
//____ Loves Linux uWu
//I am Batman

void Matrix::alloc(int row, int col){
    this->row = row;
    this->col = col;
    mat = new double*[row];
    for(int i=0; i<row; i++){
        mat[i] = new double[col];
        for(int j=0; j<col; j++){
            mat[i][j] = (i == j ? 1 : 0);
        }
    }
}

void Matrix::dealloc(){
    for(int i=0; i<row; i++){
        delete[] mat[i];
    }
    delete[] mat;
}


Matrix::Matrix(){
    alloc(4, 4);
}
Matrix::Matrix(int row , int col){
    alloc(row, col);
}

Matrix::Matrix(const Matrix &m){
    alloc(m.row, m.col);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            mat[i][j] = m.mat[i][j];
        }
    }
}

Matrix::~Matrix(){
    dealloc();
}

void Matrix::convert2D(int row,int col,std::vector<double> &v){
    alloc(row,col);
    int k=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            mat[i][j]=v[k++];
        }
    }
}

bool Matrix::set(int row, int col, double val){
    if(row >= this->row || col >= this->col || row < 0 || col < 0){
        std::cout << "Index out of bound\n";
        return false;
    }
    mat[row][col] = val;
    return true;
}

double Matrix::get(int row, int col){
    if(row >= this->row || col >= this->col || row < 0 || col < 0){
        std::cout << "Index out of bound\n";
        return -1;
    }
    return mat[row][col];
}

void Matrix::print(){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            std::cout << mat[i][j] << " ";
        }
        std::cout << "\n";
    }
}

Matrix Matrix::operator*(double val){
    Matrix temp(row, col);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            temp.mat[i][j] = mat[i][j] * val;
        }
    }
    return temp ;
}

Matrix Matrix::operator=(const Matrix &m){
    dealloc();
    alloc(m.row, m.col);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            mat[i][j] = m.mat[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &m){
    if(col != m.row){
        std::cout << "Matrix multiplication not possible\n";
        return *this;
    }
    Matrix temp(row, m.col);
    for(int i=0; i<row; i++){
        for(int j=0; j<m.col; j++){
            temp.mat[i][j] = 0;
            for(int k=0; k<col; k++){
                temp.mat[i][j] += mat[i][k] * m.mat[k][j];
            }
        }
    }
    return temp ;
}

bool Matrix::operator==(const Matrix &m){
    if(row != m.row || col != m.col){
        return false;
    }
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            if(mat[i][j] != m.mat[i][j]){
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &m){
    return !(*this == m);
}

bool Matrix::makeColumn(int col ,const std::vector<double> &v){
    if(col >= this->col || v.size() < row || col < 0) {
        std::cout << "Invalid params\n";
        return false;
    }
    for(int i=0;i<row;i++){
        mat[i][col]=v[i];
    }
    return true;
}

bool Matrix::makeRow(int row ,const std::vector<double> &v){
    if(row >= this->row || v.size() < col || row < 0) {
        std::cout << "Invalid params\n";
        return false;
    }
    for(int i=0;i<col;i++){
        mat[row][i]=v[i];
    }
    return true;
}

std::pair<int,int> Matrix::getDimension(){
    return std::make_pair(row,col);
}


