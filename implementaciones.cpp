/*
Autor: Santiago Arango Henao
Codigo: 8971445
Implementacion First Come, First Served (FCFS)*/



#include <queue>
#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
#include <climits>
#define INT_MAX 2147483647

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

void FCFS(vector<Proceso>& procesos){
    vector<Proceso>::iterator it;
    ordenarProcesos(procesos);
    int tiempoActual = 0;
    cout << "Proceso\tCT\tTAT\tWT\tRT\n";
    for(it = procesos.begin(); it != procesos.end(); it++){
        if (tiempoActual < it->getArrivalTime())
            tiempoActual = it->getArrivalTime();  
        it->setCT(tiempoActual + it->getBurstTime());
        it->setTAT(it->getCT() - it->getArrivalTime());
        it->setWT(it->getTAT() - it->getBurstTime());
        it->setRT(it->getWT());  
        tiempoActual = it->getCT();
         cout << it->getEtiqueta() << "\t"
                 << it->getCT() << "\t"
                 << it->getTAT() << "\t"
                 << it->getWT() << "\t"
                 << it->getRT() << "\n";
    }
}

struct Comparador{
    bool operator()(Proceso& a, Proceso& b) {
        return a.getBurstTime() > b.getBurstTime();
    }
};

void SJF(vector<Proceso>& procesos){
    ordenarProcesos(procesos);
    priority_queue<Proceso, vector<Proceso>, Comparador> pq;
    int tiempoActual = 0,procesosCompletados = 0;
    vector<bool> enCola(procesos.size(), false);
    vector<Proceso>::iterator it;
    cout << "Proceso\tCT\tTAT\tWT\tRT\n";
    while (procesosCompletados < procesos.size()){
        for (it = procesos.begin(); it != procesos.end(); ++it){
            if (!enCola[it - procesos.begin()] && it->getArrivalTime() <= tiempoActual){
                pq.push(*it);
                enCola[it - procesos.begin()] = true;
            }
        }
        if(!pq.empty()){
            Proceso actual = pq.top();
            pq.pop();
            actual.setCT(tiempoActual + actual.getBurstTime());
            actual.setTAT(actual.getCT() - actual.getArrivalTime());
            actual.setWT(actual.getTAT() - actual.getBurstTime());
            actual.setRT(tiempoActual - actual.getArrivalTime());
            tiempoActual = actual.getCT();
            procesosCompletados++;
            cout << actual.getEtiqueta() << "\t"
                 << actual.getCT() << "\t"
                 << actual.getTAT() << "\t"
                 << actual.getWT() << "\t"
                 << actual.getRT() << "\n";
        }else{
            tiempoActual++;
        }
    }
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
/*
void MLQ(vector<Proceso>& procesos){
    vector<Proceso>::iterator it;

    vector<Proceso> q1,q2,q3;
    for(it = procesos.begin(); it != procesos.end(); ++it){
        if(it->getNumQueue() == 1)
            q1.push_back(*it);
        else if(it->getNumQueue() == 2)
            q2.push_back(*it);
        else
            q3.push_back(*it);
    }


    ordenarProcesos(q1);
    ordenarProcesos(q2);
    ordenarProcesos(q3);



    // Primero ejecutar la cola 1 (RR quantum 5)
    if(!q1.empty()) {
        cout << "Ejecutando cola 1 (Round Robin quantum 5):\n";
        RoundRobin(q1, 5);
    }

    // Luego ejecutar la cola 2 (RR quantum 3)
    if(!q2.empty()) {
        cout << "\nEjecutando cola 2 (Round Robin quantum 3):\n";
        RoundRobin(q2, 3);
    }

    // Finalmente ejecutar la cola 3 (SJF no expropiativo)
    if(!q3.empty()) {
        cout << "\nEjecutando cola 3 (SJF no expropiativo):\n";
        SJF(q3);
    }

    // Combinar resultados en el orden original
    procesos.clear();
    procesos.insert(procesos.end(), q1.begin(), q1.end());
    procesos.insert(procesos.end(), q2.begin(), q2.end());
    procesos.insert(procesos.end(), q3.begin(), q3.end());
}*/

void MLQ(vector<Proceso>& procesos) {
    vector<Proceso>::iterator it;
    vector<Proceso> q1,q2,q3;
    for(it = procesos.begin(); it != procesos.end(); ++it){
        if(it->getNumQueue() == 1)
            q1.push_back(*it);
        else if(it->getNumQueue() == 2)
            q2.push_back(*it);
        else
            q3.push_back(*it);
    }

    ordenarProcesos(q1);
    ordenarProcesos(q2);
    ordenarProcesos(q3);
    
    int tiempoActual = 0;
    int idx1 = 0, idx2 = 0, idx3 = 0;
    queue<Proceso*> readyQ1, readyQ2;
    vector<Proceso*> readyQ3;
    
    cout << "Proceso\tCT\tTAT\tWT\tRT\n";

    while(idx1 < q1.size() || idx2 < q2.size() || idx3 < q3.size() || 
          !readyQ1.empty() || !readyQ2.empty() || !readyQ3.empty()) {
        
        // Agregar procesos a colas listas según arrival time
        while(idx1 < q1.size() && q1[idx1].getArrivalTime() <= tiempoActual) {
            readyQ1.push(&q1[idx1]);
            idx1++;
        }
        while(idx2 < q2.size() && q2[idx2].getArrivalTime() <= tiempoActual) {
            readyQ2.push(&q2[idx2]);
            idx2++;
        }
        while(idx3 < q3.size() && q3[idx3].getArrivalTime() <= tiempoActual) {
            readyQ3.push_back(&q3[idx3]);
            idx3++;
        }

        // Ordenar cola 3 por SJF (burst time)
        sort(readyQ3.begin(), readyQ3.end(), [](Proceso* a, Proceso* b) {
            return a->getBurstTime() < b->getBurstTime();
        });

        if(!readyQ1.empty()) {
            // Round Robin quantum 5 para cola 1
            Proceso* actual = readyQ1.front();
            readyQ1.pop();
            
            
            if(actual->getFirstExecution()) {
                actual->setRT(tiempoActual - actual->getArrivalTime());
                actual->setFirstExecution(false);
            }
            
            int ejecutado = min(5, actual->getRemainingTime());
            actual->setRemainingTime(actual->getRemainingTime() - ejecutado);
            tiempoActual += ejecutado;
            
            if(actual->getRemainingTime() == 0){
                actual->setCT(tiempoActual);
                actual->setTAT(actual->getCT() - actual->getArrivalTime());
                actual->setWT(actual->getTAT() - actual->getBurstTime());
                
                cout << actual->getEtiqueta() << "\t"
                     << actual->getCT() << "\t"
                     << actual->getTAT() << "\t"
                     << actual->getWT() << "\t"
                     << actual->getRT() << "\n";
            } else {
                readyQ1.push(actual);
            }
        }
        else if(!readyQ2.empty()) {
            // Round Robin quantum 3 para cola 2
            Proceso* actual = readyQ2.front();
            readyQ2.pop();
            
            if(actual->getFirstExecution()) {
                actual->setRT(tiempoActual - actual->getArrivalTime());
                actual->setFirstExecution(false);
            }
            
            int ejecutado = min(3, actual->getRemainingTime());
            actual->setRemainingTime(actual->getRemainingTime() - ejecutado);
            tiempoActual += ejecutado;
            
            if(actual->getRemainingTime() == 0) {
                actual->setCT(tiempoActual);
                actual->setTAT(actual->getCT() - actual->getArrivalTime());
                actual->setWT(actual->getTAT() - actual->getBurstTime());
                
                cout << actual->getEtiqueta() << "\t"
                     << actual->getCT() << "\t"
                     << actual->getTAT() << "\t"
                     << actual->getWT() << "\t"
                     << actual->getRT() << "\n";
            } else {
                readyQ2.push(actual);
            }
        }
        else if(!readyQ3.empty()) {
            // SJF no expropiativo para cola 3
            Proceso* actual = readyQ3.front();
            readyQ3.erase(readyQ3.begin());
            
            if(actual->getFirstExecution()) {
                actual->setRT(tiempoActual - actual->getArrivalTime());
                actual->setFirstExecution(false);
            }
            
            tiempoActual += actual->getRemainingTime();
            actual->setRemainingTime(0);
            actual->setCT(tiempoActual);
            actual->setTAT(actual->getCT() - actual->getArrivalTime());
            actual->setWT(actual->getTAT() - actual->getBurstTime());
            
            cout << actual->getEtiqueta() << "\t"
                 << actual->getCT() << "\t"
                 << actual->getTAT() << "\t"
                 << actual->getWT() << "\t"
                 << actual->getRT() << "\n";
        }
        else {
            int nextArrival = INT_MAX;
            if(idx1 < q1.size()) nextArrival = min(nextArrival, q1[idx1].getArrivalTime());
            if(idx2 < q2.size()) nextArrival = min(nextArrival, q2[idx2].getArrivalTime());
            if(idx3 < q3.size()) nextArrival = min(nextArrival, q3[idx3].getArrivalTime());
            
            if(nextArrival != INT_MAX) {
                tiempoActual = nextArrival;
            }
        }
    }
}



int main() {
    string burstTime, arrivalTime, numQueue, priority, etiqueta;
    string linea;
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


    MLQ(procesos);
    return 0;
}