import networkx as nx
import matplotlib.pyplot as plt

# Define a function to get user input for the start and end nodes
def get_start_end_nodes(graph):
    start_node = None
    end_node = None
    while start_node is None or end_node is None:
        # Draw the graph with the current start and end nodes highlighted
        plt.clf() # Clear the previous graph
        pos = nx.spring_layout(graph)
        nx.draw_networkx(graph, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in graph.nodes()])
        plt.draw()
        plt.pause(0.001)
        
        # Get the next user input
        click_event = plt.ginput(n=1, timeout=0)
        node = None
        for n, (x, y) in pos.items():
            if abs(x - click_event[0][0]) < 0.05 and abs(y - click_event[0][1]) < 0.05:
                node = n
                break
                
        # Update the start or end node if the user clicked on a node
        if node is not None:
            if start_node is None:
                start_node = node
            elif end_node is None and node != start_node:
                end_node = node
    
    return start_node, end_node

# Define the A* algorithm using the networkx implementation
def find_shortest_path(graph, start, end):
    # Set the weight attribute for all nodes in the graph
    nx.set_node_attributes(graph, 0, 'weight')

    # Compute the shortest path using A*
    path = nx.astar_path(graph, start, end, heuristic=lambda u, v: graph.nodes[v]['weight'])
    return path


# Define the main function that creates the graph and runs the A* algorithm
def main():
    # Create the graph
    G = nx.Graph()
    G.add_edge('A', 'B', weight=2)
    G.add_edge('A', 'C', weight=1)
    G.add_edge('B', 'D', weight=3)
    G.add_edge('C', 'D', weight=1)
    G.add_edge('C', 'E', weight=2)
    G.add_edge('D', 'E', weight=1)
    
    # Get the start and end nodes from the user
    start_node, end_node = get_start_end_nodes(G)
    
    # Find the shortest path using A*
    shortest_path = find_shortest_path(G, start_node, end_node)
    
    # Draw the final graph with the shortest path highlighted
    plt.clf() # Clear the previous graph
    pos = nx.spring_layout(G)
    nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in G.nodes()])
    nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=5)
    plt.show()

if __name__ == '__main__':
    main()
