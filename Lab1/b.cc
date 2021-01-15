#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<bitset>
#include<utility>
#include<stack>
using namespace std;

/*
 * n        -> No. of rows in grid
 * m        -> No. of colums in grid
 * grid     -> stores the entire grid
 * visited  -> stores if cell is already visited or not 
 * parent   -> saves parent of current cell
 * */

int n, m, no_of_states_explored = 0, length_of_path = 0;
const int N = 1e3+10;
vector<string>grid;
vector<vector<int>> visited;
vector<vector<pair<int, int>>> parent;

//const int dx[] = {0, 1, -1, 0}, dy[] = {1, 0, 0, -1}; //right first
//const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1}; //down first
//const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0}; //down first



//TODO:
//  clean code
//  nice names

// priority : DOWN > UP > RIGHT > LEFT
const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};

/*
 * below functions returns if cell (i, j) is valid or not 
 * i.e. it should lie in grid, not borders (+, |, -) and should not be visited 
 * */
bool validState(int i, int j) {
	return i>=0 && i<n && j>=0 && j<m && visited[i][j] == 0 && grid[i][j] != '|' && grid[i][j] != '+' && grid[i][j] != '-';
}


/*
 * return if cell (i, j) is goal (*)
 */
bool GoalTest(int i, int j) {
    return grid[i][j] == '*';
}

/*
 * return the valid neibhours of cell (i, j) according to priority order
 */
vector<pair<int, int>> MoveGen(int i, int j) {
    vector<pair<int, int>> res;
    for(int k=0;k<4;++k) {
        int x = i + dx[k], y = j + dy[k];
        if(validState(x, y))
            res.push_back({x, y});
    }
    return res;
}

/*
 * makes the shortest path in maze with '0' characters 
 */
void markPath(pair<int, int> end) {
    length_of_path = 1;
    while(end.first != 0 || end.second != 0) {
        ++ length_of_path;
        grid[end.first][end.second] = '0';
        end = parent[end.first][end.second];
    }
    grid[end.first][end.second] = '0';
    return;
}

/* 
 * prints answer
 */
void printAns() {
    cout << no_of_states_explored << "\n" << length_of_path << "\n";
    for(string& s : grid)
        cout << s << "\n";
    return;
}

/*
 * BFS usign queue
 */
bool BFS(pair<int, int> start = {0, 0}) {
    queue<pair<int, int>> st;
    st.push(start);
    visited[start.first][start.second] = 1;
    while(st.size()) {
        pair<int, int> t = st.front();
        st.pop();
        no_of_states_explored ++;
        if(GoalTest(t.first, t.second)) {
            markPath(t);
            printAns();
            return 1;
        }
        for(pair<int, int> node : MoveGen(t.first, t.second))
            st.push(node), visited[node.first][node.second]=1, parent[node.first][node.second] = t;
    }
    return 0;
}


/*
 * DFS using stack
 */
bool DFS(pair<int, int> start = {0, 0}) {
    stack<pair<int, int>> st;
    st.push(start);
    visited[start.first][start.second] = 1;
    while(st.size()) {
        pair<int, int> t = st.top();
        st.pop();
        ++ no_of_states_explored;

        // check if goal is reached
        if(GoalTest(t.first, t.second)) {
            markPath(t);
            printAns();
            return 1;
        }

        vector<pair<int, int>> sideNodes = MoveGen(t.first, t.second); 
        reverse(sideNodes.begin(), sideNodes.end());

        for(pair<int, int> node : sideNodes) 
            st.push(node), visited[node.first][node.second] = 1, parent[node.first][node.second] = t;
    }
    return 0;
}

/*
 * Dep is 2D vector which stores the depth of each cell (i, j) in Dep[i][j]
 */
vector<vector<int>> Dep;
int maxDepth = -1;

/*
 * DFS using recursion based on depth
 * if reach at a node with more depth than max allowed depth, we don't proceed on that path
 */
bool DFID_DSF(pair<int, int> start, int depth) {

    if(depth > maxDepth)
        return 0;
    
    Dep[start.first][start.second] = depth;

    if(GoalTest(start.first, start.second)) {
        markPath(start);
        printAns();
        return 1;
    }
    no_of_states_explored ++;

    for(pair<int, int> node : MoveGen(start.first, start.second)) {
        if(depth + 1 < Dep[node.first][node.second]) {
            parent[node.first][node.second] = start;
            if(DFID_DSF(node, depth + 1))
                return 1;
        }
    }
   
    return 0;
}

/*
 * DFID implementation
 * iterate over depths from 0 to n*m / INF and if we find our -
 * target(*), we print path as its guarantees the shortest path
 */
bool DFID() {
    for(maxDepth=0; maxDepth<=n*m; ++maxDepth) {
        Dep.assign(n, vector<int>(m, 1e9));
        if(DFID_DSF({0, 0}, 0)) {
            return 1;
        }
    }
    return 0;
}



int main() {
    int T;
	string s;
    getline(cin, s);
    T = stoi(s);

    // inputing grid
    while(getline(cin, s)) {
        if((int)s.size() == 0)
            break;
        else 
            grid.push_back(s);
    }
    
    // setting rows and columns of grid
    n = (int)grid.size();
    m = (int)grid[0].size();


    //allocating memory to containers
    visited.assign(n, vector<int>(m, 0));
    parent.assign(n, vector<pair<int, int>>(m, {-1, -1}));


    /*
        cerr << T << " " << n << " " << m << "\n";
        for(string& i: grid)
            cerr << i << "\n";

        return 0;
    */

    bool found = false;
    if(T == 0) {
        found = BFS();
    } else if(T == 1) {
        found = DFS();
    } else if(T == 2) {
        found = DFID();
    }
    
    // if given test case doesn't have a target (*) just print original grid 
    if(!found) {
        printAns();
    }
	return 0;
}

