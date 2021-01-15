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

void printDFSMaze(vector<string> maze,int m,int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            switch(maze[i][j])
            {
                case '1':
                    cout<<' ';
                    break;
                case '2':
                    cout<<'2';
                    break;
                default:
                    cout<<maze[i][j];
                    break;
            }

        }
        cout<<endl;
    }
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
            maze[i][j]='0';
            Node *pnode = node.parent;
            while(pnode!=nullptr){
                i = pnode->coord.first;
                j = pnode->coord.second;
                maze[i][j] = '0';
                cout<<i<<j<<endl;
                pnode = pnode->parent;
            }
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
    cout<<"Complete!"<<endl;
}

int main(){
    cout<<"Hello World\n";
    vector<string> maze = readMaze();
    int m = maze.size();
    int n = maze[m-1].size();
    printMaze(maze,m,n);
    stackDFS(maze,m,n);
    printDFSMaze(maze,m,n);
    return 0;
}
