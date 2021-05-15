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
    int limiteMaxBateria = 0.8;
    int limiteMinBateria = 0.2;

    // Declarando conjuntos de entrada
    int conjuntoArestas;
    int conjuntoNos;
    int conjuntoVeiculos;
    int conjuntoClientes;
    int conjuntoEstacoesCarregamento;
    int conjuntoDepositos; // vai ser sempre 1?

    // Dados de entrada dependentes dos conjuntos
    int* capacidadeMercadoriasDeposito;
    int* mercadoriaVeiculo;
    int* demandaCliente; // demanda de mercadoria por cliente
    int* bateriaVeiculoMax;
    int* bateriaVeiculoInicial;
    bool* estadoEstacao; // estacao esta ativa ou nao
    int** custoTransporteEntreNos;
    int** distanciaEntreNos;

    // nomes
    char** nomeDeposito;
    char** nomeVeiculo;
    char** nomeCliente;
    char** nomeEstacaoCarregamento;

    // Leitura do Arquivo
    FILE* fp;
    fp = fopen(argv[1],"r");

    if( fp == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);    
    }
    
    // Leitura dos conjuntos
    fscanf(fp, "%d", &conjuntoDepositos);
    fscanf(fp, "%d", &conjuntoVeiculos);
    fscanf(fp, "%d", &conjuntoClientes);
    fscanf(fp, "%d", &conjuntoEstacoesCarregamento);

    // Definindo os dados de entrada de acordo com o tamanho dos conjuntos lidos
    // deposito
    nomeDeposito = new char*[conjuntoDepositos];
    capacidadeMercadoriasDeposito = new int[conjuntoDepositos];
    for(int i = 0; i < conjuntoDepositos; i++) {
        nomeDeposito[i] = new char[50];
    }

    // veiculos
    nomeVeiculo = new char*[conjuntoVeiculos];
    mercadoriaVeiculo = new int[conjuntoVeiculos];
    bateriaVeiculoInicial = new int[conjuntoVeiculos];
    bateriaVeiculoMax = new int[conjuntoVeiculos];
    for(int i = 0; i < conjuntoVeiculos; i++) {
        nomeVeiculo[i] = new char[50];
    }

    // clientes
    nomeCliente = new char*[conjuntoClientes];
    demandaCliente = new int[conjuntoClientes];
    for(int i = 0; i < conjuntoClientes; i++) {
        nomeCliente[i] = new char[50];
    }

    // estacoes carregamento
    nomeEstacaoCarregamento = new char*[conjuntoEstacoesCarregamento];
    estadoEstacao = new bool[conjuntoEstacoesCarregamento];
    for(int i = 0; i < conjuntoEstacoesCarregamento; i++) {
        nomeEstacaoCarregamento[i] = new char[50];
    }

    // alocacao matrizes
    int totalNos = conjuntoEstacoesCarregamento + conjuntoClientes + conjuntoDepositos;
    custoTransporteEntreNos = new int*[totalNos];
    for(int i = 0; i < totalNos; i++) {
        custoTransporteEntreNos[i] = new int[totalNos];
    }
    distanciaEntreNos = new int*[totalNos];
    for(int i = 0; i < totalNos; i++) {
        distanciaEntreNos[i] = new int[totalNos];
    }


    // Após a declaração dos tamanhos dos dados de entrada, realizando a leitura
    // depositos
    for(int i = 0; i < conjuntoDepositos; i++) {
        fscanf(fp, "\n%s %d", nomeDeposito[i], &capacidadeMercadoriasDeposito[i]);
    }

    // veiculos
    for(int i = 0; i < conjuntoVeiculos; i++) {
        fscanf(fp, "\n%s %d %d %d", nomeVeiculo[i], &mercadoriaVeiculo[i], &bateriaVeiculoInicial[i], &bateriaVeiculoMax[i]);
    }


    // clientes
    for(int i = 0; i < conjuntoClientes; i++) {
        fscanf(fp, "\n%s %d", nomeCliente[i], &demandaCliente[i]);
    }

    // estacao
    for(int i = 0; i < conjuntoEstacoesCarregamento; i++) {
        fscanf(fp, "\n%s %d", nomeEstacaoCarregamento[i], &estadoEstacao[i]);
    }

    // matriz de distancias
    for(int i = 0; i < totalNos; i++) {
        int no;
        fscanf(fp, "%d", &no);
        for(int j = 0; j < totalNos; j++) {
            fscanf(fp, "%d", &distanciaEntreNos[i][j]);
        }
    }

    // matriz custo
    for(int i = 0; i < totalNos; i++) {
        int no;
        fscanf(fp, "%d", &no);
        for(int j = 0; j < totalNos; j++) {
            fscanf(fp, "%d", &custoTransporteEntreNos[i][j]);
        }
    }
    
    // Impressão para Verificação dos dados
    

    // DECLARANDO O AMBIENTE E O MODELO MATEMATICO
    IloEnv env;
    IloModel modelo;
    // iniciando o modelo
    modelo = IloModel(env);
    

    // DECLARAÇÃO DE VARIÁVEIS DE TRÊS DIMENSÕES
    // exemplo de declaração da variável x_ijk >= 0 do tipo FLOAT, onde I, J e K maísculos representam as dimensões da variável
    // parâmetros quando não é IloNunVarArray: ambiente, tamanho

    // uso da aresta (binario)
    IloNumVar3Matrix x(env, totalNos);
    for(int i = 0; i < totalNos; i++)
    {
        x[i] = IloNumVarMatrix(env, totalNos);
        for(int j = 0; j < totalNos; j++)
        {
            x[i][j] = IloNumVarArray(env, conjuntoVeiculos, 0, 1, ILOBOOL);
        }
    }

    for(int t1 = 0 ; t1 < totalNos; t1++)
    {
        for(int t2 = 0; t2 < totalNos; t2++)
        {
            for(int t3 = 0; t3 < conjuntoVeiculos; t3++) {
                stringstream var;
                var << "x["<< t1 << "]["<< t2 << "]["<< t3 <<"]";
                x[t1][t2][t3].setName(var.str().c_str());
                modelo.add(x[t1][t2][t3]);
            }
            
        }
    }
    
    // fluxo de mercadoria
    IloNumVar3Matrix y(env, totalNos);
    for(int i = 0; i < totalNos; i++)
    {
        y[i] = IloNumVarMatrix(env, totalNos);
        for(int j = 0; j < totalNos; j++)
        {
            y[i][j] = IloNumVarArray(env, conjuntoVeiculos, 0, IloInfinity, ILOINT);
        }
    }

    for(int t1 = 0 ; t1 < totalNos; t1++)
    {
        for(int t2 = 0; t2 < totalNos; t2++)
        {
            for(int t3 = 0; t3 < conjuntoVeiculos; t3++) {
                stringstream var;
                var << "y["<< t1 << "]["<< t2 << "]["<< t3 <<"]";
                y[t1][t2][t3].setName(var.str().c_str());
                modelo.add(y[t1][t2][t3]);
            }
        }
    }

    // estado da bateria
    IloNumVar3Matrix z(env, totalNos);
    for(int i = 0; i < totalNos; i++)
    {
        z[i] = IloNumVarMatrix(env, totalNos);
        for(int j = 0; j < totalNos; j++)
        {
            z[i][j] = IloNumVarArray(env, conjuntoVeiculos, 0, IloInfinity, ILOINT);
        }
    }

    for(int t1 = 0 ; t1 < totalNos; t1++)
    {
        for(int t2 = 0; t2 < totalNos; t2++)
        {
            for(int t3 = 0; t3 < conjuntoVeiculos; t3++) {
                stringstream var;
                var << "z["<< t1 << "]["<< t2 << "]["<< t3 <<"]";
                z[t1][t2][t3].setName(var.str().c_str());
                modelo.add(z[t1][t2][t3]);
            }
        }
    }


    // DECLARAÇÃO DA FUNÇÃO OBJETIVO

    IloExpr fo(env);

    // Somatorio...
    for(int i = 0 ; i < totalNos; i++)
    {
        for(int j = 0; j < totalNos; j++)
        {
            for(int k = 0; k < conjuntoVeiculos; k++) {
                fo += custoTransporteEntreNos[i][j]*x[i][j][k];
            }
        }
    }

    //IloMinimize e IloMaximize
    modelo.add(IloMinimize(env, fo));

    
    // DECLARAÇÃO DAS RESTRIÇÕES DO PROBLEMA

    // declarando a restrição
    // IloRange parametros: ambiente, valor min, expressão, valor maximo
    
    //rest 1
    for(int c = conjuntoDepositos; c < (totalNos - conjuntoEstacoesCarregamento); c++) {
        for(int i = 0 ; i < totalNos; i++)
        {
            if(i != c) {
                IloExpr soma(env);
                for(int k = 0; k < conjuntoVeiculos; k++) {
                    soma += x[i][c][k];
                }
                modelo.add(soma == 1);
            }
        }
    } 

    //rest 2
    for(int i = 0 ; i < totalNos; i++)
    {
        for(int j = 0; j < totalNos; j++)
        {
            IloExpr soma(env);
            for(int k = 0; k < conjuntoVeiculos; k++) {
                soma += x[i][j][k];
            }
            modelo.add(soma <= 1);
        }
    } 

    //rest 3
    for(int k = 0; k < conjuntoVeiculos; k++) {
        for(int i = 0 ; i < totalNos; i++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int j = 0; j < totalNos; j++)
            {
                exp1 += x[i][j][k];
                exp2 += x[j][i][k];
            }
            modelo.add(exp1 == exp2);
        }
    }

    // rest4
    for(int j = 0; j < totalNos; j++) {
        IloExpr soma(env);
        for(int k = 0; k < conjuntoVeiculos; k++) {
            soma += x[0][j][k];
        }
        modelo.add(soma <= conjuntoVeiculos);
    }

    // rest5
    for(int i = 0; i < totalNos; i++) {
        IloExpr soma(env);
        for(int k = 0; k < conjuntoVeiculos; k++) {
            soma += x[i][0][k];
        }
        modelo.add(soma <= conjuntoVeiculos);
    }

    // rest6
    for(int c = conjuntoDepositos; c < (totalNos - conjuntoEstacoesCarregamento); c++) {
        for(int i = 0; i < totalNos; i++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int k = 0; k < conjuntoVeiculos; k++) {
                exp1 += y[i][c][k];
                exp2 += y[c][i][k] + demandaCliente[c - conjuntoDepositos];
            }
            modelo.add(exp1 == exp2);
        }
    }

    // rest7
    for(int i = 0 ; i < totalNos; i++)
    {
        for(int j = 0; j < totalNos; j++)
        {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int k = 0; k < conjuntoVeiculos; k++) {
                exp1 = x[i][j][k]*mercadoriaVeiculo[k];
                exp2 = y[i][j][k];
                modelo.add(exp2 <= exp1);
            }
        }
    }

    // rest8
    for(int k = 0; k < conjuntoVeiculos; k++) {
        IloExpr soma(env);
        for(int j = 0; j < totalNos; j++) {
            soma += y[0][j][k];
        }
        modelo.add(soma <= capacidadeMercadoriasDeposito[0]);
    }

    // rest9
    for(int c = conjuntoDepositos; c < (totalNos - conjuntoEstacoesCarregamento); c++) {
        for(int i = 0; i < totalNos; i++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            IloExpr exp3(env);
            for(int k = 0; k < conjuntoVeiculos; k++) {
                exp1 += z[i][c][k];
                exp2 += z[c][i][k];
                exp3 += distanciaEntreNos[i][c]*z[c][i][k];
            }
            modelo.add(exp1 == (exp2 + exp3));
        }
    }

    // rest10
    for(int k = 0; k < conjuntoVeiculos; k++) {
        for(int i = 0; i < totalNos; i++) {
            for(int j = 0; j < totalNos; j++) {
                modelo.add(z[i][j][k] <= bateriaVeiculoInicial[k]*x[i][j][k]);
            }
        }
    }

    // rest11
    for(int k = 0; k < conjuntoVeiculos; k++) {
        IloExpr exp1(env);
        for(int j = 0; j < totalNos; j++) {
            exp1 += z[0][j][k];
        }
        modelo.add(exp1 <= limiteMaxBateria*bateriaVeiculoMax[k]);
    }

    // rest12
    for(int k = 0; k < conjuntoVeiculos; k++) {
        IloExpr exp1(env);
        for(int j = 0; j < totalNos; j++) {
            exp1 += z[0][j][k];
        }
        modelo.add(exp1 >= limiteMinBateria*bateriaVeiculoMax[k]);
    }

    // rest13
    for(int k = 0; k < conjuntoVeiculos; k++) {
        IloExpr exp1(env);
        IloExpr exp2(env);
        for(int c = conjuntoDepositos; c < (totalNos - conjuntoEstacoesCarregamento); c++) {
            exp1 = z[c][0][k];
            exp2 = distanciaEntreNos[c][0]*x[c][0][k];
            modelo.add(exp1 >= exp2);
        }
    }

    // rest14.1
    for(int k = 0; k < conjuntoVeiculos; k++) {
        for(int e = totalNos - conjuntoEstacoesCarregamento; e < totalNos; e++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int j = 0; j < totalNos; j++) {
                exp1 += z[e][j][k];
                exp2 += x[e][j][k];
            }
            exp2 *= limiteMaxBateria*bateriaVeiculoMax[k];
            modelo.add(exp1 <= exp2);
        }
    }

    // rest14.2
    for(int k = 0; k < conjuntoVeiculos; k++) {
        for(int e = totalNos - conjuntoEstacoesCarregamento; e < totalNos; e++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int j = 0; j < totalNos; j++) {
                exp1 += z[e][j][k];
                exp2 += x[e][j][k];
            }
            exp2 *= limiteMinBateria*bateriaVeiculoMax[k];
            modelo.add(exp1 >= exp2);
        }
    }

    // rest15
    for(int e = totalNos - conjuntoEstacoesCarregamento; e < totalNos; e++) {
        for(int i = 0; i < totalNos; i++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int k = 0; k < conjuntoVeiculos; k++) {
                exp1 += y[i][e][k];
                exp2 += y[e][i][k];
            }
            modelo.add(exp1 == exp2);
        }
    }

    // rest16
    for(int k = 0; k < conjuntoVeiculos; k++) {
        for(int e = totalNos - conjuntoEstacoesCarregamento; e < totalNos; e++) {
            IloExpr exp1(env);
            IloExpr exp2(env);
            for(int c = conjuntoDepositos; c < (totalNos - conjuntoEstacoesCarregamento); c++) {
                exp1 += z[c][e][k];
                exp2 += distanciaEntreNos[c][e]*x[c][e][k];
            }
            modelo.add(exp1 >= exp2);
        }
    }


    // RESOLVENDO O MODELO

    // Carregando o módulo do Cplex
    IloCplex cplex(modelo);
    // exportando o lp
    cplex.exportModel("modelo.lp");
    // Executando o modelo
    if(!cplex.solve()) {
        cout << "Sem solucao!" << endl;
        exit(1);
    }
    
    
    // PEGAR OS VALORES DAS VARIÁVEIS 
    //cplex.getValue(NOME_VAR)    
    cout << endl;
    cout << "Resultado" << endl;

    try {
        for(int t1 = 0 ; t1 < totalNos; t1++)
        {
            for(int t2 = 0; t2 < totalNos; t2++)
            {
                for(int t3 = 0; t3 < conjuntoVeiculos; t3++) {
                    int valor = cplex.getValue(x[t1][t2][t3]);
                    cout << "x["<< t1 << "]["<< t2 << "]["<< t3 <<"]: " << valor << endl;
                }
            }
        }
    }
    catch(IloException ex) {
        cout << ex;
    }

    cout << endl;

    try {
        int obj = cplex.getObjValue();
        cout << "Valor da FO é: " << obj << endl;
    }
    catch(IloException ex) {
        cout << ex;
    }


    delete capacidadeMercadoriasDeposito;
    delete mercadoriaVeiculo;
    delete demandaCliente;
    delete bateriaVeiculoMax;
    delete bateriaVeiculoInicial;
    delete estadoEstacao;
    delete [] custoTransporteEntreNos;
    delete [] distanciaEntreNos;
    delete [] nomeDeposito;
    delete [] nomeVeiculo;
    delete [] nomeCliente;
    delete [] nomeEstacaoCarregamento;

    return 0;
}