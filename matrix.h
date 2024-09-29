#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <QtMath>

class QSMatrix {
private:
    std::vector<std::vector<qreal> > mat;
    unsigned rows;
    unsigned cols;

public:
    QSMatrix(unsigned _rows, unsigned _cols, const qreal& _initial);
    QSMatrix(const QSMatrix& rhs);
    virtual ~QSMatrix();

    // Operator overloading, for "standard" mathematical matrix operations
    QSMatrix& operator=(const QSMatrix& rhs);

    // Matrix mathematical operations
    QSMatrix operator+(const QSMatrix& rhs);
    QSMatrix& operator+=(const QSMatrix& rhs);
    QSMatrix operator-(const QSMatrix& rhs);
    QSMatrix& operator-=(const QSMatrix& rhs);
    QSMatrix operator*(const QSMatrix& rhs);
    QSMatrix& operator*=(const QSMatrix& rhs);
    QSMatrix transpose();

    // Matrix/scalar operations
    QSMatrix operator+(const qreal& rhs);
    QSMatrix operator-(const qreal& rhs);
    QSMatrix operator*(const qreal& rhs);
    QSMatrix operator/(const qreal& rhs);

    // Matrix/vector operations
    std::vector<qreal> operator*(const std::vector<qreal>& rhs);
    std::vector<qreal> diag_vec();

    // Access the individual elements
    qreal& operator()(const unsigned& row, const unsigned& col);
    const qreal& operator()(const unsigned& row, const unsigned& col) const;

    // Access the row and column sizes
    unsigned get_rows() const;
    unsigned get_cols() const;

    qreal determinant();
    QSMatrix mergeMatrix(const QSMatrix& rhs);
    QSMatrix getMatrixWithOut(unsigned int row, unsigned int col);
    QSMatrix getReverse();

private:
    qreal reqursiveDeterminant(QSMatrix matrix, unsigned int n);
};

#endif // MATRIX_H
