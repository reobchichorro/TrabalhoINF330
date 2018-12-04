#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <jsoncpp/json/json.h>
using namespace std;

struct Edge 
{
	int v, w;
	Edge(int v = -1, int w = -1) : v(v), w(w) { }
};

int main() {
    ifstream ifs("../CasosDeTeste/grafo_111KV_1ME/data.txt");
    // ifstream ifs("SampleDataset1/SampleDataset1.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    const Json::Value& rows = obj["rows"]; // array of rows
    map<string,int> aresss, vertexxx;
    vector<Edge> arestas, repeated;
    map<int,string> repetidas;

    vector<vector<pair<int,int> > > adjList;

    string from, to, via;
    Edge lida;
    for (int i = 0; i < rows.size(); i++){

        via = rows[i]["viaGlobalId"].asString();
        from = rows[i]["fromGlobalId"].asString();
        to = rows[i]["toGlobalId"].asString();

        auto it = aresss.insert(make_pair(via,i));
        vertexxx.insert(make_pair(from,vertexxx.size()));
        vertexxx.insert(make_pair(to,vertexxx.size()));
        
        adjList.resize(vertexxx.size());

        adjList[vertexxx[from]].push_back(make_pair(vertexxx[to],aresss[via]));
        adjList[vertexxx[to]].push_back(make_pair(vertexxx[from],aresss[via]));

        if(it.second);
        else {
            repetidas.insert(make_pair(repetidas.size(),via));
        }
        
    }

    ifstream start("../CasosDeTeste/grafo_111KV_1ME/startingPoints.txt");
    // ifstream start("SampleDataset1/startingpoints.txt");
    vector<int> startingPoints; string startingPoint;
    

    int artificiais=0;
    for(int i=0; start >> startingPoint; i++) {
        auto it = vertexxx.find(startingPoint);
        if(it != vertexxx.cend())
            startingPoints.push_back(vertexxx[startingPoint]);
        else {
            string artificial = "artificial";
            artificial += (char)artificiais;
            vertexxx.insert(make_pair(artificial,vertexxx.size()));

            auto it = aresss.find(startingPoint);
            startingPoints.push_back(vertexxx.size()-1);            
            
            int size = adjList.size();
            adjList.resize(size+artificiais+1);
            for(int j=0; j<size; j++) {
                bool achou = false;
                for(int k=0; k<adjList[j].size(); k++) {
                    
                    if(adjList[j][k].second == (*it).second) {
                        if(achou) adjList[j].erase(adjList[j].begin() + k);
                        else {
                            achou = true;
                            adjList[j][k].first = vertexxx.size()-1;
                            adjList[j][k].second = aresss.size() + artificiais;

                            adjList[vertexxx.size()-1].push_back(make_pair(j,aresss.size() + artificiais));

                            artificiais++; 
                        }
                    }
                }
            }
        }
    }

    cout << adjList.size() << "\n";
    for(int i=0; i<adjList.size(); i++) {
        cout << adjList[i].size() << " ";
        for(int j=0; j<adjList[i].size(); j++) {
            cout << adjList[i][j].first << " " << adjList[i][j].second << " ";
        }
        cout << "\n";
    }

    cout << startingPoints.size() << " ";
    for(int i=0; i<startingPoints.size(); i++)
        cout << startingPoints[i] << " ";
    cout << "\n";
    const Json::Value& controllers = obj["controllers"]; // array of controllers
    cout << controllers.size() << " ";
    for (int i = 0; i < controllers.size(); i++){
        to = controllers[i]["globalId"].asString();
        cout << vertexxx[to] << " ";
    }
    cout << endl;

    return 0;
}