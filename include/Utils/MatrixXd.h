//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2021
//* https://github.com/yangbai90/AsFem.git
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++ Author   : Yang Bai
//+++ Date     : 2020.10.18
//+++ Reviewer : Xiaoyuan @ 2021.08.20
//+++ Purpose  : Define the general Matrix  in AsFem
//+++            we mainly use this for the calculation of jacobian
//+++            If one wants to use Eigen's MatrixXd, please use
//+++            Eigen::MatrixXd, which is different with ours !!!
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "Eigen/Eigen"

#include "Utils/MessagePrinter.h"
#include "Utils/VectorXd.h"

using namespace std;

/**
 * This class implement the general matrix calculation, it shoul be noted that the vector
 * should be one of the special case of this class
 */
class MatrixXd{
public:
    /**
     * Constructor function for different purpose
     */
    MatrixXd();
    MatrixXd(const MatrixXd &a);
    MatrixXd(const int &m,const int &n);
    MatrixXd(const int &m,const int &n,const double &val);
    
    /**
     * Resize the matrix, the memory is reallocated, the matrix is set to zero by default
     * @param m integer, the size of the 1st dimention
     * @param n integra, the size of the 2nd dimention
     */
    void Resize(const int &m,const int &n){
        _vals.resize(m*n,0.0);_M=m;_N=n;_MN=m*n;
    }

    /**
     * Resize the matrix with a initial value, the memory is reallocated
     * @param m integer, the size of the 1st dimention
     * @param n integra, the size of the 2nd dimention
     * @param val the initial value for the resized matrix
     */
    void Resize(const int &m,const int &n,const double &val){
        _vals.resize(m*n,val);_M=m;_N=n;_MN=m*n;
    }

    /**
     * Return the pointer of the matrix's data (its a vector<double> type)
     */
    double* GetDataPtr(){
        return _vals.data();
    }

    /**
     * Return the size of the 1st dimension
     */
    inline int GetM()const{return _M;}
    /**
     * Return the size of the 2nd dimension
     */
    inline int GetN()const{return _N;}
    /**
     * Clean the whole matrix data
     */
    void Clean(){_vals.clear();}
    //*****************************************
    //*** Operator overload
    //*****************************************
    /**
     * The () operator for the data access
     * @param i the index of the 1st dimension, it should start from 1, not 0!!!
     * @param j the index of the 2nd dimension, it should start from 1, not 0!!!
     */
    inline double& operator()(const int &i,const int &j){
        return _vals[(i-1)*_N+j-1];
    }
    /**
     * The () operator for the data access with constant reference(not editable!)
     * @param i the index of the 1st dimension, it should start from 1, not 0!!!
     * @param j the index of the 2nd dimension, it should start from 1, not 0!!!
     */
    inline double operator()(const int &i,const int &j)const{
        return _vals[(i-1)*_N+j-1];
    }
    /**
     * The [] operator, the data of our matrix is just simple vector in 1D
     * @param i the index of the data vector element, it should start from 1, not 0!!!
     */
    inline double& operator[](const int &i){
        return _vals[i-1];
    }
    /**
     * The [] operator with constant reference, the data of our matrix is just simple vector in 1D
     * @param i the index of the data vector element, it should start from 1, not 0!!!
     */
    inline double operator[](const int &i)const{
        return _vals[i-1];
    }
    //*****************************************
    //*** For basic mathematic operator
    //*****************************************
    //*** for =
    /**
     * The '=' for equal operator
     * @param val the double type value to set up the whole matrix
     */
    inline MatrixXd& operator=(const double &val){
        fill(_vals.begin(),_vals.end(),val);
        return *this;
    }
    /**
     * The '=' for equal operator between two (same) matrix
     * @param a the right-hand side matrix with the same dimensions as current one
     */
    inline MatrixXd& operator=(const MatrixXd &a){
        if(_M==0&&_N==0){
            _M=a.GetM();_N=a.GetN();
            _MN=_M*_N;_vals.resize(_MN,0.0);
            for(int i=0;i<_MN;++i) _vals[i]=a._vals[i];
            return *this;
        }
        else{
            if(_M==a.GetM()&&_N==a.GetN()){
                for(int i=0;i<_MN;++i) _vals[i]=a._vals[i];
                return *this;
            }
            else{
                MessagePrinter::PrintErrorTxt("a=b cant be applied for two matrix with different size");
                MessagePrinter::AsFem_Exit();
            }

        }
        return *this;
    }
    //****************************
    //*** for +
    /**
     * The '+' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd operator+(const double &val)const{
        MatrixXd temp(_M,_N);
        for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]+val;
        return temp;
    }
    /**
     * The '+' operator between matrix and matrix
     * @param a the right-hand side matrix (the dimensions should be the same)
     */
    inline MatrixXd operator+(const MatrixXd &a)const{
        MatrixXd temp(_M,_N);
        if(_M==a.GetM()&&_N==a.GetN()){
            for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]+a._vals[i];
            return temp;
        }
        else{
            MessagePrinter::PrintErrorTxt("a+b cant be applied for two matrix with different size");
            MessagePrinter::AsFem_Exit();
        }

