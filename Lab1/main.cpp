#include<iostream>
#include<vector>
#include<stack>
#include<utility>

using namespace std;

enum class Color
{black,white,grey};

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
            //cout<<(maze[i][j]=='1' ? ' ':maze[i][j]);
            switch(maze[i][j])
            {
                case '1':
                    cout<<' ';
                    break;
                case '2':
                    cout<<'0';
                    break;
                default:
                    cout<<maze[i][j];
                    break;
            }

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
    Node(pair<int,int> coord, Node* parent): coord(coord), parent(parent)
    {}
    Node(){}
    pair<int,int> coord;
    Node* parent=nullptr;
    Color c=Color::white;
};

void stackDFS(vector<string> &maze,int m,int n){

    cout<<"Stack DFS\n";
    int i=1;
    int j=0;
    stack<Node> s;
//    pair<int,int> p;
//    s.push({i,j});
    Node node;
    node.coord=make_pair(i,j);
    s.push(node);
    while(!s.empty()){

        Node node=s.top();
        s.pop();
        i = node.coord.first;
        j = node.coord.second;

        printMaze(maze,m,n);
        if(maze[i][j]=='*'){
            maze[i][j]='2';
            cout<<"wth";
            break;
            // return 1;
        }
        maze[i][j]='2';
        if( j>0 && (maze[i][j-1]==' ' || maze[i][j-1]=='*')){
            if(maze[i][j-1]!='*') maze[i][j-1]='1';
            s.push(Node({i,j-1},&node));

        }
        if( j<n-1 && (maze[i][j+1]==' ' || maze[i][j+1]=='*')){
            if(maze[i][j+1]!='*') maze[i][j+1]='1';
            s.push(Node({i,j+1},&node));
        }
        if( i>0 && (maze[i-1][j]==' ' || maze[i-1][j]=='*')){
            if(maze[i-1][j]!='*')maze[i-1][j]='1';
            s.push(Node({i-1,j},&node));
        }
        if( i<m-1 && (maze[i+1][j]==' ' || maze[i+1][j]=='*')){
            if(maze[i+1][j]!='*') maze[i+1][j]='1';
            s.push(Node({i+1,j},&node));
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
    printMaze(maze,m,n);
    return 0;
}
