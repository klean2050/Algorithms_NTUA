using namespace std;
#include <iostream>
#include <cstdint>
#include <cstdlib>

int64_t* A = (int64_t*) malloc(1000000*sizeof(*A));
int64_t* B = (int64_t*) malloc(1000000*sizeof(*A));
int64_t* LIS = (int64_t*) malloc(1000000*sizeof(*A));

int binary_gr(int64_t arr[],int key,int start,int end)
{
	int mid = (end+start)/2;
	if (start==end || arr[mid]==key) return mid;
	else if (arr[mid]>key) return binary_gr(arr,key,start,mid);
	else return binary_gr(arr,key,mid+1,end);
}

int binary_sm(int64_t arr[],int key,int start,int end)
{
	int mid = (end+start)/2;
	if (start==end || arr[mid]==key) return mid;
	else if (arr[mid]<key) return binary_sm(arr,key,start,mid);
	else return binary_sm(arr,key,mid+1,end);
}

int main()
{
	std::ios::sync_with_stdio(false); // faster cin
	int N, i, count=0, max=0, smallest_greater, greatest_smaller;
	cin >> N;
	for (i=0 ; i<N ; i++) cin >> A[i];
	
	// find size of longest increasing subsequences
	LIS[count] = A[0];
	B[count] = 1;
	for (i=1 ; i<N ; i++) {
		if (A[i]>LIS[count]) {
			count++;
			LIS[count] = A[i];
		} else {
			smallest_greater = binary_gr(LIS,A[i],0,count);
			LIS[smallest_greater] = A[i];
		}
		B[i] = count+1;
	}
	
	// find size of longest decreasing subsequences
	count = 0;
	LIS[count] = A[N-1];
	B[N-1] = B[N-2]+1;
	for (i=N-2 ; i>0 ; i--) {
		if (A[i]<LIS[count]) {
			count++;
			LIS[count] = A[i];
		} else {
			greatest_smaller = binary_sm(LIS,A[i],0,count);
			LIS[greatest_smaller] = A[i];
		}
		B[i] = B[i-1]+count+1;
	}
	
	// find cut that maximises combined result
	for (i=0 ; i<N ; i++) if (B[i]>max) max = B[i];
	cout << max << endl;
	return 0;
}
