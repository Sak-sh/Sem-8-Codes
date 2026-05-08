#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std::chrono;

void bubble_sort(vector<int> &arr)
{

    int n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
void bubble_sort_parallel(vector<int> &arr)
{

    int n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {

        if(i%2==0){
            #pragma omp parallel for
            for (int j = 0; j < n-1; j+=2)
            {
                 if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }else{
            #pragma omp parallel for
            for (int j = 1; j < n-1; j+=2)
            {
                 if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}


void merge(vector<int> &arr,int left,int mid,int right){
    vector<int> temp(right-left+1);

    int i=left,j=mid+1,k=0;

    while(i<=mid && j<=right){
        if(arr[i]<arr[j]){
            temp[k++]=arr[i++];
        }else{
            temp[k++]=arr[j++];
        }
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    k=0;
    for (int i = left; i <= right; i++)
    {
        arr[i]=temp[k++];
    }
}


void merge_sort(vector<int> &arr,int left,int right){

    if(left>=right){
        return;
    }

    int mid=(left + right)/2;
    merge_sort(arr,left,mid);
    merge_sort(arr,mid+1,right);
    merge(arr,left,mid,right);
}


void merge_sort_parallel(vector<int> &arr,int left,int right,int depth=0){

    if(left>=right){
        return;
    }

    int mid=(left+right)/2;
    if(depth<3){
        #pragma omp task shared(arr)
        merge_sort_parallel(arr,left,mid,depth+1);

        #pragma omp task shared(arr)
        merge_sort_parallel(arr,mid+1,right,depth+1);

        #pragma omp taskwait
        

    }else{

        merge_sort(arr,left,mid);
        merge_sort(arr,mid+1,right);

    }

    merge(arr,left,mid,right);
    
}


int main()
{

    int n;

    cout << "Enter no of Elements : ";
    cin >> n;

    vector<int> arr(n), arr_copy;

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    arr_copy = arr;
    // ---------- Bubble Sort ----------
    auto start = high_resolution_clock::now();
    bubble_sort(arr);
    auto end = high_resolution_clock::now();
    cout << "\nSequential Bubble Sort Time: "
         << duration_cast<microseconds>(end - start).count()
         << " microseconds\n";

    arr = arr_copy;
    start = high_resolution_clock::now();
    bubble_sort_parallel(arr);
    end = high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: "
         << duration_cast<microseconds>(end - start).count()
         << " microseconds\n";

    // ---------- Merge Sort ----------
    arr = arr_copy;
    start = high_resolution_clock::now();
    merge_sort(arr, 0, n - 1);
    end = high_resolution_clock::now();
    cout << "\nSequential Merge Sort Time: "
         << duration_cast<microseconds>(end - start).count()
         << " microseconds\n";

    arr = arr_copy;
    start = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort_parallel(arr, 0, n - 1);
    }
    end = high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: "
         << duration_cast<microseconds>(end - start).count()
         << " microseconds\n";


    return 0;
}