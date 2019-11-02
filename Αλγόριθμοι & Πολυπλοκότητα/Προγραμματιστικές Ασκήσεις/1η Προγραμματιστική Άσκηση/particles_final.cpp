using namespace std;
#include <iostream>
#include <stack>
#include <cstdint>
#include <cstdlib>

struct particle {
	int64_t tau;
	int64_t u;
};

struct intersect {
	long double time;
	int64_t next_particle;
};

// L=0 when same kind of particles
long double col_time (particle a, particle b, int64_t L) {
	if (a.u == 0 || b.u == 0) return 0; // particles that do not exist
	return (double)(L + a.u*a.tau - b.u*b.tau)/(double)(a.u-b.u);
}

particle* A = (particle*) malloc(200000*sizeof(*A));
particle* B = (particle*) malloc(200000*sizeof(*A));

int main()
{
	int64_t N, L, K, i, j, var;
	long double check_time;
	bool flag;
	
	std::ios::sync_with_stdio(false); // faster cin
	cin >> N >> L >> K;
	for (i=0 ; i<N ; i++) cin >> A[i].tau >> A[i].u;
	for (i=0 ; i<N ; i++) {
		cin >> B[i].tau >> B[i].u;
		B[i].u = -B[i].u; // coming from opposite direction
	}
	
	stack <intersect> Afronts;
	stack <intersect> Bfronts;
	particle front;
	intersect inter;
	int count = 0;
	
	while (count < K) {
		// finding the line that first becomes front
		for (j=0 ; j<N ; j++) {
			if (A[j].u > 0) {
				Afronts.push({.time=(double)A[j].tau, .next_particle=j+1});
				break;
			}
		}
		front = A[j];
		// lines with bigger slope become front after their intersection with previous front
		for (i=j+1 ; i<N ; i++) {
			if (A[i].u > front.u) {
				inter.next_particle = i+1;
				inter.time = col_time(front,A[i],0);
				while (inter.time <= Afronts.top().time) {
					Afronts.pop();
					inter.time = col_time(A[Afronts.top().next_particle - 1], A[i], 0);	
				}
				Afronts.push(inter);
				front = A[i];
			}
		}
		
		// same as A (opposite direction - lower slope)
		for (j=0 ; j<N ; j++) {
			if (B[j].u < 0) {
				Bfronts.push({.time=(double)B[j].tau, .next_particle=j+1});
				break;
			}
		}
		front = B[j];
		for (i=j+1 ; i<N ; i++) {
			if (B[i].u < front.u) {
				inter.next_particle = i+1;
				inter.time = col_time(front,B[i],0);
				while (inter.time <= Bfronts.top().time) {
					Bfronts.pop();
					inter.time = col_time(B[Bfronts.top().next_particle - 1], B[i], 0);
				}
				Bfronts.push(inter);
				front = B[i];
			}
		}
		
		flag = false; // true when we find the first collision
		while (!flag) {
			// checking when the front lines intersect
			check_time = col_time(A[Afronts.top().next_particle - 1], B[Bfronts.top().next_particle - 1], L);
			if (check_time < Afronts.top().time) { 	// A line is not front yet
				Afronts.pop(); 						// so we check the previous front
				if (Afronts.empty()) {
					B[Bfronts.top().next_particle - 1].u = 0;	// no collision for this
					break;	
				}
			}
			else if (check_time < Bfronts.top().time) { // same as A
				Bfronts.pop();
				if (Bfronts.empty()) {
					A[Afronts.top().next_particle - 1].u = 0;
					break;
				}
			}
			else {
				// we have collision!
				count++;
				cout << Afronts.top().next_particle << " " << Bfronts.top().next_particle << endl;
				// particles are being destroyed
				A[Afronts.top().next_particle - 1].u = 0;
				B[Bfronts.top().next_particle - 1].u = 0;
				flag = true;
			}
		}
		while (!Afronts.empty()) Afronts.pop();
		while (!Bfronts.empty()) Bfronts.pop();
	}	
	return 0;
}
