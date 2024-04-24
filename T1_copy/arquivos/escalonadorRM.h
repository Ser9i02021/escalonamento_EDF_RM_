#include "escalonador.h"
using namespace std;

// Classe EscalonadorRM
class EscalonadorRM: public Escalonador {
public:
    // Ordena a fila de processos em ordem decrescente de período
    void ordenarFila(unique_ptr<ProcessosBuffer>& processos) override {
        sort(processos->filaDeProcessosProntos.begin(), processos->filaDeProcessosProntos.end(), [](const Processo* a, const Processo* b) {
            return a->periodo < b->periodo;
        });
    }
    // Verifica a troca de contexto comparando o processo na frente da fila de prontos com o que está executando, com base no período de cada um
    Processo* verificarTrocaDeContexto(Processo* processoCPU, Processo* processoFila, unique_ptr<ProcessosBuffer>& processos) override {
        if (processoFila->periodo < processoCPU->periodo) {
            // Retira o processo da CPU e o coloca na fila
            processos->filaDeProcessosProntos.push_back(processoCPU);
            // Setta o estado do processo que voltou para fila como "pronto"
            processos->filaDeProcessosProntos.back()->estadoAtual = "--";
            // Reordena a fila de prontos com o novo processo inserido
            ordenarFila(processos);
            // Retira o processo no início da fila e o coloca na CPU
            processoCPU = retirarPrimeiroDaFila(processos);
        }
        return processoCPU;
    }
};