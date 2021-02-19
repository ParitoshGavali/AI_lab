#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include <array>
#include <iterator>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <numeric>
#include <random>
#include <iterator>
#include <chrono>

using namespace std;

typedef long long int ll;
typedef short int si;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef map<int, int> mii;
typedef unordered_map<int, int> umii;

#define rep(i, n) for(int (i)=0;(i)<(n);(i)++)
#define repd(i, n) for(int (i)=(n)-1;(i)>=0;(i)--)
#define fastio ios::ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
#define eb emplace_back
#define mp make_pair
#define MOD 1000000007
#define MAX 1e9
#define MIN -1e9
#define PI 3.1415926535897932384626

template<class T>
void printVector(vector<T> &v)
{
    for(auto x:v) cout<<x<<' ';
    cout<<'\n';
}

enum class Color
{
    WHITE,GREY,BLACK
};

struct Node
{
        double x,y;
        Color color=Color::WHITE;
        Node(double x,double y):x(x),y(y) {}
        pii getCoordinates()
        {
            return mp(x,y);
        }
        Color getColor()
        {
            return color;
        }
        void setColor(Color color)
        {
            this->color=color;
        }
        bool isVisited()
        {
            return color!=Color::WHITE;
        }
        bool isProcessed()
        {
            return color==Color::BLACK;
        }
};

struct graphNode: public Node
{
    int index=-1;
    graphNode(double x,double y, int index):Node(x,y),index(index) {}
    int getIndex()
    {
        return index;
    }
};

struct Graph
{
    vector<graphNode> nodes;
    int numNodes=0;
    vector<vector<double>> distanceMatrix;

    vector<multimap<double,int>> nearestNeighbour;

    Graph(vector<graphNode> &nodes, vector<vector<double>> &distanceMatrix)
    : nodes(nodes), distanceMatrix(distanceMatrix), numNodes(nodes.size())
    {
        rep(i,numNodes)
        {
            multimap<double,int> neighbours;
            rep(j,numNodes)
            {
                neighbours.insert(mp(distanceMatrix[i][j],j));

            }
            nearestNeighbour.eb(neighbours);
        }
    }

    graphNode getNode(int index)
    {
        return nodes[index];
    }

    void changeNodeColor(int index,Color c)
    {
        nodes[index].setColor(c);
    }

    void makeAllNodesUnvisited()
    {
        rep(i,numNodes)
        {
            nodes[i].setColor(Color::WHITE);
        }
    }

    bool isVisited(int index)
    {
        return nodes[index].isVisited();
    }

    bool isProcessed(int index)
    {
        return nodes[index].isProcessed();
    }

    multimap<double,int> getNeighbours(int index)
    {
        return nearestNeighbour[index];
    }

    multimap<double,int> getUnvisitedNeighbours(int index)
    {
        multimap<double,int> unvisited;
        for(auto p : nearestNeighbour[index])
        {
            if(!isVisited(p.second))
            {
                unvisited.insert(p);
            }
        }
        return unvisited;
    }

    int getNearestUnvisitedNeighbour(int index)
    {

        for(auto p : nearestNeighbour[index])
        {
            if(!isVisited(p.second))
            {
                return p.second;
            }
        }

    }





    double getDistance(int index1, int index2)
    {
        return distanceMatrix[index1][index2];
    }

    double getMaxPossibleTourLength()
    {
        double maximum=0;
        rep(i,numNodes)
        {
            rep(j,i)
            {
                if(distanceMatrix[i][j]>maximum) maximum=distanceMatrix[i][j];
            }
        }
        return maximum*numNodes;
    }

    double getHeuristicCost(vector<int> &path)
    {
        double cost=0;
        rep(i,path.size()-1)
        {
            cost+=getDistance(path[i],path[i+1]);
        }
        cost+=getDistance(path[path.size()-1],path[0]);
        return cost;
    }

};

Graph readData()
{
    string distanceType;
    cin>>distanceType;
    int numNodes;
    cin>>numNodes;
    vector<graphNode> nodes;
    rep(i,numNodes)
    {
        double x,y;
        cin>>x>>y;
        nodes.eb(graphNode(x,y,i));
    }

    vector<vector<double>> distMatrix;
    rep(i,numNodes)
    {
        vector<double> nodeDist;
        rep(j,numNodes)
        {
            double x;
            cin>>x;
            nodeDist.eb(x);
        }
        distMatrix.eb(nodeDist);
    }

    return Graph(nodes,distMatrix);

}

double TSPfitness(vector<int> &gene,Graph &graph)
{
    return graph.getHeuristicCost(gene);
}

