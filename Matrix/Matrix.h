#ifndef _MATRIX_
#define _MATRIX_
#include <iostream>
#include <typeinfo>
#define OUT_OF_RANGE 1
#define WRONG_DIM 2

template<typename T>
class Matrix {
public:
	class MAT_EXCEPTION : std::exception{
		int err;
		const char *message;

	public:
		MAT_EXCEPTION(int e, std::string mes){
			err = e;
			message = mes.c_str();
		}
		const char *getMessage(){
			return message;
		}
		const char *what(){
			return message;
		}
		
		int getErrorCode(){
			return err;
		}
	};
private:
	struct _Cell{
		_Cell *left, *right, *up, *down;
		T value;
		_Cell(T value=0);
	};
	class Row{
		_Cell *row;
	public:
		Row(_Cell *r){
			row = r;
		}
		T & operator [](int col){
			if(col < 0)
				throw MAT_EXCEPTION(OUT_OF_RANGE, "Out of range exception(no negative values alowed).");
			_Cell *temp = row;
			while(col && temp != 0){
				temp = temp->right;
				col--;
			}
			T t = 0;
			if(temp)
				return (temp->value);
			else
				throw MAT_EXCEPTION(OUT_OF_RANGE, "Out of range exception.");
		}
		operator T(){return row->value;}
	};
	_Cell *getCell(int y, int x){
		_Cell *temp = getCellY(y, mat);
		temp = getCellX(x, temp);
		return temp;
	}
	_Cell *getCellX(int x, _Cell *beg){
		_Cell *temp = beg;
		while(x && temp != 0){
			temp = temp->right;
			x--;
		}
		return temp;
	}
	_Cell *getCellY(int y, _Cell *beg){
		_Cell *temp = beg;
		while(y && temp != 0){
			temp = temp->down;
			y--;
		}
		return temp;
	}
public:
	Row operator [](int row){
		return Row(getCellY(row, mat));
	}

	Matrix<T> operator*(const Matrix<T> &m){
		return mult(m);
	}
	Matrix<T> mult(const Matrix<T> &m);
	Matrix<T> operator*(const T &s){
		return mult(s);
	}
	Matrix<T> mult(const T &s);
	T getValue(int y, int x);
	int getRows(){
		return max_y;
	}
	int getColumns(){
		return max_x;
	}
	Matrix<T> & setColumn(int col, T *vals);
	Matrix<T> & setRow(int col, T *vals);

	Matrix<T> & addColumn();
	Matrix<T> & removeColumn(int c = 0);
	Matrix<T> & addRow();
	Matrix<T> & removeRow(int r = 0);
	/// TODO: add row operations here
	Matrix<T> & multRow(int i, T s);
	Matrix<T> & addToRow(int i, int r, T factor = 1);
	Matrix<T> & addToRow(Matrix<T> &i, int r);
	Matrix<T> & switchRow(int i, int r);

	Matrix<T> operator^(T &s){
		return exp(s);
	}
	T det(){
		if(max_x == max_y && max_x == 2){
			return (mat->value * mat->right->down->value) - (mat->down->value * mat->right->value);
		}
		T d = 0;
		for(int i=0; i < max_x; i++){
			Matrix<T> next(*this);
			next.removeRow(0);
			next.removeColumn(i);

			 // negate if even number
			d -= ((i%2 + 1)*2 - 3) * next.det() * (*this)[0][i];
		}
		return d;
	}
	Matrix<T> invert();
	Matrix<T> exp(T s);
	Matrix<T> operator^(const char &s);
	Matrix<T> transposed();
	// copy
	Matrix(const Matrix<T>& m);
	// assignment
	Matrix<T> & operator=(const Matrix<T> &m);
	Matrix(int is=0);
	Matrix(int i, int j);
	~Matrix();

private:
	_Cell *mat;	// pointer to first cell
	int _currentX, _currentY;
	int max_x, max_y;
	void _init (const Matrix<T> *m);
};

#include "Matrix.cpp"


#endif