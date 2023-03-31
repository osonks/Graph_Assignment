#include "graph.h"
#include <vector>

int main() {
    
    Graph g(4);
    g.add_edge(0,1,4);
    g.add_edge(1,0,4);
    g.add_edge(2,3,2);
    g.add_edge(3,2,2);
    g.add_edge(3,0,3);
    g.add_edge(0,3,3);

    g.print();
    
    g.get_reachable_from(1);
    cout<<endl;
    bool t=g.is_undirected();
    cout<<t;
    return 0;
}