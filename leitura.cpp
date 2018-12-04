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

        auto it = edgeCode.insert(make_pair(via,i));
        vertexCode.insert(make_pair(from,vertexCode.size()));
        vertexCode.insert(make_pair(to,vertexCode.size()));
        
        //cerr << from << " " << vertexCode[from] << "\n";
        //cerr << to << " " << vertexCode[to] << "\n";

        adjList.resize(vertexCode.size());

        adjList[vertexCode[from]].push_back(make_pair(vertexCode[to],edgeCode[via]));
        adjList[vertexCode[to]].push_back(make_pair(vertexCode[from],edgeCode[via]));

        if(it.second) {
            edgeName.insert(make_pair(i,via));
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
            artificial += (char)artificiais;
            vertexCode.insert(make_pair(artificial,vertexCode.size()));
            vertexName.insert(make_pair(vertexCode.size(),artificial));

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
        cout << vertexCode[to] << " ";
    }
    cout << endl;

    if(true) {
        ifstream end("SampleDataset1/output.txt");
        string read;
        while(end >> read) {
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
        }
    }

    return 0;
}