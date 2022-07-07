#include "Operations.h"
#include <thread>
#include <chrono>

void recursiveDFS(Graph* graph, Node* node, std::unordered_set<int>& visited)
{
	visited.insert(node->id);
	node->setFillColor(RED);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	node->setFillColor(BLACK);
	for (const auto& adjNode : graph->adjacentList.at(node))
	{
		if (!visited.contains(adjNode->id))
		{
			recursiveDFS(graph, adjNode, visited);
		}
	}
}
void DFS(Graph* graph)
{
	std::unordered_set<int> visited;

	for (const auto& kv : graph->adjacentList)
	{
		if (!visited.contains(kv.first->id))
		{
			recursiveDFS(graph, kv.first, visited);
		}
	}
}


