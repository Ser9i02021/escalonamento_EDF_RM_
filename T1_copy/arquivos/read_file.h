#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "processo.h"

using namespace std;

// Note que ProcessParams foi substituído pela classe Processo
#pragma once
class File {
public:
    File() {
        myfile.open("entrada.txt");
        if (!myfile.is_open()) {
            cout << "Erro ao abrir o arquivo!\n";
        }
    }
    
    void read_file() {
        int a, b, c, d, e;
        
        if (!myfile.is_open()) {
            cout << "Arquivo não está aberto!" << endl;
        }
        
		int pid = 0;	// ID dos processos para cada novo criado (pid = pid + 1)
        while (myfile >> a >> b >> c >> d >> e) {
            string ea;
            if (a == 0) {
                ea = "--";
            } else {
                ea = "  ";
            }
            Processo *p = new Processo(pid, a, b, c, d, e, ea); // Ajuste conforme o construtor de Processo
            processes.push_back(p);
			pid = pid + 1;
        }
    }

    // Método para obter os processos lidos
    const vector<Processo*>& getProcesses() const {
        return processes;
    }

    void print_processes_params() {
        for(auto *p : processes) {
            cout << "ID: " << p->id << ", Creation time: " << p->dataCriacao 
                 << ", Duration: " << p->duracao << ", Period: " << p->periodo
                 << ", Deadline: " << p->deadline << ", Priority: " << p->prioridadeEstatica << endl;
        }
    }

    ~File() {
        for(auto *p : processes) {
            delete p;
        }
    }

private:
    ifstream myfile; 
    vector<Processo*> processes; // Alterado para armazenar ponteiros para Processo
};