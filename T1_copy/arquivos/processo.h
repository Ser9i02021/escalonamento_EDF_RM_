#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
using namespace std;

// Estrutura para representar o contexto de CPU de um processo.
// Considera apenas uma simplificação com program counter, stack pointer, status e outros 6 registradores de propósito geral.
#pragma once
struct CPUContext {
    unsigned long long int pc;  // Program Counter
    unsigned long long int sp;  // Stack Pointer
    unsigned long long int status;  // Status register
    map<string, int> generalPurposeRegisters;  // Registradores de propósito geral

    CPUContext()
    : pc(0x00400000), sp(0x7fffeffc), status(0x0000ff11) {
        generalPurposeRegisters["$s0"] = 0x00000000;
        generalPurposeRegisters["$s1"] = 0x00000000;
        generalPurposeRegisters["$v0"] = 0x00000000;
        generalPurposeRegisters["$a0"] = 0x00000000;
        generalPurposeRegisters["$a1"] = 0x00000000;
        generalPurposeRegisters["$ra"] = 0x00000000;
    }  // Inicializa com 6 registradores, PC, ST e SP
};

// Classe Processo
class Processo {
public:
    int id;  // Identificador único do processo
    int dataCriacao;  // Timestamp de criação
    int duracao;  // Duração total do processo
    int periodo; // Perído de cada instância do processo
    int deadline; // Deadline de cada instância do processo
    int prioridadeEstatica;  // Prioridade estática do processo
    string estadoAtual;  // Estado atual: Pronto, Executando, Terminado
    int tempoJaExecutado;  // Tempo que o processo já executou
    stack<int> procMemory; // Memória alocada hipoteticamente na região de pilha para a chamada de procedimentos do programa
    CPUContext* contexto;  // Ponteiro para um contexto de CPU

    // Construtor da classe
    Processo(int id, int dataCriacao, int duracao, int periodo, int deadline, int prioridade, string EA) :
        id(id), dataCriacao(dataCriacao), duracao(duracao),
        periodo(periodo), deadline(deadline), prioridadeEstatica(prioridade),
        estadoAtual(EA), tempoJaExecutado(0), contexto(new CPUContext()) {}

    // Destrutor
    ~Processo() {
        delete contexto;  // Libera o contexto de CPU
    }

