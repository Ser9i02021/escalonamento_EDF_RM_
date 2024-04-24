#include "read_file.h"
using namespace std;

class ProcessosBuffer {
public:
    vector<Processo*> Processos;
    vector<Processo*> filaDeProcessosProntos;
    
    ProcessosBuffer() {
        File file; // Abre o arquivo ".txt" que contém os processos
        file.read_file(); // Lê os processos do arquivo ".txt" e os coloca em um vetor (que é atributo privado da classe "File")

        for (const auto& processo : file.getProcesses()) {
			Processos.push_back(new Processo(*processo));
            if (processo->estadoAtual == "--") {
                // Adiciona os processos na fila de processos prontos
                filaDeProcessosProntos.push_back(new Processo(*processo));
            }
		}
    }
    // Retorna a fila de processos prontos
    const std::vector<Processo*>& getFilaDeProcessosProntos() const {
        return filaDeProcessosProntos;
    }
};