using namespace std;
#include <iostream>
#include <stack>
#include <cstdint>
#include <cstdlib>

struct info {
	int64_t h;
	int64_t ind;
};

int64_t* A = (int64_t*) malloc(1000000*sizeof(*A));
int64_t* sum_left = (int64_t*) malloc(1000000*sizeof(*A));
int64_t* sum_right = (int64_t*) malloc(1000000*sizeof(*A));

int main()
{
	int64_t N, i, min_sum;
	stack <info> heights;
	
	std::ios::sync_with_stdio(false);
	cin >> N;
	for (i=0 ; i<N ; i++) cin >> A[i];
	
	// left parse
	sum_left[0] = A[0];
	heights.push({.h=A[0], .ind=0});
	for (i=1 ; i<N ; i++) {
		while (heights.top().h <= A[i]) {
			heights.pop();
			if (heights.empty()) break;
		}
		
		if (A[i] <= A[i-1]) sum_left[i] = sum_left[i-1] + A[i];
		else if (!heights.empty()) sum_left[i] = sum_left[heights.top().ind] + (i-heights.top().ind)*A[i];
		else sum_left[i] = (i+1)*A[i];
		
		heights.push({.h=A[i], .ind=i});
	}
	
	while (!heights.empty()) heights.pop();
	
	// right parse
	sum_right[N-1] = A[N-1];
	heights.push({.h=A[N-1], .ind=N-1});
	for (i=N-2 ; i>=0 ; i--) {
		while (heights.top().h <= A[i]) {
			heights.pop();
			if (heights.empty()) break;
		}
		
		if (A[i] <= A[i+1]) sum_right[i] = sum_right[i+1] + A[i];
		else if (!heights.empty()) sum_right[i] = sum_right[heights.top().ind] + (heights.top().ind-i)*A[i];
		else sum_right[i] = (N-i)*A[i];
		
		heights.push({.h=A[i], .ind=i});
	}
	
	// adding partial sums and finding min
	min_sum = sum_left[0];
	for (i=0 ; i<N ; i++) {
		sum_left[i] = sum_left[i] + sum_right[i] - A[i];
		if (i == 0) min_sum = sum_left[0];
		if (sum_left[i] < min_sum) min_sum = sum_left[i];
	}
	
	cout << min_sum << endl;
	return 0;
}
