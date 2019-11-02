using namespace std;
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

struct edge {
	int cost, nodea, nodeb;
}; vector<edge> edges;

bool comp(edge a,edge b)
{
    return a.cost < b.cost;
}

struct set {
	int parent;
	int rank;
}; set s[50000];

void makeset(int N)
{
	for (int n=0 ; n<N ; n++) {
		s[n].parent = n;
		s[n].rank = 0;
	}
}

int find(int n)
{
	if (s[n].parent != n) s[n].parent = find(s[n].parent);
	return s[n].parent;
}

void unite(int n1, int n2)
{
	n1 = find(n1); n2 = find(n2); // enables union many times in order
	if (s[n1].rank < s[n2].rank) s[n1].parent = n2;
	else {
		s[n2].parent = n1;
		if (s[n1].rank == s[n2].rank) s[n1].rank++;
	}
}

// bridges() & dfs() implement algorithm for finding bridges in a graph
int count1=0, t=0;
void dfs(map<int,vector<int>>& graph, int v, vector<int>& disc, vector<int>& low, int parent, map<int,int>& visited, map<pair<int,int>,int>& f)
{
	visited[v] = 1;
	disc[v] = low[v] = ++t;
	for (int w : graph[v]) {
		if (!visited[w]) {
			dfs(graph,w,disc,low,v,visited,f);
			low[v] = min(low[v],low[w]);
			if (low[w]>disc[v] && f[make_pair(w,v)]==1) count1++; // found a bridge => (w,v) appears in every MST
		} else if (w != parent) low[v] = min(low[v],disc[w]);
	}
}

void bridges(map<int,vector<int>>& graph, map<pair<int,int>,int>& f, map<int,int>& visited)
{
	int size = 50000;
	vector<int> low(size), disc(size);
	for (auto const& p : visited) if (!p.second) dfs(graph,p.first,disc,low,-1,visited,f);
}

int main()
{
	int i, a, b, c, k, N, M, seta, setb, count2=0;
	std::ios::sync_with_stdio(false); // faster cin
	cin >> N >> M;
	makeset(N); // disjoint sets for Kruskal
	
	// sorted array of edges for Kruskal
	for (i=0 ; i<M ; i++) {
		cin >> a >> b >> c;
		edge e = {.cost = c, .nodea = a-1, .nodeb = b-1};
		edges.push_back(e);
	}
	sort(edges.begin(),edges.end(),comp);
	
	// Kruskal selection
	vector<pair<int,int>> unite_keeper; // used for saving unions that must be made
	map<int,vector<int>> adj;    		// used for finding bridges
	map<int,int> visited;				// used for DFS in finding-bridges graph
	map<pair<int,int>,int> f;	 		// used for detecting multiple edges in finding-bridges graph
	
	i=0;
	while (i<M) {
		// initialisation for new edges
		unite_keeper.clear();	
		visited.clear();
		adj.clear();
		f.clear();
		do {
			seta = find(edges[i].nodea);
			setb = find(edges[i].nodeb);
			if (seta != setb) {
				// put to block of same-cost edges (with united sets) and initialise visited[] for DFS
				adj[seta].push_back(setb);
				adj[setb].push_back(seta);
				visited[seta] = visited[setb] = 0;
				// check if there is already this edge in the block
				pair<int,int> p1 = make_pair(seta,setb);
				pair<int,int> p2 = make_pair(setb,seta);
				if (f.find(p1) != f.end()) f[p1]++;
				else f[p1] = 1;
				if (f.find(p2) != f.end()) f[p2]++;
				else f[p2] = 1;
				// save this pair of sets in order to unite them later
				unite_keeper.push_back(p1);
			} else count2++; // connecting inside a set => does not appear in any MST
			i++;
		} while (i!=M && edges[i-1].cost==edges[i].cost); // repeat for all same-cost edges
		
		// count bridges at same-cost edges => could replace this edge in another MST
		bridges(adj,f,visited);
		// make the unions for all the same-cost edges and proceed to the next edges
		for (k=0 ; k<unite_keeper.size() ; k++) unite(unite_keeper[k].first,unite_keeper[k].second);
	}
	
	cout << count1 << endl << count2 << endl << M-count1-count2 << endl;
	return 0;
}
