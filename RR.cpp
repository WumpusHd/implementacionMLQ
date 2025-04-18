/*
Autor: Santiago Arango Henao
Codigo: 8971445
Implementacion Round Robin con Quantum Personalizable*/


#include <queue>
#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
#include <fstream>
using namespace std;

class Proceso{
private:
    string etiqueta;
    int burstTime,arrivalTime,numQueue,priority,remainingTime;
    int WT,TAT,CT,RT;
    bool firstExecution;
public:
    Proceso() {
        this->burstTime = 0;
        this->arrivalTime = 0;
        this->numQueue = 0;
        this->priority = 0;
        this->WT = 0;
        this->TAT = 0;
        this->CT = 0;
        this->RT = 0;
        this->firstExecution = true;
        this->remainingTime = burstTime;
    }
    string getEtiqueta(){return etiqueta;}
    int getBurstTime(){ return burstTime; }
    int getArrivalTime(){ return arrivalTime; }
    int getNumQueue(){ return numQueue; }
    int getPriority(){ return priority; }
    int getWT(){ return WT; }
    int getTAT(){ return TAT; }
    int getCT(){ return CT; }
    int getRT(){ return RT; }
    int getRemainingTime(){ return remainingTime; }
    bool getFirstExecution(){return firstExecution; }


    void setEtiqueta(string aux){ etiqueta = aux; }
    void setBurstTime(int bt) { burstTime = bt; }
    void setArrivalTime(int at) { arrivalTime = at; }
    void setNumQueue(int nq) { numQueue = nq; }
    void setPriority(int p) { priority = p; }
    void setWT(int wt) { WT = wt; }
    void setTAT(int tat) { TAT = tat; }
    void setCT(int ct) { CT = ct; }
    void setRT(int rt) { RT = rt; }
    void setRemainingTime(int remainingT){ remainingTime = remainingT; } 
	void setFirstExecution(bool flag){ firstExecution = flag; }
};

int convertirASCII(string texto){
    int numero = 0;
    int longitud = texto.size(); 
    for (int i = 0; i < longitud; i++){
        char c = texto[i];
        int digito = c - '0'; 
        numero = numero * 10 + digito; 
    } 
    return numero;
}

tuple<string, int> obtenerDato(string& lectura, int k){
    string ans;
    int i;
    bool flag = false;    
    for (i = k; i < lectura.size() && !flag; i++) {
        if (lectura[i] == ';')
            flag = true;
        else
            ans += lectura[i];
    }

    while (!ans.empty() && ans.front() == ' ') ans.erase(ans.begin());
    while (!ans.empty() && ans.back() == ' ') ans.pop_back();

    return make_tuple(ans, i + 1);
}

void ordenarProcesos(vector<Proceso>& procesos){
    sort(procesos.begin(), procesos.end(), []( Proceso& a,  Proceso& b){
            bool ans;
            if(a.getArrivalTime() == b.getArrivalTime())
                ans = a.getEtiqueta() < b.getEtiqueta();  
            else
                ans = a.getArrivalTime() < b.getArrivalTime(); 
            return ans;
    });
}


void RoundRobin(vector<Proceso>& procesos, int quantum) {
    ordenarProcesos(procesos);
    queue<int> cola;  
    int tiempoActual = 0, procesosCompletados = 0;
    int n = procesos.size();
    vector<bool> enCola(n, false);
    cout << "Proceso\tCT\tTAT\tWT\tRT\n";
    while (procesosCompletados < n) {
        for (int i = 0; i < n; ++i) {
            if (!enCola[i] && procesos[i].getArrivalTime() <= tiempoActual) {
                cola.push(i);
                enCola[i] = true;  
            }
        }
        if (!cola.empty()) {
            int idx = cola.front();
            cola.pop();
            Proceso &actual = procesos[idx];  
            if (actual.getFirstExecution()) {
                actual.setRT(tiempoActual - actual.getArrivalTime());
                actual.setFirstExecution(false);
            }
            int tiempoEjecutado = min(quantum, actual.getRemainingTime());
            actual.setRemainingTime(actual.getRemainingTime() - tiempoEjecutado);
            tiempoActual += tiempoEjecutado;
            if(actual.getRemainingTime() == 0){
                actual.setCT(tiempoActual);
                actual.setTAT(actual.getCT() - actual.getArrivalTime());
                actual.setWT(actual.getTAT() - actual.getBurstTime());
                procesosCompletados++;

                cout << actual.getEtiqueta() << "\t"
                     << actual.getCT() << "\t"
                     << actual.getTAT() << "\t"
                     << actual.getWT() << "\t"
                     << actual.getRT() << "\n";
            } else {
                cola.push(idx); 
            }
        } else {
            tiempoActual++;
        }
    }
}

int main() {
    string burstTime, arrivalTime, numQueue, priority, etiqueta, linea;
    tuple<string, int> aux;
    vector<Proceso> procesos;
    Proceso aux1;
    while(getline(cin, linea)){
        etiqueta = burstTime = arrivalTime = numQueue = priority = "";
        int i = 0;
        aux = obtenerDato(linea, i);
        etiqueta = get<0>(aux);
        i = get<1>(aux);
        aux = obtenerDato(linea, i);
        burstTime = get<0>(aux);
        i = get<1>(aux);
        aux = obtenerDato(linea, i);
        arrivalTime = get<0>(aux);
        i = get<1>(aux);
        aux = obtenerDato(linea, i);
        numQueue = get<0>(aux);
        i = get<1>(aux);
        aux = obtenerDato(linea, i);
        priority = get<0>(aux);
        i = get<1>(aux);
        aux1.setEtiqueta(etiqueta);
        aux1.setBurstTime(convertirASCII(burstTime));
        aux1.setArrivalTime(convertirASCII(arrivalTime));
        aux1.setNumQueue(convertirASCII(numQueue));
        aux1.setPriority(convertirASCII(priority));
        aux1.setRemainingTime(convertirASCII(burstTime));
        procesos.push_back(aux1);
    }


    RoundRobin(procesos,5);

    ofstream archivo("RR001.txt");
    archivo << "# archivo: RR001.txt\n";
    archivo << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT\n";

    double totalWT = 0, totalCT = 0, totalRT = 0, totalTAT = 0;

    for (Proceso p : procesos) {
        archivo << p.getEtiqueta() << ";"
                << p.getBurstTime() << ";"
                << p.getArrivalTime() << ";"
                << p.getNumQueue() << ";"
                << p.getPriority() << ";"
                << p.getWT() << ";"
                << p.getCT() << ";"
                << p.getRT() << ";"
                << p.getTAT() << "\n";

        totalWT += p.getWT();
        totalCT += p.getCT();
        totalRT += p.getRT();
        totalTAT += p.getTAT();
    }
    int n = procesos.size();
    archivo << "WT=" << totalWT / n << "; "
            << "CT=" << totalCT / n << "; "
            << "RT=" << totalRT / n << "; "
            << "TAT=" << totalTAT / n << ";\n";

    archivo << "\nEste archivo mostrará el tiempo de espera (WT), tiempo de completado (CT),\n";
    archivo << "tiempo de respuesta (RT) y TurnAround Time (TAT). Adicional, los valores\n";
    archivo << "promedio de estas métricas\n";
    archivo.close();
    return 0;
}
