#include <cstdio>
#include <iostream>
#include <memory>
#include <iomanip> // Para impressão (setw(2), setfill('0'))
#include <numeric> // Para (std::accumulate)
#include "escalonadorRM.h" // Inclui "escalonadorRM.h", que inclui "escalonador.h", que inclui "processos_buffer.h", que inclui "read_file.cc", que inclui "processo.h"
#include "escalonadorEDF.h" // Inclui "escalonadorEDF.h", que inclui "escalonador.h", que inclui "processos_buffer.h", que inclui "read_file.cc", que inclui "processo.h"
using namespace std;

class Simulacao {
private:
    unique_ptr<ProcessosBuffer> processos; // Smart pointer para melhor gerenciamento de memória
    unique_ptr<Escalonador> escalonador; // Ponteiro para o objeto da classe Escalonador
    int tempoTotal = 0; // Tempo transcorrido na simulação
    int trocasDeContexto = 0; // Quantificação das trocas de contexto ao longo da simulação
    vector<int> espera; // Vetor para armazenar o somatório do tempos de espera das instâncias de um processo para todos os processos
    vector<vector<int>> turnaround; // Vetor de vetores para armazenar o turnaround time de todas as instâncias de um processo para cada processo
    vector<int> deadlinesPerdidosPorProcesso; // Vetor para guardar a quantidade de deadlines perdidos para cada processo
    vector<vector<string>> diagrama; // Vetor de vetores para gerenciar a impressão do diagrama da simulação
    
public:
    // Construtor
    Simulacao(bool algoritmoDeEscalonamento) {
        // Cria-se todos os processos da simulação e a fila de processos prontos
        processos.reset(new ProcessosBuffer());

        if (algoritmoDeEscalonamento) {
            escalonador.reset(new EscalonadorRM());
        } else {
            escalonador.reset(new EscalonadorEDF());
        }

        espera = vector<int>(processos->Processos.size(), 0);
        turnaround = vector<vector<int> >(processos->Processos.size());
        deadlinesPerdidosPorProcesso = vector<int>(processos->Processos.size(), 0);
        
    }

