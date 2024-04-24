#include <iostream>
#include <memory>
#include "arquivos/simulacao.h"
#define EDF 0
#define RM 1

bool is_not_number(const std::string& s) {
    return !std::all_of(s.begin(), s.end(), ::isdigit);
}

int main(int argc, char *argv[]) {
    std::string str = argv[1];
    if (argc != 2 || is_not_number(str)) { // Numero de parametros inválido
        std::cerr << "Parâmetros inválidos (apenas um número 0 ou 1)" << std::endl;
        return 1;
    }
    int escalonador = atoi(argv[1]);
    if (escalonador != EDF && escalonador != RM) { // Parametro inválido
        std::cerr << "Parâmetros inválidos (apenas 1 ou 0)" << std::endl;
        return 1;
    }

    std::unique_ptr<Simulacao> simulacao(new Simulacao(escalonador));  // Construindo unique_ptr manualmente
    simulacao->simular();
    simulacao->imprimirResultado();

    return 0;
}