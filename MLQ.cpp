/*
Autor: Santiago Arango Henao
Codigo: 8971445
Implementacion Multi Level Queue*/



#include <queue>
#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
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


void ordenarProcesos1(vector<Proceso*>& procesos){
    sort(procesos.begin(), procesos.end(), [](Proceso* a, Proceso* b){
        bool ans;
        if(a->getArrivalTime() == b->getArrivalTime())
            ans = a->getEtiqueta() < b->getEtiqueta();
        else
            ans = a->getArrivalTime() < b->getArrivalTime();
        return ans;
    });
}


void MLQ(vector<Proceso>& procesos) {
    vector<Proceso>::iterator it;
    vector<Proceso*> q1, q2, q3;
    
    for(it = procesos.begin(); it != procesos.end(); ++it){
        if(it->getNumQueue() == 1)
            q1.push_back(&(*it));
        else if(it->getNumQueue() == 2)
            q2.push_back(&(*it));
        else
            q3.push_back(&(*it));
    }

    ordenarProcesos1(q1);
    ordenarProcesos1(q2);
    ordenarProcesos1(q3);

    int tiempoActual = 0;
    cout << "Proceso\tCT\tTAT\tWT\tRT\n";

    // ======================= COLA 1 - Round Robin (Quantum 1) =======================
    int idx1 = 0;
    queue<Proceso*> readyQ1;

    while (idx1 < q1.size() || !readyQ1.empty()) {
        while (idx1 < q1.size() && q1[idx1]->getArrivalTime() <= tiempoActual) {
            readyQ1.push(q1[idx1]);
            idx1++;
        }

        if (!readyQ1.empty()) {
            Proceso* actual = readyQ1.front();
            readyQ1.pop();

            if (actual->getFirstExecution()) {
                actual->setRT(tiempoActual - actual->getArrivalTime());
                actual->setFirstExecution(false);
            }

            int ejecutado = min(1, actual->getRemainingTime());
            actual->setRemainingTime(actual->getRemainingTime() - ejecutado);
            tiempoActual += ejecutado;

            if (actual->getRemainingTime() == 0) {
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
        } else {
            tiempoActual++;
        }
    }

    // ======================= COLA 2 - Round Robin (Quantum 3) =======================
    int idx2 = 0;
    queue<Proceso*> readyQ2;

    while (idx2 < q2.size() || !readyQ2.empty()) {
        while (idx2 < q2.size() && q2[idx2]->getArrivalTime() <= tiempoActual) {
            readyQ2.push(q2[idx2]);
            idx2++;
        }

        if (!readyQ2.empty()) {
            Proceso* actual = readyQ2.front();
            readyQ2.pop();

            if (actual->getFirstExecution()) {
                actual->setRT(tiempoActual - actual->getArrivalTime());
                actual->setFirstExecution(false);
            }

            int ejecutado = min(3, actual->getRemainingTime());
            actual->setRemainingTime(actual->getRemainingTime() - ejecutado);
            tiempoActual += ejecutado;

            if (actual->getRemainingTime() == 0) {
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
        } else {
            tiempoActual++;
        }
    }

    // ======================= COLA 3 - SJF No Expropiativo =======================
    int idx3 = 0;
    vector<Proceso*> readyQ3;

    while (idx3 < q3.size() || !readyQ3.empty()) {
        while (idx3 < q3.size() && q3[idx3]->getArrivalTime() <= tiempoActual) {
            readyQ3.push_back(q3[idx3]);
            idx3++;
        }

        sort(readyQ3.begin(), readyQ3.end(), [](Proceso* a, Proceso* b) {
            return a->getBurstTime() < b->getBurstTime();
        });

        if (!readyQ3.empty()) {
            Proceso* actual = readyQ3.front();
            readyQ3.erase(readyQ3.begin());

            if (actual->getFirstExecution()) {
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


    MLQ(procesos);

    for (Proceso p : procesos) {
        cout << "Proceso: " << p.getEtiqueta() 
             << " | Arrival Time: " << p.getArrivalTime() 
             << " | Burst Time: " << p.getBurstTime() 
             << " | Remaining Time: " << p.getRemainingTime() 
             << " | Completion Time (CT): " << p.getCT() 
             << " | Turnaround Time (TAT): " << p.getTAT() 
             << " | Waiting Time (WT): " << p.getWT() 
             << " | Response Time (RT): " << p.getRT() 
             << " | Queue Number: " << p.getNumQueue()
             << " | Priority: " << p.getPriority()
             << " | First Execution: " << (p.getFirstExecution() ? "Yes" : "No") 
             << endl;
    }
    return 0;
}
