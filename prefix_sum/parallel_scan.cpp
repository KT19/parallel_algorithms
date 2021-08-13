#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<ctime>
#include<chrono>
#include<string>
#include<omp.h>

using namespace std;

//prototype
int random_generate(int min_num = 0, int max_num = INT_MAX);
void calc_parallel_prefix_sum(vector<int>& P, int N, int thread_num);
void calc_prefix_sum(vector<int>& A, int N);
bool compare_result(vector<int>& A, vector<int>& P, int N);

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cout<<"Error in arguments"<<endl;
        cout<<"Please call such as ./a.out data_size thread num"<<endl;
        
        return 1;
    }

    int orig_N = stoi(argv[1]);
    int thread_num = stoi(argv[2]);
    int N = 1; //to be 2^x
    while(N < orig_N) 
        N <<= 1;
    
    vector<int> A(N, 0); //single 
    vector<int> P(2*N, 0); //parallel
    
    srand(time(NULL));

    for(int i = 0;i < orig_N;i++) {
        P[i] = A[i] = random_generate(-10,10);
    }
    
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    calc_parallel_prefix_sum(P, N, thread_num);
    end = chrono::system_clock::now();

    //check
    calc_prefix_sum(A, orig_N);
    if(!compare_result(A, P, orig_N)) {
        cout<<"Error!, Not match results"<<endl;

        return 1;
    }

    cout<<chrono::duration_cast<chrono::milliseconds> (end - start).count()<<endl;
    
    return 0;
}



int random_generate(int min_num, int max_num) {
    return (int)(min_num + rand() % (max_num - min_num + 1));
}

void calc_parallel_prefix_sum(vector<int>& P, int N, int thread_num) {
    //assign thread
    omp_set_num_threads(thread_num);
    
    //reduction step
    #pragma omp parallel
    {
        //function called in each thread (reduction step)
        auto reduction_scan = [](vector<int>&P, int N, int thread_id, int stride) {
            int index = (thread_id+1)*stride*2-1;
            if(index < 2*N)
                P[index] += P[index-stride];
        };
        //function called in each thread (post scan)
        auto post_scan = [](vector<int>&P, int N, int thread_id, int stride) {
            int index = (thread_id+1)*stride*2 - 1;
                if((index + stride) < 2*N) {
                    P[index + stride] += P[index];
            }
        };
    
        for(int stride = 1;stride <= N;stride *= 2) {
            int end = (N+1)/stride;

            #pragma omp for
            for(int i = 0;i < end; i++) {
                reduction_scan(P, N, i, stride);
            }
        }

        for(int stride = N / 2;stride > 0; stride /= 2){
            int end = (N+1) / stride;
       
            #pragma omp for
            for(int i = 0;i < end;i++) {
                post_scan(P, N, i, stride);
            }    
        }
    }

    return;
}

void calc_prefix_sum(vector<int>& A, int N) {
    for(int i = 1;i < N;i++) {
        A[i] = A[i-1] + A[i];
    }

    return;
}

bool compare_result(vector<int>& A, vector<int>& P, int N) {
    for(int i = 0;i < N;i++) {
        if(A[i] != P[i])
            return false;
    }

    return true;
}

