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

    string from, to, via;
    Edge lida;
    for (int i = 0; i < rows.size(); i++){
        via = rows[i]["viaGlobalId"].asString();
        from = rows[i]["fromGlobalId"].asString();
        to = rows[i]["toGlobalId"].asString();

        auto it = aresss.insert(make_pair(via,i));
        vertexxx.insert(make_pair(from,vertexxx.size()));
        vertexxx.insert(make_pair(to,vertexxx.size()));
        lida.v = vertexxx[from];
        lida.w = vertexxx[to];

        //cerr << vertexxx[from] << " " << lida.w << " " << it.second << "\n";

        if(it.second)
            arestas.push_back(lida);
        else {
            cerr << "Repetiu\n";
            repetidas.insert(make_pair(repetidas.size(),via));
            repeated.push_back(lida);
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
            artificiais++;

            auto it = aresss.find(startingPoint);
            Edge artificiaresta;

            artificiaresta.v = vertexxx.size()-1;
            artificiaresta.w = arestas[(*it).second].v;
            arestas.push_back(artificiaresta);
            
            artificiaresta.v = vertexxx.size()-1;
            artificiaresta.w = arestas[(*it).second].w;
            arestas.push_back(artificiaresta);

            arestas[(*it).second].v = arestas[(*it).second].w = -1;
            
            for(int j=0; j<repeated.size(); j++) {
                if(repetidas[j]==startingPoint) { //Aresta que e' startingPoint
                    artificiaresta.v = vertexxx.size()-1;
                    artificiaresta.w = repeated[j].w;
                    repeated[j].v = repeated[j].w = -1;

                }
            }
        }
    }
    
    
    cout << vertexxx.size() << "\n";
    for(int i=0; i<vertexxx.size(); i++)
        cout << i << "\n";
    cout << arestas.size()+repeated.size()-artificiais << "\n";
    for(int i=0; i<arestas.size(); i++) {
        if(arestas[i].v!=-1)
            cout << arestas[i].v << " " << arestas[i].w << "\n";
    }
    for(int i=0; i<repeated.size(); i++) {
        cout << repeated[i].v << " " << repeated[i].w << "\n";
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