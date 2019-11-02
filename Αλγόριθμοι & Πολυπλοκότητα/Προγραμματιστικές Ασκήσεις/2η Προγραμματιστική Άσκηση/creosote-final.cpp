using namespace std;
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <deque>

struct line {
	int64_t A, B;
};

struct intersect {
	double x;
	line next_line;
};
deque <intersect> polygon;

void add_line(line new_line)
{
	intersect new_inter;
	new_inter.next_line = new_line;
	if (!polygon.empty()) {
		new_inter.x = (polygon.front().next_line.B-new_line.B)/(new_line.A-polygon.front().next_line.A);
		while (new_inter.x <= polygon.front().x) {
			polygon.pop_front();
			new_inter.x = (polygon.front().next_line.B - new_line.B)/(new_line.A-polygon.front().next_line.A);	
		}
	} else new_inter.x = 0;
	polygon.push_front(new_inter);
}

line find_max(int s)
{
	intersect temp;
	bool flag = false;
	while (!polygon.empty() && s>polygon.back().x) {
		temp = polygon.back();
		polygon.pop_back();
		flag = true;
	}
	if (flag) polygon.push_back(temp);
	return polygon.back().next_line;
}

int64_t* ingredient = (int64_t*) malloc(1000000*sizeof(*ingredient));
int64_t* DP = (int64_t*) malloc(1000000*sizeof(*DP));
int64_t* S = (int64_t*) malloc(1000000*sizeof(*S));

int main()
{
	int64_t N, a, b, c, j, A, B, i, maxj;
	line l;
	std::ios::sync_with_stdio(false);
	cin >> N >> a >> b >> c;
	for (i=0 ; i<N ; i++) cin >> ingredient[i];
	
	S[0] = ingredient[0];
	for (i=1 ; i<N ; i++) S[i] = S[i-1] + ingredient[i];
	
	DP[0] = a*S[0]*S[0]+b*S[0]+c;
	for (i=1 ; i<N ; i++) {
		j = i-1;
		A = -2*a*S[j];
		B = a*S[j]*S[j] - b*S[j] + DP[j];
		add_line({.A=A, .B=B});
		l = find_max(S[i]);
		maxj = l.A*S[i]+l.B;
		if (maxj<0) DP[i] = a*S[i]*S[i]+b*S[i]+c;
		else DP[i] = a*S[i]*S[i]+b*S[i]+c + maxj;
	}
	
	cout << DP[N-1] << endl;
	return 0;
}
