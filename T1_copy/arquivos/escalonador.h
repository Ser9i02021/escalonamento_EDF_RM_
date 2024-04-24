#include "processos_buffer.h"
#include <algorithm>
using namespace std;

#pragma once
class Escalonador {
public:
    virtual ~Escalonador() {}  // Destrutor virtual para limpeza de classes derivadas

    // Adiciona processos à fila com base no período (RM) ou na deadline (EDF)
    virtual void  ordenarFila(unique_ptr<ProcessosBuffer>& processos) {
        
    }
    // Verifica a troca de contexto com base no algoritmo de escalonamento requisitado (RM ou EDF)
    virtual Processo* verificarTrocaDeContexto(Processo* processoCPU, Processo* processoFila, unique_ptr<ProcessosBuffer>& processos)= 0;

    // Retira o primeiro processo da fila de prontos
    Processo* retirarPrimeiroDaFila(unique_ptr<ProcessosBuffer>& processos) {
        if (!processos->filaDeProcessosProntos.empty()) {
            // O processo no início da fila é o de maior prioridade
            Processo* processoEscolhido = processos->filaDeProcessosProntos.front();
            processos->filaDeProcessosProntos.erase(processos->filaDeProcessosProntos.begin()); // Remove o processo da fila
            return processoEscolhido;
        }
        return nullptr; // Retorna nulo se não houver processos
    }

     // Função para imprimir a fila de processos (para depuração ou visualização)
    void imprimirFila(unique_ptr<ProcessosBuffer>& processos) const {
        if (processos) {
            for (const auto* processo : processos->filaDeProcessosProntos) {
                std::cout << "Processo ID: " << processo->id << ", Prioridade: " << processo->prioridadeEstatica << std::endl;
            }
        }
    }
};