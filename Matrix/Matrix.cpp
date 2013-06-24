//#include "Matrix.h"

/* This is constructors */
template <typename T>
Matrix<T>::Matrix(int is){ 
	max_x = max_y = is;
	_init(0);
}
template <typename T>
Matrix<T>::Matrix(int i, int j){ 
	max_y = i;
	max_x = j;
	_init(0);
}
template <typename T>
Matrix<T>::~Matrix(){
	if(mat == 0)return;
	bool goRight = true;
	_Cell *temp = mat;
	for(int i = 0;i < max_y || temp != 0;i++){
		for(int j = 0;j < max_x || temp != 0; j++){
			//std::cout << i << j << "\n";
			_Cell *delPart = temp;
			if(goRight && temp->right != 0)
				temp = temp->right;
			else if(!goRight && temp->left != 0)
				temp = temp->left;
			if(temp == delPart){
				temp = temp->down;
				delete delPart;
				break;
			}
			delete delPart;
		}
		//if(temp->down != 0)
		//	temp = temp->down;
		goRight = !goRight;
	}
	mat = 0;
}
// copy
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& m){
	max_x = m.max_x;
	max_y = m.max_y;
	_init(&m);
}
// assignment
template <typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix<T> &m){
	if( this != &m ){
		// make same size of m and this;
		while(max_x != m.max_x){
			if(max_x > m.max_x)
				removeColumn(max_x-1);
			else
				addColumn();
		}
		while(max_y != m.max_y){
			if(max_y > m.max_y)
				removeRow(max_y-1);
			else
				addRow();
		}
		if(max_y == 0 || max_x == 0 || m.max_x == 0 || m.max_y == 0) return *this;

		// Make a copy here:
		_Cell *tempT = mat,
			   *tempM = m.mat;
		bool goRight = true;
		while(tempT != 0 && tempM != 0){
			(*tempT).value = (*tempM).value;
			if(goRight){
				if(tempT->right == 0 || tempM->right == 0 ){
					tempT = tempT->down;
					tempM = tempM->down;
					goRight = !goRight;
				}
				else{
					tempT = tempT->right;
					tempM = tempM->right;
				} 
			}
			else{
				if(tempT->left == 0 || tempM->left == 0 ){
					tempT = tempT->down;
					tempM = tempM->down;
					goRight = !goRight;	
				}
				else{
					tempT = tempT->left;
					tempM = tempM->left;
				}
			}
		}
	}
	return (*this);
}

/* This is _Cell class funktions */
template <typename T>
Matrix<T>::_Cell::_Cell(T value){
	left = right = up = down = 0;
	this->value = value;
}

/* This is Matrix functions public: */

