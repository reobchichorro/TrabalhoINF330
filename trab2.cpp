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

bool ehSimples(vector <int> &path){
    for (int i=0;i<path.size();i++)
        for (int j=i+1;j<path.size();j++)
            if(path[i]==path[j])
                return false;
    return true;
}

void concatenate(const pair<int,int>& s, const int d, vector<vector<list<pair<int,int> > > > &pontos) {
    if(d == -1) {
        list<pair<int,int> > dummy;
        dummy.push_front(s);
        pontos[s.first].push_back(dummy);
        auto it3=dummy.begin();
        it3++;
        while(it3!=dummy.end()) { 
            dummy.erase(it3);
            it3 = dummy.begin();
            it3++;
        }
    }
    else {
        for(int i=0; i<pontos[d].size(); i++) {
            list<pair<int,int> > dummy;
            dummy.push_front(s);
            dummy.insert(dummy.end(), pontos[d][i].begin(), pontos[d][i].end());
            pontos[s.first].push_back(dummy);
            
            auto it3=dummy.begin();
            it3++;
            while(it3!=dummy.end()) { 
                dummy.erase(it3);
                it3 = dummy.begin();
                it3++;
            }
        }
    }
}

//Funcao recursiva que encontra e imprime os caminhos 's'(start) para 'd'(destino). 
// path[] guarda os vértice no caminho e path_index eh o vert atual no caminho 
void AcharCaminhosRecur(const pair<int,int>& s, int d, vector<bool> &visitados, vector<bool>& taNumCaminho,
    list<pair<int,int> >& path, int &path_index, vector < vector<pair<int,int> > > &listadj, vector<vector<list<pair<int,int> > > > &pontos) { 
    //visita o vertice atual e o coloca no caminho
    visitados[s.first] = true; 
    path.push_back(s); 
    path_index++; 
    //Se chegou no destino checa se eh simples, se for imprime se nao continua
    cerr << s.first << " piroca " << d << " via " << s.second << "\n";
    if (s.first==d){ 
        cerr<<"ASDF\n";
        taNumCaminho[s.first] = true;
        auto it2 = path.crbegin(); 
        for(; *it2 != s; it2++);
        it2--;
        if(it2 == path.crend()) {
            concatenate(s, -1, pontos);
        }
        else {
            cout<<"pirocao\n";
            concatenate(s,(*it2).first,pontos);
        }
    } 
    else //Se nao chegou no destino continua a dfs 
    { 
        for (int i = 0; i < listadj[s.first].size(); i++) 
            if (!visitados[listadj[s.first][i].first]) {
                if(taNumCaminho[listadj[s.first][i].first]) {
                    cerr<<"Otimizou\n";
                    taNumCaminho[s.first] = true;
                    auto it2 = path.crbegin(); 
                    for(; *it2 != s; it2++);
                    it2--;
                    if(it2 == path.crend()) {
                        concatenate(s, listadj[s.first][i].first, pontos);
                    }
                    else {
                        concatenate(s,(*it2).first,pontos);
                    }
                }
                else {
                    AcharCaminhosRecur(listadj[s.first][i], d, visitados, taNumCaminho, path, path_index, listadj, pontos); 
                    if(taNumCaminho[listadj[s.first][i].first]) {
                        taNumCaminho[s.first] = true;
                        concatenate(s,listadj[s.first][i].first,pontos);
                    }
                }
            }
    }

    
  
    //retira o vertice do caminho e "desvisita"
    path_index--;
    path.pop_back();
    visitados[s.first] = false; 
} 

void AcharCaminhos(int s, int d, int nVertices, vector<vector<pair<int,int> > > &listadj) { 
    vector <bool> visitados (nVertices,false);
    vector <bool> taNumCaminho (nVertices,false); 
    list<pair<int,int> > path;
    int path_index = 0; 
    vector<vector<list<pair<int,int> > > > pontoss(nVertices);
    pair<int,int> start(s,-1);
    AcharCaminhosRecur(start, d, visitados, taNumCaminho, path, path_index, listadj, pontoss);
    /*
    for(int i=0; i<pontoss.size(); i++) {
        cout << i << "-" << pontoss[i].size() << " ";
    } */
    for(int i=0; i<pontoss[s].size(); i++) {
        //cout << i+1 << ": ";
        auto it=pontoss[s][i].cbegin();
        cout << (*it).first << "\n";
        it++;
        for(; it!=pontoss[s][i].cend(); it++) {
            cout << (*it).second << "-\n" << (*it).first << "\n";
        }
        //cout << "\n";
    }
} 
  

int main() {
	int vertices;
	cin>>vertices;
	
	vector< vector<pair<int,int> > > listadj(vertices,vector<pair<int,int> >(0));
    int grau; pair<int,int> ligacao;
	for(int i=0;i<vertices;i++) {
        cin >> grau;
        for(int j=0; j<grau; j++) {
            cin >> ligacao.first >> ligacao.second;
            listadj[i].push_back(ligacao);
        }
    }
    int size; cin >> size;
    vector<int> start(size);
    for(int i=0;i<start.size();i++)
        cin >> start[i];

    cin >> size;
    vector<int> end(size);
    for(int i=0;i<end.size();i++)
        cin >> end[i];

    for(int j=0;j<end.size();j++) {
        for(int i=0;i<start.size();i++) {
            cerr << start[i] << " capivaraSexy " << end[j] << "\n";   
            AcharCaminhos(start[i], end[j], vertices, listadj);
        }
    }
    return 0;
}

//end*start*