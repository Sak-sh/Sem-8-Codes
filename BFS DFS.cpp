#include <iostream>
#include <omp.h>
#include <chrono>
#include<map>
#include<vector>
#include<queue>
using namespace std;
using namespace std::chrono;


void BFS(map<int,vector<int>> m,int start,int n){
    vector<int> visited(n+1,false);
    queue<int> q;

    q.push(start);
    visited[start]=true;

    while (!q.empty())
    {
        int node=q.front();
        q.pop();

        cout<<node<<" ";

        for(auto nbr:m[node])
        {
            if(!visited[nbr]){
                q.push(nbr);
                visited[nbr]=true;
            }
        }
        
    }
    
}


void BFS_parallel(map<int,vector<int>> m,int start,int n){
    vector<int> visited(n+1,false);
    queue<int> q;

    q.push(start);
    visited[start]=true;

    while (!q.empty())
    {
        int node=q.front();
        q.pop();

        cout<<node<<" ";

        #pragma omp parallel for
        for (int i=0;i<m[node].size();i++)
        {
            int nbr=m[node][i];

            if(!visited[nbr]){
                #pragma omp critical
                {
                    if(!visited[nbr]){
                    q.push(nbr);
                    visited[nbr]=true;
                }
                }
            }
            
            
        }
        
    }
    
}

void DFS(map<int,vector<int>> m,int node,vector<bool> &visited){

    cout<<node<<" ";
    visited[node]=true;
    for(auto nbr:m[node]){
        if(!visited[nbr]){
            DFS(m,nbr,visited);
        }
    }
}

void DFS_parallel(map<int,vector<int>> m,int node,vector<bool> &visited){

    
    #pragma omp critical
    {
        if(visited[node]){
            return;
        }

        visited[node]=true;
        cout<<node<<" ";

    }
    
    #pragma omp parallel for
    for(int i=0;i<m[node].size();i++){
        int nbr=m[node][i];
        if(!visited[nbr]){
            DFS_parallel(m,nbr,visited);
        }
    }
}

int main(){

    int n,e,start;

    cout<<"Enter no of Nodes : ";
    cin>>n;
    map<int,vector<int>> m;
    cout<<"Enter no of Edges : ";
    cin>>e;

    cout<<"Enter source and destination : ";
    for (int i = 0; i < e; i++)
    {
        int u,v;
        cin>>u>>v;
        m[u].push_back(v);
        m[v].push_back(u);

    }
    cout<<"Enter start Node : ";
    cin>>start;


    cout << "\nSequential BFS:\n";
    auto t1=high_resolution_clock::now();
    BFS(m,start,n);
    auto t2=high_resolution_clock::now();
    cout<<"serial time BFS : "<<duration_cast<microseconds>(t2-t1).count()<<" Microsecond"<<endl;


    cout << "\n parallel BFS:\n";
    t1=high_resolution_clock::now();
    BFS_parallel(m,start,n);
    t2=high_resolution_clock::now();
    cout<<"parallel time BFS : "<<duration_cast<microseconds>(t2-t1).count()<<" Microseconds"<<endl;


    cout<<"\nDFS : \n";
    vector<bool> visited1(n+1,false);
    t1=high_resolution_clock::now();
    DFS(m,start,visited1);
    t2=high_resolution_clock::now();
    cout<<"serial time DFS : "<<duration_cast<microseconds>(t2-t1).count()<<" Microsecond"<<endl;

    
    cout<<"\nDFS parallel : \n";
    vector<bool> visited2(n+1,false);
    t1=high_resolution_clock::now();
    DFS_parallel(m,start,visited2);
    t2=high_resolution_clock::now();
    cout<<"parallel time DFS : "<<duration_cast<microseconds>(t2-t1).count()<<" Microsecond"<<endl;

    

    return 0;
}