#ifndef _FSA_MATRIX_
#define _FSA_MATRIX_
#include<vector>

#include<iostream>

using namespace std;

template<class ElementType>

class Matrix{
public:
	typedef vector<ElementType>  RowVector;
	typedef vector<ElementType>  LineVector;
	Matrix(const int &m,const int &n);

	Matrix(const int &m,const int &n,const ElementType propetype);

	void resize(unsigned int m,unsigned int  n);

	void AddRow();

	void AddRow(const RowVector &rowadd);
	
	inline RowVector &operator[](const int &rowindex);


	template<class T> friend ostream &operator<<(ostream &os,const Matrix<T> &matrix);

private:
	inline RowVector getRow(const int &rowindex);
	inline LineVector getLine(const int &lineindex);
	vector<RowVector>  matrix;
	int Rows;
	int Lines;
	ElementType defaultValue;
};
template<class T> Matrix<T>::Matrix(const int &m,const int &n):matrix(m),Rows(m),Lines(n)
{
	for(vector<RowVector>::iterator p=matrix.begin();p!=matrix.end();p++)
		p->resize(n);
};

template<class T> Matrix<T>::Matrix(const int &m,const int &n,const T propetype):matrix(m),Rows(m),Lines(n)
{
	for(vector<RowVector>::iterator p=matrix.begin();p!=matrix.end();p++)
		p->resize(n,propetype);
	defaultValue=propetype;
};
template<class T> void Matrix<T>::resize(unsigned int m,unsigned int n)
{
	matrix.resize(m);
	for(vector<RowVector>::iterator p=matrix.begin();p!=matrix.end();p++)
		p->resize(n,defaultValue);
	Rows=m;
	Lines=n;
}

template<class T> void Matrix<T>::AddRow()
{
	RowVector x;
	x.resize(Lines,defaultValue);
	matrix.push_back(x);
}
template<class T> void Matrix<T>::AddRow(const RowVector &rowadd)
{
	matrix.push_back(rowadd);
}
template<class T> typename Matrix<T>::RowVector & Matrix<T>::operator[](const int &rowindex)
{	
	return matrix.at(rowindex);
};

template<class T> ostream& operator<<(ostream& os,const Matrix<T> &matrix)
{
	for( vector<typename Matrix<T>::RowVector>::const_iterator p=matrix.matrix.begin();p!=matrix.matrix.end();p++)
	{
		for(typename Matrix<T>::RowVector::const_iterator q=p->begin();q!=p->end();q++)
			os<<*q<<" ";
		os<<endl;
	}
	return os;
};
template<class T> typename Matrix<T>::RowVector Matrix<T>::getRow(const int &rowindex)
{
	return matrix.at(rowindex);
};
template<class T> typename Matrix<T>::LineVector Matrix<T>::getLine(const int &lineindex)
{
	LineVector result;
	for(unsigned int i=0;i<Rows;i++)
		result.pushback(matrix[i][lineindex])
	return result;
}

#endif 