        return temp;
    }
    //*** for +=
    /**
     * The '+=' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd& operator+=(const double &val){
        for(int i=0;i<_MN;++i) _vals[i]=_vals[i]+val;
        return *this;
    }
    /**
     * The '+=' operator between matrix and matrix
     * @param a the right-hand side matrix (the dimensions should be the same)
     */
    inline MatrixXd& operator+=(const MatrixXd &a){
        MatrixXd temp(_M,_N);
        if(_M==a.GetM()&&_N==a.GetN()){
            for(int i=0;i<_MN;++i) _vals[i]=_vals[i]+a._vals[i];
            return *this;
        }
        else{
            MessagePrinter::PrintErrorTxt("a+b cant be applied for two matrix with different size");
            MessagePrinter::AsFem_Exit();
        }

        return *this;
    }
    //****************************
    //*** for -
    /**
     * The '-' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd operator-(const double &val)const{
        MatrixXd temp(_M,_N);
        for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]-val;
        return temp;
    }
    /**
     * The '-' operator between matrix and matrix
     * @param a the right-hand side matrix (the dimensions should be the same)
     */
    inline MatrixXd operator-(const MatrixXd &a)const{
        MatrixXd temp(_M,_N);
        if(_M==a.GetM()&&_N==a.GetN()){
            for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]-a._vals[i];
            return temp;
        }
        else{
            MessagePrinter::PrintErrorTxt("a-b cant be applied for two matrix with different size");
            MessagePrinter::AsFem_Exit();
        }

        return temp;
    }
    //*** for -=
    /**
     * The '-=' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd& operator-=(const double &val){
        for(int i=0;i<_MN;++i) _vals[i]=_vals[i]-val;
        return *this;
    }
    /**
     * The '-=' operator between matrix and matrix
     * @param a the right-hand side matrix (the dimensions should be the same)
     */
    inline MatrixXd& operator-=(const MatrixXd &a){
        MatrixXd temp(_M,_N);
        if(_M==a.GetM()&&_N==a.GetN()){
            for(int i=0;i<_MN;++i) _vals[i]=_vals[i]-a._vals[i];
            return *this;
        }
        else{
            MessagePrinter::PrintErrorTxt("a-b cant be applied for two matrix with different size");
            MessagePrinter::AsFem_Exit();
        }

        return *this;
    }
    //****************************
    //*** for *
    /**
     * The '*' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd operator*(const double &val)const{
        MatrixXd temp(_M,_N);
        for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]*val;
        return temp;
    }
    /**
     * The '*' operator between matrix and vector
     * @param a the right-hand side vector (the dimensions should be the same)
     */
    inline VectorXd operator*(const VectorXd &a)const{
        VectorXd temp(_M,0.0);
        if(_N!=a.GetM()){
            MessagePrinter::PrintErrorTxt("A*b should be applied for A matrix has the same cols as b vector!");
            MessagePrinter::AsFem_Exit();
        }
        else{
            for(int i=1;i<=_M;i++){
                temp(i)=0.0;
                for(int j=1;j<=_N;j++){
                    temp(i)+=(*this)(i,j)*a(j);
                }
            }
            return temp;
        }
        return temp;
    }
    /**
     * The '*' operator between matrix and matrix
     * @param a the right-hand side matrix (the dimensions should be the same)
     */
    inline MatrixXd operator*(const MatrixXd &a)const{
        MatrixXd temp(_M,a.GetN());
        if(_N!=a.GetM()){
            MessagePrinter::PrintErrorTxt("A*B should be applied for A matrix has the same cols as the rows of B matrix!");
            MessagePrinter::AsFem_Exit();
        }
        else{
            for(int i=1;i<=_M;i++){
                for(int j=1;j<=a.GetN();j++){
                    temp(i,j)=0.0;
                    for(int k=1;k<=a.GetM();k++){
                        temp(i,j)+=(*this)(i,k)*a(k,j);
                    }
                }
            }
            return temp;
        }
        return temp;
    }
    //*** for *=
    /**
     * The '*=' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd& operator*=(const double &val){
        for(int i=0;i<_MN;++i) _vals[i]=_vals[i]*val;
        return *this;
    }
    //****************************
    //*** for /
    /**
     * The '/' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd operator/(const double &val)const{
        MatrixXd temp(_M,_N);
        for(int i=0;i<_MN;++i) temp._vals[i]=_vals[i]/val;
        return temp;
    }
    //*** for /=
    /**
     * The '/=' operator between matrix and scalar
     * @param val the right-hand side scalar (double type)
     */
    inline MatrixXd& operator/=(const double &val){
        for(int i=0;i<_MN;++i) _vals[i]=_vals[i]/val;
        return *this;
    }
    /**
     * This function will set the whole matrix to zero
     */
    void setZero(){
        fill(_vals.begin(),_vals.end(),0.0);
    }
    /**
     * This function will set each element of the matrix to be random value
     */
    void setRandom(){
        srand(time(0));
        for(int i=0;i<_MN;++i) _vals[i]=static_cast<double>(1.0*rand()/RAND_MAX);
    }
    /**
     * This function return the inverse matrix of current one, it should be noted
     * this function will not change the value of current matrix
     */
    inline MatrixXd Inverse()const{
        if(_M!=_N){
            MessagePrinter::PrintErrorTxt("the inverse operation only works for square matrix");
            MessagePrinter::AsFem_Exit();
        }
        Eigen::MatrixXd Mat(_M,_N),MatInv(_M,_N);
        MatrixXd temp(_M,_N);

        for(int i=1;i<=_M;i++){
            for(int j=1;j<=_N;j++){
                Mat.coeffRef(i-1,j-1)=(*this)(i,j);
            }
        }
        MatInv=Mat.inverse();
        for(int i=1;i<=_M;i++){
            for(int j=1;j<=_N;j++){
                temp(i,j)=MatInv.coeff(i-1,j-1);
            }
        }
        return temp;
    }
    /**
     * This function return the determinant of the current matrix
     */
    inline double Det()const{
        Eigen::MatrixXd Mat(_M,_N);
        for(int i=1;i<=_M;i++){
            for(int j=1;j<=_N;j++){
                Mat.coeffRef(i-1,j-1)=(*this)(i,j);
            }
        }
        return Mat.determinant();
    }
    /**
     * This function return the transporse matrix of current one,
     * the current matrix will not be changed
     */
    inline MatrixXd Transpose() const{
        MatrixXd temp(_N,_M);
        for(int i=1;i<=_M;i++){
            for(int j=1;j<=_N;j++){
                temp(j,i)=(*this)(i,j);
            }
        }
        return temp;
    }
    /**
     * This function return the transposed matrix, important, the current matrix will
     * be transposed, if you don't want to use this one, then please call Transpose()
     */
    inline void Transposed(){
        MatrixXd temp=(*this).Transpose();
        (*this).Resize(temp.GetM(),temp.GetN());
        (*this)=temp;
    }

private:
    vector<double> _vals;/**< double type vector to store the matrix element*/
    int _M; /**< the integer variable for the 1st dimension of the matrix*/
    int _N; /**< the integer variable for the 2nd dimension of the matrix*/
    int _MN;/**< the integer variable for the total length of the matrix*/
};