template <typename T>
Matrix<T> Matrix<T>::exp(T s){
	if(max_y == 0 || max_x == 0) return *this;
	int rows = getRows(), cols = getColumns();
	Matrix<T> res(*this);
	if(s < 0){
		s *= -1;
		res = res.invert();
	}
	while(--s)
		res = res * res;
	return res;
}
template <typename T>
Matrix<T> Matrix<T>::invert(){
	if(max_y == 0 || max_x == 0 || max_y != max_x) return *this;
	int rows = getRows(), cols = getColumns();
	Matrix<T> res(rows, cols),
			  temp(*this);
	/*for(int i=0;i<cols; i++){
		res.addColumn();
		if(i < rows)
			res[i][i+cols] = 1;
	}*/
	for(int r=0;r<rows; r++){
		int cr = r;
		T val = temp[cr][cr];
		if(val == 0){
			res.multRow(cr, 0);
			temp.multRow(cr, 0);
		}
		else{
			res.multRow(cr, 1/val);
			temp.multRow(cr, 1/val);
		}
		for(int i = cr+1; i<rows; i++){
			val = temp[i][r];
			res.addToRow(r, i, -val);
			temp.addToRow(r, i, -val);
		}
	}
	for(int r=0;r<rows; r++){
		for(int i = r-1; i>=0; i--){
			T val = temp[i][r];
			res.addToRow(r, i, -val);
			temp.addToRow(r, i, -val);
		}
	}
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(const char &s){
	if(typeid(T) != typeid(char) && s == 'T'){
		return transposed();
	}
	return exp(s);
}
template <typename T>
Matrix<T> Matrix<T>::transposed(){
	int rows = getRows(), cols = getColumns();
	Matrix<T> res(cols, rows);
	for(int i=0; i<rows;i++)
		for(int j=0; j<cols;j++)
			res[j][i] = getValue(i,j);
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::mult(const Matrix<T> &m){
	if(getColumns() != m.max_y)
		throw MAT_EXCEPTION(WRONG_DIM, "Wrong dimension in matrix-multiplikation.");
	int rows = getRows(), cols = m.max_x;
	Matrix<T> res(rows, cols);

	_Cell *firstM = m.mat, 
		  *firstT = mat, 
		  *firstR = res.mat;
	int cx = 0, cy = 0;
	_Cell *tempT = firstT, 
		  *tempR = firstR, 
		  *tempM = firstM;
	for(int i=0;i<rows; i++){
		for(int j = 0; j < cols; j++){
			tempR = res.getCell(i,j);//getCellX(j,getCellY(i, firstR));
			tempT = this->getCellY(i, firstT);
			tempM = this->getCellX(j, firstM);
			tempR->value = 0;
			while(tempT != 0 && tempM != 0){
				tempR->value += tempM->value * tempT->value;
				tempM = tempM->down;
				tempT = tempT->right;
			}
		}
	}
	return res;
}
template <typename T>
Matrix<T> Matrix<T>::mult(const T &s){
	int rows = getRows(), cols = getColumns();
	Matrix<T> res(rows, cols);
	for(int i=0; i<rows;i++)
		for(int j=0; j<cols;j++)
			res[i][j] = s * getValue(i,j);
	return res;
}

template <typename T>
Matrix<T> & Matrix<T>::addColumn(){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = mat;
	while(temp->right != 0)
		temp = temp->right;
	max_x++;
	while(temp){
		_Cell *newC = new _Cell(0);
		temp->right = newC;
		newC->left = temp;
		if(temp->up){
			newC->up = temp->up->right;
			newC->up->down = newC;
		}

		temp = temp->down;
	}
	return *this;
}
template <typename T>
Matrix<T> & Matrix<T>::removeColumn(int c){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = mat;

	while(c-- && temp)
		temp = temp->right;
	if(temp != 0){
		max_x--;
		if(temp == mat)
			mat = temp->right;
		while(temp != 0){
			if( temp->left != 0 )
				temp->left->right = temp->right;
			if(temp->right != 0)
				temp->right->left = temp->left;
			_Cell *t = temp->down;
			delete temp;
			temp = t;
		}
	}

	return *this;
	while(temp->right != 0)
		temp = temp->right;
	while(temp){
		temp->left->right = 0;
		if(temp->down){
			temp = temp->down;
			delete temp->up;
		}
		else{
			delete temp;
			temp = 0;
		}
	}
	return *this;
}

template <typename T>
Matrix<T> & Matrix<T>::addRow(){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = mat;
	while(temp->down != 0)
		temp = temp->down;
	max_y++;
	while(temp){
		_Cell *newC = new _Cell(0);
		temp->down = newC;
		newC->up = temp;
		if(temp->left){
			newC->left = temp->left->down;
			newC->left->right = newC;
		}

		temp = temp->right;
	}
	return *this;
}
template <typename T>
Matrix<T> & Matrix<T>::removeRow(int r){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = mat;

	while(r-- && temp)
		temp = temp->down;
	if(temp != 0){
		max_y--;
		if(temp == mat)
			mat = temp->down;
		while(temp != 0){
			if( temp->up != 0 )
				temp->up->down = temp->down;
			if(temp->down != 0)
				temp->down->up = temp->up;
			_Cell *t = temp->right;
			delete temp;
			temp = t;
		}
	}

	return *this;
	if(max_y == 0 || max_x == 0) return *this;
	//_Cell *temp = mat;
	while(temp->down != 0)
		temp = temp->down;
	while(temp){
		temp->up->down = 0;
		if(temp->right){
			temp = temp->right;
			delete temp->left;
		}
		else{
			delete temp;
			temp = 0;
		}
	}
	max_y--;
	return *this;
}
template <typename T>
Matrix<T> & Matrix<T>::multRow(int i, T s){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *row = getCellY(i, mat);
	while(row){
		row->value *= s;

		row = row->right;
	}
	return *this;	
}
template <typename T>	//		add to, add from
Matrix<T> & Matrix<T>::addToRow(int i, int r, T factor){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *row = getCellY(r, mat),
		  *rad = getCellY(i, mat);
	while(row){
		row->value += rad->value * factor;

		row = row->right;
		rad = rad->right;
	}
	return *this;	
}
template <typename T>	//		add to, add from
Matrix<T> & Matrix<T>::addToRow(Matrix<T> &i, int r){
	if(max_y == 0 || max_x == 0 || i.max_x == 0 || i.max_y != 1) return *this;
	_Cell *row = getCellY(r, mat),
		  *rad = i.mat;
	while(row && rad){
		row->value += rad->value;

		row = row->right;
		rad = rad->right;
	}
	return *this;	
}
template <typename T>
Matrix<T> & Matrix<T>::switchRow(int i, int r){
	if(max_y == 0 || max_x == 0) return *this;
	if(i == r)return *this;
	_Cell *row = getCellY(i, mat),
		  *rad = getCellY(r, mat);
	while(row){
		T temp = row->value;
		row->value = rad->value;
		rad->value = temp;

		row = row->right;
		rad = rad->right;
	}
	return *this;
}

/* This is Matrix functions private: */

template <typename T>
void Matrix<T>::_init (const Matrix<T> *m){
	mat = 0;
	_currentY = _currentX = 0;
	//mat = new _Cell*[max_x*max_y];
	for(int i = 0;i < max_y;i++){
		for(int j = 0;j < max_x; j++){
			_Cell *ij = new _Cell(0), *lij;

			if(m != 0)
				ij->value = getCellX(j,getCellY(i,m->mat))->value;//m->getCell(i,j);//->value;
			else if(i == j && max_x == max_y)
				ij->value = 1;

			if(i == 0 && j == 0)
				mat = ij;
			// set ij to the relatives
			// set the relatives to ij
			lij = getCell(i, j-1);
			if(lij)
				lij->right = ij;
			ij->left = lij;

			lij = getCell(i, j+1);
			if(lij)
				lij->left = ij;
			ij->right = lij;
			
			lij = getCell(i-1, j);
			if(lij)
				lij->down = ij;
			ij->up = lij;

			lij = getCell(i+1, j);
			if(lij)
				lij->up = ij;
			ij->down = lij;
		}
	}
}

template<typename T>
T Matrix<T>::getValue(int y, int x){
	if(max_y == 0 || max_x == 0) return 0;
	_Cell *temp = getCellY(y, mat);
	temp = getCellX(x, temp);
	if(temp != 0)
		return temp->value;
	else
		return 0;
}
template<typename T>
Matrix<T> & Matrix<T>::setColumn(int col, T *vals){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = getCellX(col, mat);
	int i = 0;
	while(temp != 0){
		temp->value = vals[i];
		temp = temp->down;
		i++;
	}
	return *this;
}
template<typename T>
Matrix<T> & Matrix<T>::setRow(int col, T *vals){
	if(max_y == 0 || max_x == 0) return *this;
	_Cell *temp = getCellY(col, mat);
	int i = 0;
	while(temp != 0){
		temp->value = vals[i];
		temp = temp->right;
		i++;
	}
	return *this;
}

