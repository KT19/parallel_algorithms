#include<iostream>
#include<vector>
#include<ctime>
#include<ctime>
#include<chrono>
#include<thread>
#include"Thread_Pool.h"

using namespace std;

//prototype
int random_generate(int min_num = 0, int max_num = INT_MAX);
void parallel_bubble_sort(vector<int>& A, int thread_num);
void sort_per_thread(vector<int> &A, int left, int right);
void myswap(int &x, int &y);
bool check(vector<int>& A);

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cout<<"Error in arguments"<<endl;
        cout<<"Please call such as ./a.out data_size thread_num"<<endl;

        return 0;
    }

    int N = stoi(argv[1]);
    int thread_num = stoi(argv[2]);
    vector<int> A(N);

    srand(time(NULL));

    for(int i = 0;i < N;i++) {
        A[i] = random_generate();
    }

    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    parallel_bubble_sort(A, thread_num);
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

void parallel_bubble_sort(vector<int>& A, int thread_num) {
    int N = A.size();
    int step_size = N / thread_num;
    ThreadPool pth(thread_num);
    vector<int> even_s; //start index in each thread
    vector<int> even_e; //end index in each thread
    vector<int> odd_s; //start index in each thread
    vector<int> odd_e; //end index in each thread

    //create index for multi thread
    for(int i = 0;i < thread_num;i++) {
        if(i == 0) {
            even_s.push_back(0);
            even_e.push_back(min(even_s.back()+step_size, N-1));
            odd_s.push_back(1);
            odd_e.push_back(min(odd_s.back()+step_size, N-1));
        } else if(i == thread_num-1){
            even_s.push_back(even_e.back());
            even_e.push_back(N-1);
            odd_s.push_back(odd_e.back());
            odd_e.push_back(N-1);
        } else {
            even_s.push_back(even_e.back());
            even_e.push_back(even_s.back()+step_size);
            odd_s.push_back(odd_e.back());
            odd_e.push_back(odd_s.back()+step_size);
        }
    }

    for(int k = 0;k < N;k++) { //outer loop
        if(k % 2 == 0) { //even
            for(int i = 0;i < thread_num;i++) {
                pth.enqueue(sort_per_thread, ref(A), even_s[i], even_e[i]);
            }
        } else { //odd
            for(int i = 0;i < thread_num;i++) {
                pth.enqueue(sort_per_thread, ref(A), odd_s[i], odd_e[i]);
            }
        }

        pth.waitAll();
    }
    
    return;
}

void sort_per_thread(vector<int> &A, int left, int right) {
    for(int i = left;i < right;i+=2) {
        if(A[i] > A[i+1]) {
            myswap(A[i], A[i+1]);
        }
    }
}

void myswap(int &x, int &y) {
    int c = x;
    x = y;
    y = c;
    return;
}

bool check(vector<int>& A) {
    for(int i = 1;i < A.size();i++) {
        if(A[i-1] > A[i])
            return false;
    }

    return true;
}
