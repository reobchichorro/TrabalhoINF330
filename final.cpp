#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <iomanip>
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

struct Edge 
{
	int v, w;
	Edge(int v = -1, int w = -1) : v(v), w(w) { }
};

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

//Funcao recursiva que encontra e imprime os caminhos 's'(start) para 'd'(destino). 
// path[] guarda os vértice no caminho e path_index eh o vert atual no caminho 
void AcharCaminhosRecur(const pair<int,int>& s, int d, vector<bool> &visitados, vector<bool>& taNumCaminho, vector < vector<pair<int,int> > > &listadj, vector<int> &eNumCaminho) { 
    //visita o vertice atual e o coloca no caminho
    visitados[s.first] = true; 
    //Se chegou no destino checa se eh simples, se for imprime se nao continua
    // cerr << s.first << " piroca " << d << " via " << s.second << "\n";
    if (s.first==d){ 
        // cerr<<"ASDF\n";
        taNumCaminho[s.first] = true;
        eNumCaminho.push_back(s.second);
    } 
    else { //Se nao chegou no destino continua a dfs  
        for (int i = 0; i < listadj[s.first].size(); i++) 
            if (!visitados[listadj[s.first][i].first]) {
                if(taNumCaminho[listadj[s.first][i].first]) {
                    // cerr<<"Otimizou\n";
                    //pontos[listadj[s.first][i].first];
                    // auto k = listadj[s.first][i];

                    // for(int j=0; j<pontos[k.first].size(); j++) {
                    //     (*pontos[k.first][j].begin()).second = k.second;
                    // }

                    taNumCaminho[s.first] = true;
                    eNumCaminho.push_back(s.second);
                }
                else {
                    AcharCaminhosRecur(listadj[s.first][i], d, visitados, taNumCaminho, listadj, eNumCaminho); 
                    if(taNumCaminho[listadj[s.first][i].first]) {
                        taNumCaminho[s.first] = true;
                        eNumCaminho.push_back(s.second);
                    }
                }
            }
    }
    //retira o vertice do caminho e "desvisita"
    visitados[s.first] = false; 
} 

void AcharCaminhos(int s, int d, int nVertices, vector<vector<pair<int,int> > > &listadj, vector<int>& vAnswer, vector<int>& eAnswer) { 
    vector <bool> visitados (nVertices,false);
    vector <bool> taNumCaminho (nVertices,false); 
    vector<int> eNumCaminho;
    pair<int,int> start(s,-1);
    AcharCaminhosRecur(start, d, visitados, taNumCaminho, listadj, eNumCaminho);
    /*
    for(int i=0; i<pontoss.size(); i++) {
        cout << i << "-" << pontoss[i].size() << " ";
    } */
    for(int i=0; i<taNumCaminho.size(); i++) {
        //cout << i+1 << ": ";
        if(taNumCaminho[i])
            vAnswer.push_back(i);
    }
    for(int i=0; i<eNumCaminho.size(); i++) {
        eAnswer.push_back(eNumCaminho[i]);
    }
}

