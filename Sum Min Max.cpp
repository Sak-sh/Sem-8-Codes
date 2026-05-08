#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

void sum(vector<int> arr){
    long long sum=0;
    int n=arr.size();

    for (int i = 0; i < n; i++)
    {
        sum+=arr[i];
    }
    
    cout << "Sum : " << sum << endl;
    
}

void sum_prallel(vector<int> arr){
    long long sum=0;
    int n=arr.size();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
    {
        sum+=arr[i];
    }
    
    cout << "Sum : " << sum << endl;
    
}

void avg(vector<int> arr){
    long long sum=0;
    int n=arr.size();

    for (int i = 0; i < n; i++)
    {
        sum+=arr[i];
    }
    
    cout << "Avg : " << (double)sum/n << endl;
    
}

void avg_prallel(vector<int> arr){
    long long sum=0;
    int n=arr.size();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
    {
        sum+=arr[i];
    }
    
    cout << "Avg : " << (double)sum/n << endl;
    
}


void min(vector<int> arr){
    int min_val=arr[0];
    int n=arr.size();

    for (int i = 0; i < n; i++)
    {
        if (arr[i]<min_val)
        {
            min_val=arr[i];
        }
        
    }
    cout << "Minimum : " << min_val << endl;

}
void min_parallel(vector<int> arr){
    int min_val=arr[0];
    int n=arr.size();

    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < n; i++)
    {
        if (arr[i]<min_val)
        {
            min_val=arr[i];
        }
        
    }
    cout << "Minimum : " << min_val << endl;

}
void max(vector<int> arr){
    int max_val=arr[0];
    int n=arr.size();

    for (int i = 0; i < n; i++)
    {
        if (arr[i]>max_val)
        {
            max_val=arr[i];
        }
        
    }
    cout << "Maximum : " << max_val << endl;

}
void max_parallel(vector<int> arr){
    int max_val=arr[0];
    int n=arr.size();

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < n; i++)
    {
        if (arr[i]>max_val)
        {
            max_val=arr[i];
        }
        
    }
    cout << "Maximum : " << max_val << endl;

}



int main(){

    int n=100000;
    vector<int> arr(n);

    for (int i = 0; i < n; i++)
    {
        arr[i]=i;
    }
    
    cout << "Serial Execution:\n";
    auto start_serial = high_resolution_clock::now();

    sum(arr);
    avg(arr);
    min(arr);
    max(arr);

    auto end_serial = high_resolution_clock::now();

    cout << "Time Required: "
         << duration_cast<microseconds>(end_serial - start_serial).count()
         << " microseconds\n\n";

    cout << "Parallel Execution:\n";
    auto start_parallel = high_resolution_clock::now();

    sum_prallel(arr);
    avg_prallel(arr);
    min_parallel(arr);
    max_parallel(arr);

    auto end_parallel = high_resolution_clock::now();

    cout << "Time Required: "
         << duration_cast<microseconds>(end_parallel - start_parallel).count()
         << " microseconds\n";

    

    return 0;
}