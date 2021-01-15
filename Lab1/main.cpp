#include<iostream>
#include<vector>
#include<stack>
#include<utility>

using namespace std;

vector<string> readMaze(){
    string line;
    vector<string> maze;
    while(getline(cin,line)){
        maze.emplace_back(line);
    }
    // int m = maze.size();
    // int n = maze[m-1].size();
    return maze;
}

void printMaze(vector<string> maze,int m,int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cout<<maze[i][j];
        }
        cout<<endl;
    }
}

void DFS(vector<string> &maze, int m, int n, int i, int j,string dir="default"){
    // int ret = 0;
    cout<<dir<<endl;
    printMaze(maze,m,n);
    if(maze[i][j]=='*'){
        maze[i][j]='0';
        cout<<"wth";
        // return 1;
    }
    if( i<n-1 && (maze[i+1][j]==' ' || maze[i+1][j]=='*')){
        maze[i+1][j] = '0';
        // ret =
        DFS(maze,m,n,i+1,j,"down");
        // if(ret==1){
        //     cout<<"NOOOO";
        //     return 1;
        // }
    }
    if( i>0 && (maze[i-1][j]==' ' || maze[i-1][j]=='*')){
        maze[i-1][j] = '0';
        // ret =
        DFS(maze,m,n,i-1,j,"up");
        // if(ret==1){
        //     cout<<"NOOOO";
        //     return 1;
        // }
    }
    if( j<m-1 && (maze[i][j+1]==' ' || maze[i][j+1]=='*')){
        maze[i][j+1] = '0';
        // ret =
        DFS(maze,m,n,i,j+1,"right");
        // if(ret==1){
        //     cout<<"NOOOO";
        //     return 1;
        // }
    }
    if( j>0 && (maze[i][j-1]==' ' || maze[i][j-1]=='*')){
        maze[i][j-1] = '0';
        // ret =
        DFS(maze,m,n,i,j-1,"left");
        // if(ret==1){
        //     cout<<"NOOOO";
        //     return 1;
        // }
    }
    // return ret;
}

void performDFS(vector<string> maze,int m,int n){
    cout<<"DFS";
    int i=1;
    int j=0;
    maze[i][j] = '0';
    DFS(maze,m,n,i,j);
    cout<<"completed"<<endl;
    printMaze(maze,m,n);
}

struct Node{
    pair<int,int> coord;
    Node* parent=nullptr;
};

void stackDFS(vector<string> maze,int m,int n){

    cout<<"Stack DFS\n";
    int i=1;
    int j=0;
    stack<pair<int,int>> s;
    pair<int,int> p;
    s.push({i,j});
    while(!s.empty()){
        p=s.top();
        s.pop();
        i = p.first;
        j = p.second;
        maze[i][j]='0';
        printMaze(maze,m,n);
        if(maze[i][j]=='*'){
            maze[i][j]='0';
            cout<<"wth";
            break;
            // return 1;
        }
        if( j>0 && (maze[i][j-1]==' ' || maze[i][j-1]=='*')){
            s.push({i,j-1});
        }
        if( j<n-1 && (maze[i][j+1]==' ' || maze[i][j+1]=='*')){
            s.push({i,j+1});
        }
        if( i>0 && (maze[i-1][j]==' ' || maze[i-1][j]=='*')){
            s.push({i-1,j});
        }
        if( i<m-1 && (maze[i+1][j]==' ' || maze[i+1][j]=='*')){
            s.push({i+1,j});
        }
    }
    cout<<"Complete!";
}

int main(){
    cout<<"Hello World\n";
    vector<string> maze = readMaze();
    int m = maze.size();
    int n = maze[m-1].size();
    printMaze(maze,m,n);
    // maze[3][3] = '0';
    // printMaze(maze,m,n);
    // performDFS(maze,m,n);
    stackDFS(maze,m,n);
    return 0;
}