int main() {
    // ifstream ifs("../CasosDeTeste/EsriNapervilleElectricNetwork/EsriNapervilleElectricNetwork.json");
    ifstream ifs("../CasosDeTeste/wupstream/patherdosrenyi_10_500/network.json");
    // ifstream ifs("SampleDataset1/SampleDataset1.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    const Json::Value& rows = obj["rows"]; // array of rows
    map<string,int> edgeCode, vertexCode;
    map<int,string> edgeName, vertexName;
    map<int,string> repetidasName;

    vector<vector<pair<int,int> > > adjList;

    string from, to, via;
    Edge lida;
    for (int i = 0; i < rows.size(); i++){

        via = rows[i]["viaGlobalId"].asString();
        from = rows[i]["fromGlobalId"].asString();
        to = rows[i]["toGlobalId"].asString();

        auto it = edgeCode.insert(make_pair(via,i-repetidasName.size()));

        auto itV = vertexCode.insert(make_pair(from,vertexCode.size()));
        if(itV.second)
            vertexName.insert(make_pair(vertexCode.size()-1,from));
        itV = vertexCode.insert(make_pair(to,vertexCode.size()));
        if(itV.second)
            vertexName.insert(make_pair(vertexCode.size()-1,to));

        adjList.resize(vertexCode.size());

        adjList[vertexCode[from]].push_back(make_pair(vertexCode[to],edgeCode[via]));
        adjList[vertexCode[to]].push_back(make_pair(vertexCode[from],edgeCode[via]));

        if(it.second) {
            edgeName.insert(make_pair(i-repetidasName.size(),via));
        }
        else {
            repetidasName.insert(make_pair(repetidasName.size(),via));
        }
        
    }
    //cerr << "acabou de ler" << endl;
    // ifstream start("../CasosDeTeste/EsriNapervilleElectricNetwork/startingpoints.txt");
    ifstream start("../CasosDeTeste/wupstream/patherdosrenyi_10_500/start.txt");
    // ifstream start("SampleDataset1/startingpoints.txt");
    vector<int> startingPoints; string startingPoint;
    

    int artificiais=0;
    for(int i=0; start >> startingPoint; i++) {
        auto it = vertexCode.find(startingPoint);
        if(it != vertexCode.cend())
            startingPoints.push_back(vertexCode[startingPoint]);
        else {
            string artificial = "artificial";
            vertexName.insert(make_pair(vertexCode.size(),startingPoint));
            vertexCode.insert(make_pair(artificial,vertexCode.size()));

            auto it = edgeCode.find(startingPoint);
            startingPoints.push_back(vertexCode.size()-1);            
            
            int size = adjList.size();
            adjList.resize(size+artificiais+1);
            for(int j=0; j<size; j++) {
                bool achou = false;
                for(int k=0; k<adjList[j].size(); k++) {
                    
                    if(adjList[j][k].second == (*it).second) {
                        if(achou) adjList[j].erase(adjList[j].begin() + k);
                        else {
                            achou = true;
                            adjList[j][k].first = vertexCode.size()-1;
                            adjList[j][k].second = edgeCode.size() + artificiais;
                            
                            adjList[vertexCode.size()-1].push_back(make_pair(j,edgeCode.size() + artificiais));

                            artificiais++; 
                        }
                    }
                }
            }
        }
    }

    cerr << adjList.size() << "\n";
    // for(int i=0; i<adjList.size(); i++) {
    //     cerr << adjList[i].size() << " ";
    //     for(int j=0; j<adjList[i].size(); j++) {
    //         cerr << adjList[i][j].first << " " << adjList[i][j].second << " ";
    //     }
    //     cerr << "\n";
    // }

    // cout << startingPoints.size() << " ";
    // for(int i=0; i<startingPoints.size(); i++)
    //     cout << startingPoints[i] << " ";
    // cout << "\n";

    const Json::Value& controllers = obj["controllers"]; // array of controllers
    cerr << controllers.size() << " cont" << endl;
    vector<int> end(controllers.size());
    for (int i = 0; i < controllers.size(); i++){
        to = controllers[i]["globalId"].asString();
        //cout << vertexCode[to] << " ";
        end[i] = vertexCode[to];
    }
    //cout << endl;


    vector<int> vAnswer; vector<int> eAnswer;

    for(int j=0;j<end.size();j++) {
        for(int i=0;i<startingPoints.size();i++) {
            //cerr << startingPoints[i] << " capivaraSexy " << end[j] << "\n";   
            AcharCaminhos(startingPoints[i], end[j], adjList.size(), adjList, vAnswer, eAnswer);
        }
    }

    if(true) {
        for(int i=0; i<vAnswer.size(); i++) {
            //if(vertexName[vAnswer[i]] == "artificial")
            // cout << vAnswer[i] << " " << vertexName[vAnswer[i]] << "\n";
            cout << vertexName[vAnswer[i]] << "\n";
        }
        for(int i=0; i<eAnswer.size(); i++) {
            if(eAnswer[i]<edgeName.size())
            cout << edgeName[eAnswer[i]] << "\n";
            // cout << eAnswer[i] << " " << edgeName[eAnswer[i]] << "\n";
            /*
            auto it = vertexCode.find(read);
            if(it != vertexCode.cend()) 
                cerr << (*it).second << "\n";
            else {
                auto it2 = edgeCode.find(read);
                if(it != edgeCode.cend()) 
                    cerr << (*it2).second << " -\n";
                else
                    for(int i=0; i<repetidasName.size(); i++) 
                        if(repetidasName[i] == read) {
                            cerr << i << " -\n";
                            repetidasName.erase(i);
                        }
            }

            */
        }
    }

    return 0;
}