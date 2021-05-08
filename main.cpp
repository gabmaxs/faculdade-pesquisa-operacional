#include<iostream>
#include<stdio.h>
#include <sstream>

#include <ilcplex/ilocplex.h>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

using namespace std;

int main(int argc, char *argv[])
{
    // declarando constantes do problema
    int limiteMaxBateria;
    int LimiteMinBateria;
    int capacidadeMercadoriasDeposito;

    // Declarando conjuntos de entrada
    int conjuntoArestas;
    int conjuntoNos;
    int conjuntoVeiculos;
    int conjuntoClientes;
    int conjuntoEstacoesCarregamento;

    // Dados de entrada dependentes dos conjuntos
    int* mercadoriaVeiculo;
    int* demandaCliente; // demanda de mercadoria por cliente
    int* bateriaVeiculo;
    int** custoTransporteEntreNos;
    int** distanciaEntreNos;

   
    // Leitura do Arquivo
    FILE* fp;
    fp = fopen(argv[1],"r");

    if( fp == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);    
    }
    
    // Leitura dos conjuntos

    // Definindo os dados de entrada de acordo com o tamanho dos conjuntos lidos



    // Após a declaração dos tamanhos dos dados de entrada, realizando a leitura
    
    // Impressão para Verificação dos dados
    

    // DECLARANDO O AMBIENTE E O MODELO MATEMATICO
    IloEnv env;
    IloModel modelo;
    // iniciando o modelo
    modelo = IloModel(env);
    

    
    /*
    // ---------------  DECLARAÇÃO DE VARIÁVEIS DE UMA DIMENSÃO    
    // parametros para declaração de variáveis: ambiente, tamanho, valor min, valor max, tipo (ILOINT, ILOFLOAT, ILOBOOL)
    IloNumVarArray y(env,TAM, 0, IloInfinity, ILOFLOAT);

    // adicionar as variáveis ao modelo
    for(int ti = 0; ti < TAM; ti++)
    {
        stringstream var;
        var << "y[" << t1 << "]";
        y[t].setName(var.str().c_str());
        modelo.add(y[t1]);
    }
    // ---------------  DECLARAÇÃO DE VARIÁVEIS DE DUAS DIMENSÕES
    // parâmetros quando não é IloNunVarArray: ambiente, tamanho
    IloNumVarMatrix k(env, TAM1);
    for(int t1 = 0 ; t1 < TAM1; t1++)
    {
        k[t1] = IloNumVarArray(env, TAM2, 0, IloInfinity, ILOFLOAT);
    }
    // adicionar as variáveis no modelo
    for(int t1 = 0 ; t1 < TAM1; t1++)
    {
        for(int t2 = 0; t2 < TAM2; t2++)
        {
            stringstream var;
            var << "k["<< t1 << "]["<< t2 << "]";
            k[t1][t2].setName(var.str().c_str());
            modelo.add(k[t1][t2]);
        }
    }
    */


    

    // DECLARAÇÃO DA FUNÇÃO OBJETIVO

    IloExpr fo(env);

    // Somatorio...
      
    //IloMinimize e IloMaximize
    modelo.add(IloMaximize(env, fo));

    
    // DECLARAÇÃO DAS RESTRIÇÕES DO PROBLEMA

    // declarando a restrição
    // IloRange parametros: ambiente, valor min, expressão, valor maximo
    

    // RESOLVENDO O MODELO

    // Carregando o módulo do Cplex
    IloCplex cplex(modelo);
    // exportando o lp
    cplex.exportModel("modelo.lp");
    // Executando o modelo
    cplex.solve();
    
    // PEGAR OS VALORES DAS VARIÁVEIS 
    //cplex.getValue(NOME_VAR)    


    return 0;
}