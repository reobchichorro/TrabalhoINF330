#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <queue>
#include <cstdlib>
using namespace std;

//Codigo adaptado da implementacao do livro Competitive Programming
void tarjan(int u, const vector<vector<int> > &adj, vector<int> &dfsParent, vector<int> &dfsNum,
					 vector<int> &dfsLow, vector<bool> &isArticulation,vector<pair<int,int> > &bridges, 
						int &dfsCount, int &rootChildren, int dfsRoot) {
	dfsLow[u] = dfsNum[u] = dfsCount++; // dfsLow[u] <= dfsNum[u]
	for(int v:adj[u]) {
		//vertices nao descobertos tem tempo de descoberto (dfsNum) 0 
		if (dfsNum[v] == 0) { 
				dfsParent[v] = u; //pai de v e' u (v foi descoberto por u)
			if (u == dfsRoot) //caso especial... precisamos contar quantas vezes descobrimos vertices a partir da raiz para descobrir se raiz e' articulacao...
				rootChildren++; 

			tarjan(v,adj,dfsParent,dfsNum,dfsLow,isArticulation,bridges,dfsCount,rootChildren,dfsRoot);

			if (dfsLow[v] >= dfsNum[u]) //apos visitar v recursivamente, conseguimos chegar em um vertice em v ou antes passando por u? (sem voltar pela aresta v,u)
				isArticulation[u] = true; // store this information first
			if (dfsLow[v] > dfsNum[u]) // ponte?
				bridges.push_back(make_pair(u,v));
			dfsLow[u] = min(dfsLow[u], dfsLow[v]); // update dfsLow[u]
		}
		else if (v != dfsParent[u]) //cheguei de volta em um vertice ja descoberto... se nao passei pela aresta v,u de volta --> ciclo...
			dfsLow[u] = min(dfsLow[u], dfsNum[v]); //atualize dfsLow...
	}
}

void tarjan(const vector<vector<int> > &adj, 
						vector<bool> &isArticulation,vector<pair<int,int> > &bridges) {
	int nv = adj.size();
	isArticulation = vector<bool>(nv,false);
	bridges.resize(0);

	vector<int> dfsParent(nv,-1);
	vector<int> dfsLow(nv,0);
	vector<int> dfsNum(nv,0);	
	int dfsCount = 0;

	//para multiplos CCs ...
	for(int i=0;i<nv;i++) {
		if(dfsNum[i]==0) {
			int rootChildren = 0;  //resete para cada CC!!
			tarjan(i,adj,dfsParent,dfsNum,dfsLow,isArticulation,bridges,dfsCount,rootChildren,i);
			isArticulation[i] = rootChildren>1;
		}
	}
}



int main() {
	int nv,ne;
	while(true) {		
		scanf("%d %d",&nv,&ne);
		if (nv+ne==0) return 0;
	
		vector<vector<int> > adj(nv);
		
		for(int i=0;i<ne;i++) {
			int u,v;
			scanf("%d %d",&u,&v);
			u--; v--;
			adj[u].push_back(v);	
			adj[v].push_back(u);	
		}

		vector<bool> isArticulation;
		vector<pair<int,int> > bridges;
		tarjan(adj,isArticulation,bridges);
	
		if (bridges.size()==0) printf("S\n");
		else printf("N\n");
		
	}

}