    // Método para administrar o funcionamento da simulação
    void simular() {
        int processoAtualID = -1;
        Processo* processoAtual = nullptr;

        cout << "TEMPO" << " | "; 
        for (Processo* processo : processos->Processos) {
            cout << "P" << processo->id << " | ";
        }
        cout << "PERDERAM DEADLINE" << endl;
        vector<string> Tarefas_passaram_dead_line = vector<string>(processos->Processos.size());
        while (tempoTotal < 50) {
            Tarefas_passaram_dead_line.clear();
            for (Processo* processo : processos->Processos) {
                
                if (processo != nullptr) {  // Verifica se ponteiro é não nulo
                    if (processo->tempoJaExecutado == processo->duracao && processo->estadoAtual == "##") { // Verifica se o processo terminou sua execução
                        processo->estadoAtual = "  ";
                        processo->tempoJaExecutado = 0;
                        turnaround[processo->id].push_back(tempoTotal - processo->dataCriacao);
                        processoAtualID = -1;

                        processo->resetarContexto();
                    }
                    if (tempoTotal > 0 && ((tempoTotal + 1) == (processo->periodo + 1 + processo->dataCriacao) || processo->dataCriacao == tempoTotal)) { // Nova instância do processo ficou pronta        
                        if (processo->estadoAtual != "  ") { // Caso COM perda de deadline
                            deadlinesPerdidosPorProcesso[processo->id]++;
                            Tarefas_passaram_dead_line.push_back("P" + to_string(processo->id));
                            if (processo->estadoAtual == "##") {
                                processos->filaDeProcessosProntos.push_back(processo);    
                            }
                            // O instante da perda do deadline será o instante final do turnaround time da instância do processo
                            turnaround[processo->id].push_back(tempoTotal - processo->dataCriacao);
                        } else { // Caso SEM perda de deadline
                            processos->filaDeProcessosProntos.push_back(processo);    
                        }
                        processo->tempoJaExecutado = 0;
                        processo->dataCriacao = tempoTotal;
                        processo->estadoAtual = "--";
                        processo->resetarContexto();
                    }
                }
            }
            // Ordena-se a fila de instâncias de processos prontas a cada instante da simulção
            escalonador->ordenarFila(processos);
            // Obtém-se o processo no início da fila de prontos (sem retirá-lo da fila)
            Processo* proximoProcesso = processos->filaDeProcessosProntos.front();
            // Verifica se há algum processo na CPU
            if (processoAtualID == -1) { 

                // Caso não haja nenhuma instância de processo na CPU,
                // retira-se a instância do processo no início da fila de
                // prontos e a coloca na CPU
                processoAtual = escalonador->retirarPrimeiroDaFila(processos);
                processoAtualID = processoAtual->id;
                processoAtual->estadoAtual = "##";
                processos->Processos[processoAtualID]->estadoAtual = "##";
                
                
                trocasDeContexto++;
                
            } else {
                // Caso exista um processo na CPU,
                // avalia-se a possibilidade de retirar a instâncias do processo
                // em execução da CPU, reinserí-lo na fila de prontos,
                // que será reordenada e retirar a instância do processo no início da
                // fila de prontos, colocando-a na CPU, com base
                // no algoritmo de escalonamento da simulação
                Processo* novoProcessoAtual = escalonador->verificarTrocaDeContexto(processoAtual, proximoProcesso, processos);
                if (novoProcessoAtual != nullptr && novoProcessoAtual != processoAtual) {
                    processoAtual = novoProcessoAtual;
                    processoAtualID = processoAtual->id;
                    processoAtual->estadoAtual = "##";
                    trocasDeContexto++;
                }
            }

            // Incrementa o tempo de espera para todas as instâncias de processos prontas
            for (Processo* processo : processos->filaDeProcessosProntos) {
                if (processo != nullptr) {
                    espera[processo->id]++;
                }
            }

            // Atualiza o diagrama de execução e o tempo de turnaround
            vector<string> estadoAtualProcessos(processos->Processos.size(), "  ");
            for (Processo* processo : processos->Processos) {
                if (processo != nullptr) {
                    if (processo->estadoAtual == "##") { // Se a instância do processo está em execução
                        estadoAtualProcessos[processo->id] = "##";
                        processo->tempoJaExecutado++;
                    } else if (processo->estadoAtual == "--") { // Se a instância do processo está no estado pronto
                        estadoAtualProcessos[processo->id] = "--";
                    } else { // Se a instância do processo já terminou ou ainda não foi criada
                        estadoAtualProcessos[processo->id] = "  ";
                    }
                }
            }

            diagrama.push_back(estadoAtualProcessos);
            // Caso tempo tiver apenas um digito é preenchido um 0 na frente para ajustar impressão
            cout << std::setw(2) << std::setfill('0') << tempoTotal << "-";
            cout << std::setw(2) << std::setfill('0') << tempoTotal+1 << " | ";
            for (Processo* processo : processos->Processos) {
                cout << processo->estadoAtual << " | ";
            }
            for (const string& tarefa : Tarefas_passaram_dead_line) {
                cout << tarefa << " ";
            }
            cout << endl;


            // Execução de um instante da instância do processo atual
            processos->Processos[processoAtualID]->progredirContexto();
            tempoTotal++;
        }
    }

    // Após a simulação, imprime-se os resultados
    void imprimirResultado() {
        double somaTT = 0;
        cout << "Turnaround time por processo:" << endl;
        for (Processo* processo : processos->Processos) {
            if (processo != nullptr) {
                double soma_turnaround = accumulate(turnaround[processo->id].begin(), turnaround[processo->id].end(), 0);
                if (turnaround[processo->id].size()) {
                    double media_turnaround = soma_turnaround / turnaround[processo->id].size();
                    cout << "Processo " << processo->id << ": " << media_turnaround << endl;
                    
                    somaTT = somaTT + media_turnaround;
                } else {
                    cout << "Processo " << processo->id << ": 0" << endl;
                }
            }
        }
        double averageTurnaroundTime = somaTT / processos->Processos.size();
        cout << "Turnaround time médio: " << averageTurnaroundTime << endl;
        
        
        cout << "Tempo médio de espera: ";
        int somaEspera = accumulate(espera.begin(), espera.end(), 0);
        cout << static_cast<double>(somaEspera) / processos->Processos.size() << endl;
        

        cout << "Total de trocas de contexto: " << trocasDeContexto << endl;
        cout << "Total de deadlines perdidas: " << endl;
        for (long unsigned int i = 0; i < processos->Processos.size(); i++) {
            cout << "Processo " << i << ": " << deadlinesPerdidosPorProcesso[i] << endl;
        }
    }
};