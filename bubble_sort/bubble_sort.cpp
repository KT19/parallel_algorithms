#include<iostream>
#include<vector>
#include<ctime>
#include<chrono>

using namespace std;

//prototype
int random_generate(int min_num = 0, int max_num = INT_MAX);
void bubble_sort(vector<int>& A);
void myswap(int &x, int &y);
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
    bubble_sort(A);
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

void bubble_sort(vector<int>& A) {
    int N = A.size();
    for(int i = N-1;i >= 0;i--) {
        for(int j = 0;j < i;j++) {
            if(A[j] > A[j+1])
                myswap(A[j],A[j+1]);
        }
    }
    return;
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
