using namespace std;
#include <iostream>
#include <cstdint>

struct node {
	int64_t paths[101], explored[101], hasp=0, hasx=0, hasy=0;
	pair<int64_t,int64_t> coord, childp;
};
int64_t C = 1000000103;
node* grid = new node[180*180];
int64_t s[100], e[100];

int find_paths(node& n, int64_t X, int64_t N)
{
	// backtrack of DFS (DP)
	if (n.explored[X]) return n.paths[X];
	n.explored[X] = 1;
	
	// leaf
	if (n.coord.first == 0 && n.coord.second == 0) {
		if (X>=0) n.paths[X] = 1;
		return n.paths[X];
	}
	
	if (n.hasp) {
		if (X!=0) n.paths[X] = find_paths(grid[N*n.childp.first+n.childp.second], X-1, N)%C;
		else return 0;
	}
	else {
		if (n.hasx) n.paths[X] = find_paths(grid[N*n.coord.first+n.coord.second-1], X, N)%C;
		if (n.hasy) n.paths[X] = n.paths[X] + find_paths(grid[N*n.coord.first+n.coord.second-N], X, N)%C;
	}
	return n.paths[X]%C;
}

int main()
{
	int64_t i, j, k, N, M, K, X;
	std::ios::sync_with_stdio(false); // faster cin
	cin >> N >> M >> K >> X;
	for (i=0 ; i<K ; i++) cin >> s[i] >> e[i];
	
	// make grid-tree
	for (i=N-1 ; i>=0 ; i--) for (j=M-1 ; j>=0 ; j--) {
		for (k=0 ; k<=X ; k++) {
			grid[i*M+j].explored[k] = 0;
			grid[i*M+j].paths[k] = 0;
		}
		grid[i*M+j].coord = make_pair(i,j);
		if (i!=0) grid[i*M+j].hasy = 1;
		if (j!=0) grid[i*M+j].hasx = 1;
	}
	for (i=0 ; i<K ; i++) {
		grid[M*(s[i]/M)+s[i]%M].childp = make_pair(e[i]/M,e[i]%M);
		grid[M*(s[i]/M)+s[i]%M].hasp = 1;
	}
	
	cout << find_paths(grid[M*(N-1)+M-1], X, M) << endl; // DFS
	return 0;
}
