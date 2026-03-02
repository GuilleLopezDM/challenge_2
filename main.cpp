#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include<chrono>
/* Muros:   #
   Raton:   R
   Camino:  . 
   Entrada: E
   Salida:  S
*/

using namespace std;
random_device rd;
mt19937 gen(rd());
struct Coordenadas{
    int col;//columna
    int fila;//fila
};

struct Extremos{
    Coordenadas entrada;
    Coordenadas salida;
};
void inicializar_matriz(vector<vector<char>>& matriz,int fila, int col,char valor){
            matriz.assign(fila,vector<char>(col,valor));
        }
void validar_tamano(int& fila,int& col){
    while (fila<3 or col<3 or fila%2==0 or col%2==0){
            cout<<"El tamano de la matriz debe ser mayor a 3x3 e impar. Ingrese nuevamente: "<<endl;
            cout<<"Ingrese el numero de fila: ";
            cin>>fila;
            cout<<endl<<"Ingrese el numero de columnas: ";
            cin>>col;
        }
}

void mostrar_matriz(const vector<vector<char>>& matriz){
    int fila=matriz.size();
    int col=matriz[0].size();
    for (int i=0;i<fila;i++){
        for (int j=0;j<col;j++){
            cout<<matriz[i][j]<<"  ";
        }
        cout<<endl;
    }
}

void mezclar_vector(vector<Coordenadas>& vecinos){
        for (int i = vecinos.size() - 1; i > 0; i--) {
        uniform_int_distribution<int> dist(0, i);
        int j = dist(gen);
        Coordenadas temp = vecinos[i];
        vecinos[i] = vecinos[j];
        vecinos[j] = temp;
    }
}
bool es_interior(Coordenadas pos,int fila,int col){
    if (pos.col>0 && pos.col<col-1 && pos.fila>0 && pos.fila<fila-1){
        return true;
    }
    else {
        return false;
    }
} 

vector<Coordenadas> obtener_vecinos(Coordenadas pos){
    vector<Coordenadas> vecinos;
    vecinos.push_back({pos.col+2,pos.fila}); // derecha
    vecinos.push_back({pos.col-2,pos.fila}); // izquierda
    vecinos.push_back({pos.col,pos.fila+2}); // abajo
    vecinos.push_back({pos.col,pos.fila-2}); // arriba
    return vecinos;
}

void generar_laberinto(vector<vector<char>>& matriz,int fila,int col){
    inicializar_matriz(matriz,fila,col,'#');
    Coordenadas inicio={1,1};
    Coordenadas fin={col-2,fila-2};
    matriz[inicio.fila][inicio.col]='.';
    stack<Coordenadas> pila= stack<Coordenadas>();
    pila.push(inicio);
    while (!pila.empty()){
        Coordenadas actual=pila.top();
        vector<Coordenadas> candidatos=vector<Coordenadas>();
        vector<Coordenadas> vecinos=obtener_vecinos(actual);
        for(int i=0;i<(int)vecinos.size();i++){
            if (es_interior(vecinos[i],fila,col) && matriz[vecinos[i].fila][vecinos[i].col]=='#'){
                candidatos.push_back(vecinos[i]);
            }
        }
        if (!candidatos.empty()){
            mezclar_vector(candidatos);
            Coordenadas siguiente=candidatos[0];
            int medio_fila=(actual.fila+siguiente.fila)/2;
            int medio_col=(actual.col+siguiente.col)/2;
            matriz[medio_fila][medio_col]='.';
            matriz[siguiente.fila][siguiente.col]='.';
            pila.push(siguiente);
        }
        else{
                pila.pop();
            }
            
            
        }
    matriz[fin.fila][fin.col]='S';
    matriz[inicio.fila][inicio.col]='E';
    }
Extremos buscar_entrada(const vector<vector<char>>& matriz){
    vector<Coordenadas> entrada_salida;
    Extremos extremos;
    for (int i=0;i<(int)matriz.size();i++){
        for (int j=0;j<(int)matriz[0].size();j++){
            if (matriz[i][j]=='E'){
                extremos.entrada={j,i};
            }
            if (matriz[i][j]=='S'){
                extremos.salida={j,i};
            }
        }
    }
    return extremos;
}

void reconstruir_camino(vector<vector<char>>& matriz,vector<vector<Coordenadas>>& padres,Extremos extremos){
    Coordenadas actual=extremos.salida;
    while (actual.col!=extremos.entrada.col || actual.fila!=extremos.entrada.fila){
        Coordenadas p=padres[actual.fila][actual.col];
        if (p.col==-1 && p.fila==-1){
            break;
        }
        if(!(p.col == extremos.entrada.col && p.fila == extremos.entrada.fila)){
            matriz[p.fila][p.col]='R';
        }
        actual=p;
    }
}

string resolver_laberinto(vector<vector<char>>& matriz){
    Extremos extremos=buscar_entrada(matriz);
    queue<Coordenadas> cola;
    vector<vector<bool>> visitados;
    visitados.assign(matriz.size(),vector<bool>(matriz[0].size(),false));
    vector<vector<Coordenadas>> padres;
    padres.assign(matriz.size(),vector<Coordenadas>(matriz[0].size(),{-1,-1}));
    visitados[extremos.entrada.fila][extremos.entrada.col]=true;
    vector<Coordenadas>direcciones={{1,0},{-1,0},{0,1},{0,-1}};
    cola.push(extremos.entrada);
    while (!cola.empty()){
        Coordenadas actual=cola.front();
        cola.pop();
        if (actual.col==extremos.salida.col && actual.fila==extremos.salida.fila){
            break;
        }
        for (int i=0;i<(int)direcciones.size();i++){
            Coordenadas siguiente={actual.col+direcciones[i].col,actual.fila+direcciones[i].fila};
            if (siguiente.col>=0 && siguiente.col<(int)matriz[0].size() && siguiente.fila>=0 && siguiente.fila<(int)matriz.size()){
                if (!visitados[siguiente.fila][siguiente.col]&&(matriz[siguiente.fila][siguiente.col]=='.'||matriz[siguiente.fila][siguiente.col]=='S')){
                    visitados[siguiente.fila][siguiente.col]=true;
                    padres[siguiente.fila][siguiente.col]=actual;
                    cola.push(siguiente);
                }
            }
        }     
    }
    if (visitados[extremos.salida.fila][extremos.salida.col]==false){
        return "No hay solución";
    }
    reconstruir_camino(matriz,padres,extremos);
    return "Camino encontrado";
}
int main(){
    int fila=0,col=0;
    validar_tamano(fila,col);

    vector<vector<char>> entorno(fila,vector<char>(col));

    using namespace chrono;

    auto t1 = high_resolution_clock::now();
    generar_laberinto(entorno,fila,col);
    auto t2 = high_resolution_clock::now();

    auto gen_us = duration_cast<microseconds>(t2 - t1).count();

    mostrar_matriz(entorno);
    cout << endl << "Laberinto generado\n";

    cout << endl << "Resolviendo el laberinto. . . " << endl;

    auto t3 = high_resolution_clock::now();
    string resultado = resolver_laberinto(entorno);
    auto t4 = high_resolution_clock::now();

    auto sol_us = duration_cast<microseconds>(t4 - t3).count();

    cout << resultado << endl;
    mostrar_matriz(entorno);

    cout << "\nTiempo generacion: " << gen_us << " microsegundos";
    cout << "\nTiempo resolucion: " << sol_us << " microsegundos\n";

    return 0;
}
