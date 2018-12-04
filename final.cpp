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
    // cerr << s.first << " piroca " << d << " via " << s.second << "\n";
    if (s.first==d){ 
        // cerr<<"ASDF\n";
        taNumCaminho[s.first] = true;
        auto it2 = path.crbegin(); 
        for(; *it2 != s; it2++);
        it2--;
        if(it2 == path.crend()) {
            concatenate(s, -1, pontos);
        }
        else {
            concatenate(s,(*it2).first,pontos);
        }
    } 
    else //Se nao chegou no destino continua a dfs 
    { 
        for (int i = 0; i < listadj[s.first].size(); i++) 
            if (!visitados[listadj[s.first][i].first]) {
                if(taNumCaminho[listadj[s.first][i].first]) {
                    //cerr<<"Otimizou\n";
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

void AcharCaminhos(int s, int d, int nVertices, vector<vector<pair<int,int> > > &listadj, vector<int>& vAnswer, vector<int>& eAnswer) { 
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
        // cout << (*it).first << "\n";
        vAnswer.push_back((*it).first);
        it++;
        for(; it!=pontoss[s][i].cend(); it++) {
            if((*it).first==12) cerr << 12 << " " << (*it).second << "\n";
            // cout << (*it).second << "-\n" << (*it).first << "\n";
            vAnswer.push_back((*it).first);
            eAnswer.push_back((*it).second);
        }
        //cout << "\n";
    }
}

int main() {
    // ifstream ifs("../CasosDeTeste/grafo_111KV_1ME/data.txt");
    ifstream ifs("SampleDataset1/SampleDataset1.json");
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
        
        //cerr << from << " " << vertexCode[from] << "\n";
        //cerr << to << " " << vertexCode[to] << "\n";

        adjList.resize(vertexCode.size());

        adjList[vertexCode[from]].push_back(make_pair(vertexCode[to],edgeCode[via]));
        adjList[vertexCode[to]].push_back(make_pair(vertexCode[from],edgeCode[via]));

        if(it.second) {
            edgeName.insert(make_pair(i-repetidasName.size(),via));
            //cerr << via << " " << i << "\n";
        }
        else {
            //cerr << via << " " << repetidasName.size() << "\n";
            repetidasName.insert(make_pair(repetidasName.size(),via));
        }
        
    }

    // ifstream start("../CasosDeTeste/grafo_111KV_1ME/startingPoints.txt");
    ifstream start("SampleDataset1/startingpoints.txt");
    vector<int> startingPoints; string startingPoint;
    

    int artificiais=0;
    for(int i=0; start >> startingPoint; i++) {
        auto it = vertexCode.find(startingPoint);
        if(it != vertexCode.cend())
            startingPoints.push_back(vertexCode[startingPoint]);
        else {
            string artificial = "artificial";
            //artificial += (char)artificiais;
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
    for(int i=0; i<adjList.size(); i++) {
        cerr << adjList[i].size() << " ";
        for(int j=0; j<adjList[i].size(); j++) {
            cerr << adjList[i][j].first << " " << adjList[i][j].second << " ";
        }
        cerr << "\n";
    }

    // cout << startingPoints.size() << " ";
    // for(int i=0; i<startingPoints.size(); i++)
    //     cout << startingPoints[i] << " ";
    // cout << "\n";

    const Json::Value& controllers = obj["controllers"]; // array of controllers
    //cout << controllers.size() << " ";
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
            cout << vAnswer[i] << " " << vertexName[vAnswer[i]] << "\n";
        }
        cout << "\n";
        for(int i=0; i<eAnswer.size(); i++) {
            cout << eAnswer[i] << " " << edgeName[eAnswer[i]] << "\n";
            
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