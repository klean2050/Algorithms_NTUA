#include <iostream>
#include <vector>
#include <stack>
using namespace std; int N;

void dfs_check(int v, int visited[], stack<int> &Stack, vector<int> adj[])
{
    visited[v] = 1;
    for (int u:adj[v]) if (!visited[u]) dfs_check(u,visited,Stack,adj);
    Stack.push(v);
}

void dfs(int v, int visited[], vector<int> adj[])
{
    visited[v] = 1;
    for (int u:adj[v]) if (!visited[u]) dfs(u,visited,adj);
}

int dfs_start(int v, vector<int> adj[])
{
    int visited[N], i;
    stack<int> dstack;
    for (i=0 ; i<N ; i++) visited[i] = 0;
    for (i=0 ; i<N ; i++) if (!visited[i]) dfs_check(i,visited,dstack,adj);
    return dstack.top();
}

int main()
{
    std::ios::sync_with_stdio(false);
    int d, k, i, j, v, res=0;
    cin >> N;
    vector<int> wins[N], loses[N];
    int visited[N];
    for (i=0 ; i<N ; i++) {
        cin >> d;
        for (j=0 ; j<d ; j++) {
            cin >> k;
            wins[k-1].push_back(i);
        }
    }
    v = dfs_start(0,wins);
    for (i=0 ; i<N ; i++) for (int u:wins[i]) loses[u].push_back(i);

    for (i=0 ; i<N ; i++) visited[i]=0; dfs(v,visited,wins);
    for (i=0 ; i<N ; i++) if (!visited[i]) { cout << 0 << endl; return 0; } // there's a team that cannot lose for sure
    
    for (i=0 ; i<N ; i++) visited[i] = 0; dfs(v,visited,loses);
    for (i=0 ; i<N ; i++) if (visited[i]) res++; // teams that can win for sure
    cout << res << endl;
    return 0;
}
