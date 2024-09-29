#ifndef __QS_MATRIX_CPP
#define __QS_MATRIX_CPP

#include "matrix.h"

// Parameter Constructor
QSMatrix::QSMatrix(unsigned _rows, unsigned _cols, const qreal& _initial) {
    mat.resize(_rows);
    for (unsigned i=0; i<mat.size(); i++) {
        mat[i].resize(_cols, _initial);
    }
    rows = _rows;
    cols = _cols;
}

// Copy Constructor
QSMatrix::QSMatrix(const QSMatrix& rhs) {
    mat = rhs.mat;
    rows = rhs.get_rows();
    cols = rhs.get_cols();
}

// (Virtual) Destructor
QSMatrix::~QSMatrix() {}

// Assignment Operator
QSMatrix& QSMatrix::operator=(const QSMatrix& rhs) {
    if (&rhs == this)
        return *this;

    unsigned new_rows = rhs.get_rows();
    unsigned new_cols = rhs.get_cols();

    mat.resize(new_rows);
    for (unsigned i=0; i<mat.size(); i++) {
        mat[i].resize(new_cols);
    }

    for (unsigned i=0; i<new_rows; i++) {
        for (unsigned j=0; j<new_cols; j++) {
            mat[i][j] = rhs(i, j);
        }
    }
    rows = new_rows;
    cols = new_cols;

    return *this;
}

// Addition of two matrices
QSMatrix QSMatrix::operator+(const QSMatrix& rhs) {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] + rhs(i,j);
        }
    }

    return result;
}

// Cumulative addition of this matrix and another
QSMatrix& QSMatrix::operator+=(const QSMatrix& rhs) {
    unsigned rows = rhs.get_rows();
    unsigned cols = rhs.get_cols();

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            this->mat[i][j] += rhs(i,j);
        }
    }

    return *this;
}

// Subtraction of this matrix and another
QSMatrix QSMatrix::operator-(const QSMatrix& rhs) {
    unsigned rows = rhs.get_rows();
    unsigned cols = rhs.get_cols();
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] - rhs(i,j);
        }
    }

    return result;
}

// Cumulative subtraction of this matrix and another
QSMatrix& QSMatrix::operator-=(const QSMatrix& rhs) {
    unsigned rows = rhs.get_rows();
    unsigned cols = rhs.get_cols();

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            this->mat[i][j] -= rhs(i,j);
        }
    }

    return *this;
}

// Left multiplication of this matrix and another
QSMatrix QSMatrix::operator*(const QSMatrix& rhs) {
    unsigned rows = rhs.get_rows();
    unsigned cols = rhs.get_cols();
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            for (unsigned k=0; k<rows; k++) {
                result(i,j) += this->mat[i][k] * rhs(k,j);
            }
        }
    }

    return result;
}

// Cumulative left multiplication of this matrix and another
QSMatrix& QSMatrix::operator*=(const QSMatrix& rhs) {
    QSMatrix result = (*this) * rhs;
    (*this) = result;
    return *this;
}

// Calculate a transpose of this matrix
QSMatrix QSMatrix::transpose() {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[j][i];
        }
    }

    return result;
}

// Matrix/scalar addition
QSMatrix QSMatrix::operator+(const qreal& rhs) {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] + rhs;
        }
    }

    return result;
}

// Matrix/scalar subtraction
QSMatrix QSMatrix::operator-(const qreal& rhs) {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] - rhs;
        }
    }

    return result;
}

// Matrix/scalar multiplication
QSMatrix QSMatrix::operator*(const qreal& rhs) {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] * rhs;
        }
    }

    return result;
}

// Matrix/scalar division
QSMatrix QSMatrix::operator/(const qreal& rhs) {
    QSMatrix result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = this->mat[i][j] / rhs;
        }
    }

    return result;
}

// Multiply a matrix with a vector
std::vector<qreal> QSMatrix::operator*(const std::vector<qreal>& rhs) {
    std::vector<qreal> result(rhs.size(), 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result[i] = this->mat[i][j] * rhs[j];
        }
    }

    return result;
}

// Obtain a vector of the diagonal elements
std::vector<qreal> QSMatrix::diag_vec() {
    std::vector<qreal> result(rows, 0.0);

    for (unsigned i=0; i<rows; i++) {
        result[i] = this->mat[i][i];
    }

    return result;
}

// Access the individual elements
qreal& QSMatrix::operator()(const unsigned& row, const unsigned& col) {
    return this->mat[row][col];
}

// Access the individual elements (const)
const qreal& QSMatrix::operator()(const unsigned& row, const unsigned& col) const {
    return this->mat[row][col];
}

// Get the number of rows of the matrix
unsigned QSMatrix::get_rows() const {
    return this->rows;
}

// Get the number of columns of the matrix
unsigned QSMatrix::get_cols() const {
    return this->cols;
}

qreal QSMatrix::determinant()
{
    if (rows != cols) return 0;
    else return reqursiveDeterminant(*this, rows);
}

QSMatrix QSMatrix::mergeMatrix(const QSMatrix& rhs)
{
    QSMatrix result(rows, cols * 2, 0);

    for (unsigned int i{}; i < rows; ++i) {
        for (unsigned int j{}; j < cols; ++j) {

            result(i, j) = operator()(i, j);
            result(i, j + cols) = rhs(i, j);
        }
    }

    return result;
}

QSMatrix QSMatrix::getMatrixWithOut(unsigned int row, unsigned int col)
{
    unsigned int di, dj;
    QSMatrix result(rows - 1, cols - 1, 0);

    di = 0;
    for (unsigned int i{}; i < rows - 1; ++i) {
        if (i == row) di = 1;

        dj = 0;

        for (unsigned int j{}; j < cols - 1; ++j) {
            if (j == col) dj = 1;

            result(i, j) = operator()(i + di, j + dj);
        }
    }

    return result;
}

QSMatrix QSMatrix::getReverse()
{
    QSMatrix result(rows, cols, 0);

    qreal det = determinant();

    for (unsigned int i{}; i < rows; ++i) {
        for (unsigned int j{}; j < cols; ++j) {

            result(i, j) = getMatrixWithOut(i, j).determinant();

            if ((i + j) % 2 == 1){
                result(i, j) = - result(i, j);
            }

            result(i, j) = result(i, j) / det;
        }
    }

    result = result.transpose();

    return result;
}

qreal QSMatrix::reqursiveDeterminant(QSMatrix matrix, unsigned int n)
{
    qreal det = 0.0;
    int subi = 0;
    int subj = 0;
    QSMatrix subMatrix(matrix.get_rows(), matrix.get_cols(), 0.0);

    if (n == 1) {
        return matrix(0, 0);
    }
    else if (n == 2) {
        return ((matrix(0, 0) * matrix(1, 1)) - (matrix(1, 0) * matrix(0, 1)));
    }
    else {
        for (unsigned int x{}; x < n; ++x) {
            subi = 0;
            for (unsigned int i{1}; i < n; ++i) {
                subj = 0;
                for (unsigned int j{0}; j < n; ++j) {
                    if (j == x) continue;

                    subMatrix(subi, subj) = matrix(i, j);
                    subj++;
                }
                subi++;
            }
            det = det + (qPow(-1, x) * mat[0][x] * reqursiveDeterminant(subMatrix, n - 1));
        }
    }

    return det;
}

#endif