    // Métodos para manipulação do processo aqui (ex: salvarContexto, restaurarContexto)
    // Método para progredir o contexto de cada processo
    void progredirContexto() {
        // Contexto progride em um ritmo inversamente proporcional à duração da execução da instância do processo
        if (contexto) {  // Verificar se contexto existe
            if (id == 0) {
                if (tempoJaExecutado == 1) {
                    contexto->generalPurposeRegisters["$a0"] = 10;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a1"] = 20;
                    contexto->pc = contexto->pc + 4;

                } else if (tempoJaExecutado == 2) {
                    contexto->generalPurposeRegisters["$ra"] = contexto->pc + 4;
                    contexto->pc = contexto->pc + 16;
                    
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] - 8;
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$ra"]);
                    contexto->pc = contexto->pc + 4;
                    
                } else if (tempoJaExecutado == 3) {
                    procMemory.push(contexto->generalPurposeRegisters["$a0"]);
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = contexto->generalPurposeRegisters["$a0"] + contexto->generalPurposeRegisters["$a1"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a0"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                
                } else if (tempoJaExecutado == 4) {
                    contexto->generalPurposeRegisters["$ra"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] + 8;
                    contexto->pc = contexto->pc + 4;

                } else {
                    contexto->pc = contexto->generalPurposeRegisters["$ra"];
                    contexto->generalPurposeRegisters["$s0"] = contexto->generalPurposeRegisters["$v0"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = 10;
                    contexto->pc = contexto->pc + 4;

                }

            } else if (id == 1) {
                // exeProc1();
                if (tempoJaExecutado == 1) {
                    contexto->generalPurposeRegisters["$a0"] = 10;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a1"] = 20;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = contexto->pc + 4;
                    contexto->pc = contexto->pc + 16;
                    
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] - 8;
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$ra"]);
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$a0"]);
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = contexto->generalPurposeRegisters["$a0"] + contexto->generalPurposeRegisters["$a1"];
                    contexto->pc = contexto->pc + 4;
                } else {
                    contexto->generalPurposeRegisters["$a0"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] + 8;
                    contexto->pc = contexto->pc + 4;

                    contexto->pc = contexto->generalPurposeRegisters["$ra"];
                    contexto->generalPurposeRegisters["$s0"] = contexto->generalPurposeRegisters["$v0"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = 10;
                    contexto->pc = contexto->pc + 4;
                }
            } else if (id == 2) {
                if (tempoJaExecutado == 1) {
                    contexto->generalPurposeRegisters["$a0"] = 10;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a1"] = 20;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = contexto->pc + 4;
                    contexto->pc = contexto->pc + 16;

                } else if (tempoJaExecutado == 2) {
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] - 8;
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$ra"]);
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$a0"]);
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = contexto->generalPurposeRegisters["$a0"] + contexto->generalPurposeRegisters["$a1"];
                    contexto->pc = contexto->pc + 4;
                    
                } else if (tempoJaExecutado == 3) {
                    contexto->generalPurposeRegisters["$a0"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] + 8;
                    contexto->pc = contexto->pc + 4;
                
                } else {
                    contexto->pc = contexto->generalPurposeRegisters["$ra"];
                    contexto->generalPurposeRegisters["$s0"] = contexto->generalPurposeRegisters["$v0"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = 10;
                    contexto->pc = contexto->pc + 4;

                }
            } else {
                if (tempoJaExecutado == 1) {
                    contexto->generalPurposeRegisters["$a0"] = 10;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a1"] = 20;
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = contexto->pc + 4;
                    contexto->pc = contexto->pc + 16;
                    
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] - 8;
                    contexto->pc = contexto->pc + 4;
                    procMemory.push(contexto->generalPurposeRegisters["$ra"]);
                    contexto->pc = contexto->pc + 4;
                } else if (tempoJaExecutado == 2) {
                    procMemory.push(contexto->generalPurposeRegisters["$a0"]);
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = contexto->generalPurposeRegisters["$a0"] + contexto->generalPurposeRegisters["$a1"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$a0"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$ra"] = procMemory.top();
                    procMemory.pop();
                    contexto->pc = contexto->pc + 4;

                } else {
                    contexto->generalPurposeRegisters["$sp"] = contexto->generalPurposeRegisters["$sp"] + 8;
                    contexto->pc = contexto->pc + 4;

                    contexto->pc = contexto->generalPurposeRegisters["$ra"];
                    contexto->generalPurposeRegisters["$s0"] = contexto->generalPurposeRegisters["$v0"];
                    contexto->pc = contexto->pc + 4;
                    contexto->generalPurposeRegisters["$v0"] = 10;
                    contexto->pc = contexto->pc + 4;

                }
            }
        }
    }

    // Metodo para resetar contexto
    void resetarContexto() {
        if (contexto) {  // Verificar se contexto existe
            contexto->pc = 0x00400000;
            contexto->sp = 0x7fffeffc;
            contexto->status = 0x0000ff11;
            contexto->generalPurposeRegisters["$s0"] = 0x00000000;
            contexto->generalPurposeRegisters["$s1"] = 0x00000000;
            contexto->generalPurposeRegisters["$v0"] = 0x00000000;
            contexto->generalPurposeRegisters["$a0"] = 0x00000000;
            contexto->generalPurposeRegisters["$a1"] = 0x00000000;
            contexto->generalPurposeRegisters["$ra"] = 0x00000000;
        }
    }

    // Cópia do construtor para melhor gerência de memória
    Processo(const Processo& other) :
        id(other.id), dataCriacao(other.dataCriacao), duracao(other.duracao),
        periodo(other.periodo), deadline(other.deadline), prioridadeEstatica(other.prioridadeEstatica),
        estadoAtual(other.estadoAtual), tempoJaExecutado(other.tempoJaExecutado),
        contexto(new CPUContext(*other.contexto)) {}
};