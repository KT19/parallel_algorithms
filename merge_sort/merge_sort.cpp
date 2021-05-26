#include<iostream>
#include<vector>
#include<ctime>
#include<ctime>
#include<chrono>

using namespace std;

//prototype
int random_generate(int min_num = 0, int max_num = INT_MAX);
void merge_sort(vector<int>& A, int left, int right);
void merge(vector<int>& A, int left, int mid, int right);
bool check(vector<int>& A);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout<<"Error in arguments"<<endl;
        cout<<"Please call such as ./a.out data_size"<<endl;

        return 0;
    }

    int N = stoi(argv[1]);
    vector<int> A(N);

    srand(time(NULL));

    for(int i = 0;i < N;i++) {
        A[i] = random_generate();
    }

    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    merge_sort(A, 0, N-1);
    end = chrono::system_clock::now();

    if(!check(A)) {
        cout<<"Not sorted"<<endl;
        return -1;
    }

    cout<<chrono::duration_cast<chrono::milliseconds> (end - start).count()<<endl;



    return 0;
}

int random_generate(int min_num, int max_num) {
    return (int)(min_num + rand() % (max_num-min_num+1));
}

void merge_sort(vector<int>& A,int left, int right) {
    if(right > left) {
        int mid = left + (right - left) / 2;
        merge_sort(A, left, mid);
        merge_sort(A, mid+1, right);
        merge(A, left, mid, right);
    }


    return;
}

void merge(vector<int>& A, int left, int mid, int right) {
    vector<int> C(right-left+1, 0);
    int N = mid-left + 1;
    int M = right-mid;
    vector<int> L;
    vector<int> R;
    for(int i = 0;i < N;i++) {
        L.push_back(A[left+i]);
    }
    for(int i =0;i < M;i++) {
        R.push_back(A[mid+1+i]);
    }

    int i = 0;
    int j = 0;
    int k = left;

    while(i < N && j < M) {
        if(L[i] <= R[j])
            A[k++] = L[i++];
        else
            A[k++] = R[j++];
    }

    while(i < N)
        A[k++] = L[i++];
    while(j < M)
        A[k++] = R[j++];

    return;
}

bool check(vector<int>& A) {
    for(int i = 1;i < A.size();i++) {
        if(A[i-1] > A[i])
            return false;
    }

    return true;
}
