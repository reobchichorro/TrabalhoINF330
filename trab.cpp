#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stack>
#include <list>
#include <cmath>
#include <sstream>
#include <queue>
#include <cstdlib>
using namespace std;

static bool valido;
static bool invalido;

void printPath(vector<int> caminho, vector <pair<string, double> > &pontos ){
    for (int i=0;i<caminho.size();i++)
        for(int j=0;j<pontos.size();j++)
            if (caminho[i]==pontos[j].second){
                cout<<pontos[j].first<<"\n";
            }
}

//void DFSPIKA (int v,vector <pair<string, double> > &pontos, vector < vector<int> > &listadj, vector <bool> &arestasUtilizadas, vector<int> &caminho) {
//     if (arestasUtilizadas[v]==true){
//         caminho.pop_back();
//         return;
//     }
//     for(int i=0;i<pontos.size();i++){
//         if (v==pontos[i].second&&pontos[i].first[0]=='c'){
//             printPath(caminho, pontos);
//             caminho.pop_back();
//             valido=true;
//             return;
//         }    
//     }
//     if (valido){
//         int VizinhosVistos=0;
//         for (auto & verti : pontos)
//             if(arestasUtilizadas[verti.second]==true)
//                 VizinhosVistos++;
//         if (VizinhosVistos<listadj[v].size()){
//             valido=false;
//         }
//         if (VizinhosVistos==listadj[v].size())
//             arestasUtilizadas[v]==false;
//             caminho.pop_back();
//             return;
//     }
//}

bool ehSimples(vector <int> &path){
    for (int i=0;i<path.size();i++)
        for (int j=i+1;j<path.size();j++)
            if(path[i]==path[j])
                return false;
    return true;
}

//Funcao recursiva que encontra e imprime os caminhos 's'(start) para 'd'(destino). 
// path[] guarda os vértice no caminho e path_index eh o vert atual no caminho 
void AcharCaminhosRecur(int s, int d, vector<bool> &visitados, vector<bool>& taNumCaminho,
    vector <int> path, int &path_index, vector < vector<int> > &listadj, vector<int> &pontos) { 
    //visita o vertice atual e o coloca no caminho
    visitados[s] = true; 
    path[path_index] = s; 
    path_index++; 
    //Se chegou no destino checa se eh simples, se for imprime se nao continua
    cerr << s << " piroca " << d << "\n";
    if (s==d){ 
        cerr<<"ASDF\n";
        for (int i = 0; i<path_index; i++) {
            taNumCaminho[path[i]] = true;
            cout << path[i] << " ";
        }
        cout << endl; 
    } 
    else //Se nao chegou no destino continua a dfs 
    { 
        for (int i = 0; i < listadj[s].size(); i++) 
            if (!visitados[listadj[s][i]]) {
                if(taNumCaminho[listadj[s][i]]) //print caminho 
                AcharCaminhosRecur(listadj[s][i], d, visitados, taNumCaminho, path, path_index, listadj, pontos); 
            }
    } 
  
    //retira o vertice do caminho e "desvisita"
    path_index--; 
    visitados[s] = false; 
} 

void AcharCaminhos(int Narest, int s, int d, vector<int> &pontos, vector<vector<int> > &listadj) { 
    vector <bool> visitados (pontos.size(),false);
    vector <bool> taNumCaminho (pontos.size(),false); 
    vector <int> path(Narest); 
    int path_index = 0; 
    cerr << "capivara\n";
    AcharCaminhosRecur(s, d, visitados, taNumCaminho, path, path_index, listadj, pontos); 
} 
  

int main() {
	int vertices;
	cin>>vertices;
		
	vector<int> pontos(vertices);
	for(int i=0;i<vertices;i++) {
		cin >> pontos[i];
	}
    int Narestas;
    cin>>Narestas;
	vector< vector<int> >listadj(pontos.size(),vector<int>(0));
	for(int i=0;i<Narestas;i++) {
        int v1,v2;
        cin>>v1>>v2;
        
        listadj[v1].push_back(v2);
        listadj[v2].push_back(v1);
    }
    int size; cin >> size;
    vector<int> start(size);
    for(int i=0;i<start.size();i++)
        cin >> start[i];

    cin >> size;
    vector<int> end(size);
    for(int i=0;i<end.size();i++)
        cin >> end[i];

    for(int i=0;i<start.size();i++)
        for(int j=0;j<end.size();j++) {
            cerr << "capivaraSexy\n";
            AcharCaminhos(Narestas, start[i], end[j], pontos, listadj);
        }
    return 0;
}

