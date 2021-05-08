#include<iostream>
#include<stdio.h>
#include <sstream>

#include <ilcplex/ilocplex.h>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

using namespace std;

int main() {
    cout << "Hello World" << endl;

    return 0;
}