vector<vector<int>> rePopulate(vector<vector<int>> &population, int reqSize)
{
    int n=population[0].size();
    vector<vector<int>> currPopulation=population;
    while(currPopulation.size() != reqSize)
    {
        vector<vector<int>> parents;
        sample(population.begin(),population.end(),back_inserter(parents),2,mt19937{random_device{}()});
//        int start=rand() % (n/4);
//        int stop=start+n/2+(rand() % (n/4))-1;
        int start=rand() % (n/2);
        int stop=start+(rand()%(n/2))+1;
        vector<int> transplant(parents[0].begin()+start,parents[0].begin()+stop+1);
        for(int i=0;i<transplant.size();i++)
        {
            parents[1].erase(find(parents[1].begin(),parents[1].end(),transplant[i]));
        }
        vector<int> offspring=parents[1];
        for(auto &g: transplant)
        {
            offspring.eb(g);
        }
        currPopulation.eb(offspring);
    }
    //cout<<"repopulation complete!\n";
    return currPopulation;
}

struct GeneticAlgorithm
{
    int populationSize=-1;
    vector<vector<int>> population;
    Graph graph;
    GeneticAlgorithm(Graph graph,vector<vi> tours, int populationSize)
    : populationSize(populationSize),  graph(graph)
    {
        vector<int> tour;
        rep(i,graph.numNodes)
        {
            tour.eb(i);
        }
        //cout<<"test 1\n";
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        population = tours;
        int currSize = population.size();
        populationSize = tours.size();
        rep(i,populationSize-currSize)
        {
            shuffle(tour.begin(),tour.end(),default_random_engine(seed));
            population.eb(tour);
        }

    }
    double fitness(vector<int> gene)
    {
        return graph.getHeuristicCost(gene);
    }

    void selection()
    {
        int n=populationSize;
        multimap<double,vector<int>> temp;
        for(auto gene:population)
        {
            temp.insert(mp(fitness(gene),gene));
        }
        vector<vi> newPopulation;
        int madara=n/5;
        for(auto p:temp)
        {
            if(!(madara))
                break;
            newPopulation.eb(p.second);
            madara--;
        }
        population=newPopulation;
        //cout<<"New population size after selection: "<<population.size()<<endl;
    }

    void crossover()
    {
        population=rePopulate(population,populationSize);
    }

    void mutation()
    {
        rep(i,population.size())
        {
            int prob=rand() % 10;
            if(prob>7)
            {
                rep(j,rand()%25+1)
                {
                    vi positions;
                    sample(population[i].begin(),population[i].end(),back_inserter(positions),2,mt19937{random_device{}()});
                    swap(population[i][positions[0]],population[i][positions[1]]);
                }
//                next_permutation(population[i].begin(),population[i].end());
            }
        }
    }

    bool isBetterGene(vi g1,vi g2)
    {
        return fitness(g1)<fitness(g2);
    }

    vi getBestGene()
    {
        vi bestGene=population[0];
        for(auto gene:population)
        {
            if(isBetterGene(gene,bestGene))
            {
                bestGene=gene;
            }
        }
        return bestGene;
    }

    vi evolve()
    {
        selection();
        crossover();
        mutation();
        vi hashirama=getBestGene();
        return hashirama;
    }
};

multimap<double,vi> greedyAlgo(Graph graph)
{
    multimap<double,vi> tours;
    rep(i,graph.numNodes)
    {
        graph.makeAllNodesUnvisited();
        vi tour;
        auto neighbours=graph.getNeighbours(i);
        graph.changeNodeColor(neighbours.begin()->second,Color::BLACK);
        graph.changeNodeColor((++neighbours.begin())->second,Color::BLACK);
        tour.eb(neighbours.begin()->second);
        tour.eb((++neighbours.begin())->second);
        int ind=(++neighbours.begin())->second;
        while(tour.size()!=graph.numNodes)
        {
            ind=graph.getNearestUnvisitedNeighbour(ind);
            tour.eb(ind);
            graph.changeNodeColor(ind,Color::BLACK);
        }
        double cost=graph.getHeuristicCost(tour);
        tours.insert(mp(cost,tour));
    }
    return tours;
}

int main() {
    fastio;
    srand(time(0));
    Graph graph=readData();
    multimap<double,vi> greedyTours=greedyAlgo(graph);
    vector<vi> tours;
    auto b=greedyTours.begin();
    auto e=next(b,50);
    for(auto it=b;it!=e;it++)
    {
        tours.eb(it->second);
    }
    for(auto v:tours) printVector(v);
    GeneticAlgorithm AI(graph,tours,50);
    auto timeout=chrono::system_clock::now()+chrono::duration<int>(298) ;
    vi bestTour=AI.evolve();
    double bestTourLength=AI.fitness(bestTour);
    int genNumber=0;
    while(chrono::system_clock::now()<timeout)
    {
        genNumber++;
        //cout<<genNumber<<endl;
        vi currGenBestTour=AI.evolve();
        double currBestLength=AI.fitness(currGenBestTour);
        if(currBestLength<bestTourLength)
        {
            bestTour=currGenBestTour;
            bestTourLength=currBestLength;
            cout<<"New best tour: "<<bestTourLength<<"(generation "<<genNumber<<")\n";
            printVector(bestTour);
        }
    }
    cout<<"Evolution stopped!\n";
    cout<<"New best tour: "<<bestTourLength<<"(generation "<<genNumber<<")\n";
    printVector(bestTour);
}