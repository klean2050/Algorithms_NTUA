#include <iostream>
using namespace std;

long long int inf = 100000000, d[100][100][11];

int main()
{
    long long int N, M, K, Q, u, v, dist, i, j, k, p, pp, q;
    std::ios::sync_with_stdio(false);

    // initialisation
    cin >> N >> M >> K >> Q;
    for (i=0 ; i<N ; i++) for (j=0 ; j<N ; j++) for (k=0 ; k<=K ; k++) d[i][j][k] = inf;
    for (i=0 ; i<N ; i++) for (k=0 ; k<=K ; k++) d[i][i][k] = 0;
    for (i=0 ; i<M ; i++) {
        cin >> u >> v >> dist;
        d[u-1][v-1][0] = dist;
        d[v-1][u-1][1] = dist;
    }
    
    // Floyd Algorithm
    for (p=0 ; p<=K ; p++)
        for (k=0 ; k<N ; k++) for (i=0 ; i<N ; i++) for (j=0 ; j<N ; j++)
            for (pp=0 ; pp<=p ; pp++) d[i][j][p] = min( d[i][j][p] , d[i][k][pp] + d[k][j][p-pp] );
    
    // Answering
    for (q=0 ; q<Q ; q++) {
        cin >> i >> j >> p;
        if (d[i-1][j-1][p]<inf) cout << d[i-1][j-1][p] << endl;
        else cout << "IMPOSSIBLE" << endl;
    }
    return 0;
}