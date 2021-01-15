#include<iostream>
#include<vector>
#include<stack>
#include<utility>
#include<queue>

using namespace std;

//Each cell in the grid is a node in the graph
struct Node {
    char value;
    pair<int, int> parent_coord = {-1, -1};
    bool visited = 0;
    int depth=1e9;
};

//Take text maze as input
vector<string> readMaze() {
    string line;
    vector<string> maze;
    while (getline(cin, line)) {
        maze.emplace_back(line);
    }
    return maze;
}

//Prints the Node maze
void printMaze(vector<vector<Node>> maze, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << maze[i][j].value;
        }
        cout<<endl;
    }
}

//Makes Nodes object grid from raw text grid
vector<vector<Node>> processGrid(vector<string> maze, int m, int n) {
    vector<vector<Node>> res(m);
    for (int i = 0; i < m; i++) {
        vector<Node> temp;
        for (int j = 0; j < n; j++) {
            Node node;
            node.value = maze[i][j];
            temp.emplace_back(node);
        }
        res[i] = temp;
    }
    return res;
}

//Performs DFS (iterative)
void stackDFS(vector<vector<Node>> &maze, int m, int n) {

    int no_of_states = 0;
    int shortest_path = 0;
    int i = 0;
    int j = 0;

    //Use stack for DFS. Push the coordinates of the Node in the stack
    stack<pair<int, int>> s;
    s.push({i, j});

    while (!s.empty()) {

        auto node = s.top();
        s.pop();
        no_of_states++;
        i = node.first;
        j = node.second;

        //Terminating case
        if (maze[i][j].value == '*') {

            pair<int, int> coord = {i, j};
            while (coord != make_pair(-1, -1)) {
                shortest_path++;
                maze[coord.first][coord.second].value = '0';
                coord = maze[coord.first][coord.second].parent_coord;
            }
            cout << no_of_states << '\n' << shortest_path << '\n';
            printMaze(maze, m, n);
            break;

        }

        maze[i][j].visited = 1;

        //We write the loops in the reverse ordered of the preferred direction (DURL) because we are using stack which is LIFO

        //left
        if (j > 0 && (maze[i][j - 1].value == ' ' || maze[i][j - 1].value == '*') && !maze[i][j - 1].visited) {
            s.push({i, j - 1});
            maze[i][j - 1].parent_coord = {i, j};
            maze[i][j - 1].visited = 1;

        }

        //right
        if (j < n - 1 && (maze[i][j + 1].value == ' ' || maze[i][j + 1].value == '*') && !maze[i][j + 1].visited) {

            s.push({i, j + 1});
            maze[i][j + 1].parent_coord = {i, j};
            maze[i][j + 1].visited = 1;
        }

        //up
        if (i > 0 && (maze[i - 1][j].value == ' ' || maze[i - 1][j].value == '*') && !maze[i - 1][j].visited) {

            s.push({i - 1, j});
            maze[i - 1][j].parent_coord = {i, j};
            maze[i - 1][j].visited = 1;
        }

        //down
        if (i < m - 1 && (maze[i + 1][j].value == ' ' || maze[i + 1][j].value == '*') && !maze[i + 1][j].visited) {

            s.push({i + 1, j});
            maze[i + 1][j].parent_coord = {i, j};
            maze[i + 1][j].visited = 1;
        }
    }

}

