#include<iostream>
#include<vector>
#include<ctime>
#include<chrono>

using namespace std;

//prototype
int random_generate(int min_num = 0, int max_num = INT_MAX);
void calc_prefix_sum(vector<int>& A);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout<<"Error in arguments"<<endl;
        cout<<"Please call such as ./a.out data_size"<<endl;
        
        return 1;
    }

    int N = stoi(argv[1]);
    vector<int> A(N);
    
    srand(time(NULL));

    for(int i = 0;i < N;i++) {
        A[i] = random_generate(-10,10);
    }
    
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    calc_prefix_sum(A);
    end = chrono::system_clock::now();
    
    cout<<chrono::duration_cast<chrono::milliseconds> (end - start).count()<<endl;

    return 0;
}

int random_generate(int min_num, int max_num) {
    return (int)(min_num + rand() % (max_num - min_num + 1));
}

void calc_prefix_sum(vector<int>& A) {
    for(int i = 1;i < A.size();i++) {
        A[i] = A[i-1] + A[i];
    }

    return;
}
