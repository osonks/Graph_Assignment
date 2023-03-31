#include <list>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include "min_pq.h"
#include <cfloat>
using namespace std;

// Represents a directed edge
class Edge {
public:
    int from;
    int to;
    double weight;

    Edge(int from, int to, double weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};


// Represents a directed weighted graph. Can also
// be used to represent an undirected weighted graph.
class Graph { 
private:
	int V;             // number of vertices 
	list<Edge> *adj;   // adjacency lists

public:
	Graph(int V) { 
        this->V = V;
        adj = new list<Edge>[V];
    }
	~Graph() { delete [] adj; }
    int get_V() const { return V; }

    // adds an edge from v to w with the given weight
    void add_edge(int v, int w, double weight) { 
        if (weight < 0)
            throw string("Negative edge weights are not supported.");
        adj[v].push_back(Edge(v, w, weight)); 
    }

    // checks if there is an edge from v to w
    bool has_edge(int v, int w) const {
        for (Edge edge : adj[v])
            if (edge.to == w)
                return true;
        return false;
    }

    // prints the graph
    void print() const {
        for (int v = 0; v < V; v++) {
            cout << "[" << v << "] : ";
            for (Edge edge : adj[v])
                cout << edge.to << ", ";
            cout << endl;
        }
    }	

    // Q1
    bool is_undirected() const;

    // Q2.A
	vector<int> get_reachable_from(int s) const;

    // Q2.B
	vector<vector<int>> connected_components() const;

    // Q3
	void shortest_paths(const vector<int>& sources,
                        vector<double>& dist,
                        vector<int>& parent) const;
};


// Returns true if the graph can be considered an undirected
// graph. I.e. if for every edge v -> w there is also an edge w -> v
// with the same weight.
bool Graph::is_undirected() const {
    
    int a[V][V];    

    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            a[i][j]=-1;
        }
    }
    
    for(int v=0;v<V;v++){
        for (Edge edge : adj[v]){
            a[edge.from][edge.to]=edge.weight;
        }
    }
    
    bool flag=true;
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            if(a[i][j] != a[j][i]){flag=false;}
        }
    }

    return flag;   
}


// Returns all the vertices in the graph that are
// reachable from the given {source} vertex.
vector<int> Graph::get_reachable_from(int s) const {
    vector<int> result;
    if(s < 0 || s >= V){
        string error = "Invalid vertex";
        throw error;
    }
    queue <int> q;
    int u;
    int color[V];
    for(int i=0;i<V;i++){color[i]=0;}
    color[s]=1;
    q.push(s);
    result.push_back(s);
    while(!q.empty()){
        u =q.front();
        q.pop();
        for (Edge edge : adj[u]){
            if (color[edge.to] == 0){
                color[edge.to]=1;
                q.push(edge.to);
                result.push_back(edge.to);
            }
        }
        color[u]=2;
    }
    
	return result;
}

// Returns all the connected components in the graph assuming
// that the graph is undirected. The vertices of each component
// are stored in a separate vector.
vector<vector<int>> Graph::connected_components() const {
    vector<vector<int>> result;
    int *done = new int[V];
    if(!is_undirected()){throw string("not undirected");}
    for(int i=0;i<V;i++)done[i]=0;

    vector<int> v; 
    for(int i=0;i<V;i++){
        if(done[i] == 0){
            //v.clear();
            v=get_reachable_from(i);  
            result.push_back(v);
            for(int j=0 ; j<v.size() ; j++) done[v[j]] = 1;
        }
    }  

    return result;
}

// Computes the shortest paths from the given source vertices
// to every vertex in the graph.
// Store the result in the given {dist} and {parent} vectors,
// where the {dist} vector stores the shortest distances and
// the {parent} vector stores the shortest paths.
void Graph::shortest_paths(const vector<int>& sources,
                           vector<double>& dist,
                           vector<int>& parent) const {
    
	dist.clear();
    parent.clear();

    MinPQ <double> pq(V);
    
    for(int i=0;i<V;i++){
        dist.push_back(DBL_MAX);
        parent.push_back(-1);
    }

    for(int i=0 ; i<sources.size() ; i++){
        if(sources[i] < 0 || sources[i] >= V){
            string error = "Invalid vertex " ;
            throw error;
        }
    }

    
    for(int i=0;i<sources.size();i++){dist[sources[i]]=0;}
    for(int i=0;i<V;i++)pq.insert(i,dist[i]);
    
    while(!pq.isEmpty()){
        int u = pq.delMin();
        
        for(Edge edge: adj[u]){
            if(dist[u] + edge.weight < dist[edge.to]){
                dist[edge.to] = dist[u] + edge.weight;
                parent[edge.to] = u;
                pq.decreaseKey(edge.to,dist[edge.to]);
            }
        }
    }

}