//Iterative BFS
void queueBFS(vector<vector<Node>> &maze, int m, int n) {

    int no_of_states = 0;
    int shortest_path = 0;
    int i = 0;
    int j = 0;

    //Use queue for BFS. Push the coordinates of the Node in the queue
    queue<pair<int, int>> s;
    s.push({i, j});
    while (!s.empty()) {

        auto node = s.front();
        s.pop();
        no_of_states++;
        i = node.first;
        j = node.second;

        //Terminating case
        if (maze[i][j].value == '*') {


            pair<int, int> coord = {i, j};
            while (coord != make_pair(-1, -1)) {
                shortest_path++;
                maze[coord.first][coord.second].value = '0';
                coord = maze[coord.first][coord.second].parent_coord;
            }
            cout << no_of_states << '\n' << shortest_path << '\n';
            printMaze(maze, m, n);
            break;

        }

        maze[i][j].visited = 1;

        //down
        if (i < m - 1 && (maze[i + 1][j].value == ' ' || maze[i + 1][j].value == '*') && !maze[i + 1][j].visited) {

            s.push({i + 1, j});
            maze[i + 1][j].parent_coord = {i, j};
            maze[i + 1][j].visited = 1;
        }

        //up
        if (i > 0 && (maze[i - 1][j].value == ' ' || maze[i - 1][j].value == '*') && !maze[i - 1][j].visited) {

            s.push({i - 1, j});
            maze[i - 1][j].parent_coord = {i, j};
            maze[i - 1][j].visited = 1;
        }

        //right
        if (j < n - 1 && (maze[i][j + 1].value == ' ' || maze[i][j + 1].value == '*') && !maze[i][j + 1].visited) {

            s.push({i, j + 1});
            maze[i][j + 1].parent_coord = {i, j};
            maze[i][j + 1].visited = 1;
        }

        //left
        if (j > 0 && (maze[i][j - 1].value == ' ' || maze[i][j - 1].value == '*') && !maze[i][j - 1].visited) {
            s.push({i, j - 1});
            maze[i][j - 1].parent_coord = {i, j};
            maze[i][j - 1].visited = 1;

        }

    }

}

bool DFID_pass(vector<vector<Node>> maze,int m,int n,int depth, int &no_of_states)
{
    int i = 0;
    int j = 0;
    stack<pair<int, int>> s;
    s.push({i, j});
    maze[i][j].depth=0;

    while (!s.empty()) {

        auto node = s.top();
        s.pop();
        no_of_states++;
        i = node.first;
        j = node.second;

        if (maze[i][j].value == '*') {


            pair<int, int> coord = {i, j};
            while (coord != make_pair(-1, -1)) {

                maze[coord.first][coord.second].value = '0';
                coord = maze[coord.first][coord.second].parent_coord;
            }
            cout << no_of_states << '\n' << depth+1 << '\n';
            printMaze(maze, m, n);

            return true;

        }

        //left
        if (j > 0 && (maze[i][j - 1].value == ' ' || maze[i][j - 1].value == '*') && maze[i][j].depth+1 < maze[i][j - 1].depth) {

            maze[i][j - 1].parent_coord = {i, j};
            maze[i][j - 1].visited = 1;
            maze[i][j-1].depth=maze[i][j].depth+1;

            //Verify depth of child <= maximum depth
            if(maze[i][j-1].depth <= depth) s.push({i, j - 1});

        }

        //right
        if (j < n - 1 && (maze[i][j + 1].value == ' ' || maze[i][j + 1].value == '*') && maze[i][j].depth+1 <maze[i][j + 1].depth) {


            maze[i][j + 1].parent_coord = {i, j};
            maze[i][j + 1].visited = 1;
            maze[i][j+1].depth=maze[i][j].depth+1;
            if(maze[i][j+1].depth <= depth) s.push({i, j + 1});
        }

        //up
        if (i > 0 && (maze[i - 1][j].value == ' ' || maze[i - 1][j].value == '*') && maze[i][j].depth+1 <maze[i-1][j].depth) {


            maze[i - 1][j].parent_coord = {i, j};
            maze[i - 1][j].visited = 1;
            maze[i-1][j].depth=maze[i][j].depth+1;
            if(maze[i-1][j].depth<=depth) s.push({i - 1, j});
        }


        //down
        if (i < m - 1 && (maze[i + 1][j].value == ' ' || maze[i + 1][j].value == '*') && maze[i][j].depth+1 < maze[i+1][j].depth) {


            maze[i + 1][j].parent_coord = {i, j};
            maze[i + 1][j].visited = 1;
            maze[i+1][j].depth=maze[i][j].depth+1;
            if(maze[i+1][j].depth<=depth) s.push({i + 1, j});
        }


    }
    return false;

}

void DFID(vector<vector<Node>> &maze, int m, int n)
{
    int depth=0;
    int no_of_states=0;
    while(!DFID_pass(maze,m,n,depth,no_of_states))
    {
        depth++;
    }
}

int main() {

    string algoCode;
    getline(cin,algoCode);
    vector<string> maze = readMaze();
    int m = maze.size();
    int n = maze[m - 1].size();
    vector<vector<Node>> proGrid = processGrid(maze, m, n);
    switch (algoCode[0]) {
        case '0':
            queueBFS(proGrid, m, n);
            break;
        case '1':
            stackDFS(proGrid, m, n);
            break;
        case '2':
            DFID(proGrid,m,n);
    }
    return 0;
}
