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


//Funcao recursiva que encontra e imprime os caminhos 's'(start) para 'd'(destino). 
// path[] guarda os vértice no caminho e path_index eh o vert atual no caminho 
void AcharCaminhosRecur(const pair<int,int>& s, int d, vector<bool> &visitados, vector<bool>& taNumCaminho, vector < vector<pair<int,int> > > &listadj, vector<int> &eNumCaminho) { 
    //visita o vertice atual e o coloca no caminho
    visitados[s.first] = true; 
    //Se chegou no destino checa se eh simples, se for imprime se nao continua
    if (s.first==d){
        taNumCaminho[s.first] = true;
        eNumCaminho.push_back(s.second);
    } 
    else { //Se nao chegou no destino continua a dfs  
        for (int i = 0; i < listadj[s.first].size(); i++) 
            if (!visitados[listadj[s.first][i].first]) {
                if(taNumCaminho[listadj[s.first][i].first]) {
                    // Otimizou
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

// funcao base para chamar a funcao recursiva de gerar os caminhos simples possiveis
void AcharCaminhos(int s, int d, int nVertices, vector<vector<pair<int,int> > > &listadj, vector<int>& vAnswer, vector<int>& eAnswer) { 
    vector <bool> visitados (nVertices,false);      // vetor para armazenar vertices visitados na dfs
    vector <bool> taNumCaminho (nVertices,false);   // vetor para armazenar se um vertice esta num caminho ate o controller
    vector<int> eNumCaminho;                        // vetor que armazenas todas as arestas que estao num caminho de s para d
    pair<int,int> start(s,-1);                      // vertice inicial
    AcharCaminhosRecur(start, d, visitados, taNumCaminho, listadj, eNumCaminho);// funcao recursiva que passa o vertice inicial,
    // o controlador, vetor para guardar os vertices visitado, vetor para armazenar se o vertice esta num caminho
    //, a lista de adjacencia, e o vertice para armazenar, as arestas que estao em algum caminho
    for(int i=0; i<taNumCaminho.size(); i++) {  //para todos os vertices que estiverem am algum caminho, elas
        if(taNumCaminho[i])                     //serao adicionadas no vetor contendo as respostas
            vAnswer.push_back(i);                                               
    }                                                                          
    for(int i=0; i<eNumCaminho.size(); i++) {   //para todos os arestas que estiverem am algum caminho, elas
        eAnswer.push_back(eNumCaminho[i]);      //serao adicionadas no vetor contendo as respostas
    }
}

int main(int argc, char *argv[]) {
    ifstream ifs(argv[1]);     //Receber o arquivo json com os dados
    ofstream ofs("saida.txt");

    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // declaracao do reader
    const Json::Value& rows = obj["rows"]; // leitura do objeto rows
    map<string,int> edgeCode, vertexCode;  //map que contem o codigo das arestas e vertices, juntamente com os inteiros correspondentes
    map<int,string> edgeName, vertexName;  //map que contem o nome das arestas e vertices, juntamente com o nome passado no json
    map<int,string> repetidasName;         //map para guardar as arestas que aparecem com mesmo nome

    vector<vector<pair<int,int> > > adjList; //lista de adjacencia

    string from, to, via;   // strings para guardar o vertice de origem, o vertice de destino e a aresta que os liga
    for (int i = 0; i < rows.size(); i++){

        via = rows[i]["viaGlobalId"].asString();   //realiza leitura do json
        from = rows[i]["fromGlobalId"].asString();
        to = rows[i]["toGlobalId"].asString();

        auto it = edgeCode.insert(make_pair(via,i-repetidasName.size()));  //insere a aresta em edgeCode, juntamente com o codigo (inteiro)
                                                                           //que sera i menos o numero de arestas repetidas
        auto itV = vertexCode.insert(make_pair(from,vertexCode.size()));   //insere a aresta em vertexCode, juntamente com o codigo (inteiro)
        if(itV.second)                                                     //que sera o tamanho do vetor que contem os vertices
            vertexName.insert(make_pair(vertexCode.size()-1,from));        //se a insercao for bem sucedida, inserir em vertex name a aresta de origem
        itV = vertexCode.insert(make_pair(to,vertexCode.size()));          //inserir em vertexCode o codigo da aresta de origem juntamente com seu id
        if(itV.second)                                                     //se a insercao for bem sucedida, inserir em vertex name a aresta de destino
            vertexName.insert(make_pair(vertexCode.size()-1,to));          //inserir em vertexCode o codigo da aresta de destino juntamente com seu id

        adjList.resize(vertexCode.size());                                 //reajustar o tamnho da lista para o tamanho do vetor que contem os vertices

        adjList[vertexCode[from]].push_back(make_pair(vertexCode[to],edgeCode[via])); //inserir na lista de adjacencia os vertices
        adjList[vertexCode[to]].push_back(make_pair(vertexCode[from],edgeCode[via])); //grafo bidirecionado

        if(it.second) {                                                 //caso a insercao da aresta ocorra, inserir a aresta no vetor de arestas
            edgeName.insert(make_pair(i-repetidasName.size(),via));
        }
        else {  //caso a insercao retorne false, significa que ja existe aquela aresta no map, logo ela sera uma copia
            repetidasName.insert(make_pair(repetidasName.size(),via));
        }
        
    }
    
    ifstream start(argv[2]);  //abertura do arquivo que contem os starting points
    vector<int> startingPoints; string startingPoint; //vetor para armazenar os starting points
    

    int artificiais=0; //contador de arestas artificiais
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

    cerr << "Numero de vertices: " << vertexCode.size() << "\n";
    cerr << "Numero de arestas: " << edgeCode.size() << "\n";
    const Json::Value& controllers = obj["controllers"];
    cerr << "Numero de controladores: " << controllers.size() << "\n";
    cerr << "Numero de starting points: " << startingPoints.size() << "\n";

    vector<int> end(controllers.size());
    for (int i = 0; i < controllers.size(); i++){
        to = controllers[i]["globalId"].asString();
        end[i] = vertexCode[to];
    }


    vector<int> vAnswer; vector<int> eAnswer;

    for(int j=0;j<end.size();j++) {
        for(int i=0;i<startingPoints.size();i++) {
            AcharCaminhos(startingPoints[i], end[j], adjList.size(), adjList, vAnswer, eAnswer);
        }
    }

    if(true) {
        for(int i=0; i<vAnswer.size(); i++) {
            ofs << vertexName[vAnswer[i]] << "\n";
        }
        for(int i=0; i<eAnswer.size(); i++) {
            if(eAnswer[i]<edgeName.size())
            ofs << edgeName[eAnswer[i]] << "\n";
        }
    }

    ifs.close();
    ofs.close();
    start.close();
    return 0;
}