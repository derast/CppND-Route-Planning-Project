#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>
#include <vector>

using std::unordered_map;
using std::vector;

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        // Add public Node variables and methods here.  
      	Node* parent = nullptr;
      	float h_value = std::numeric_limits<float>::max();
      	float g_value = 0.0;
      	bool visited = false;
      	vector<Node *> neighbors;
      	int checkDistance = 100;
      
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
       	float distance(Node node) const {
          return sqrt(std::pow((this->x - node.x), 2) + std::pow((this->y - node.y), 2));
        }
      	void FindNeighbors();
      	float GetFValue();
      
      private:
        // Add private Node variables and methods here.
        int index;
        RouteModel * parent_model = nullptr;
      
      	RouteModel::Node* FindNeighbor(vector<int> node_indices);
    };
    
    // Add public RouteModel variables and methods here.
  	std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
  
    RouteModel(const std::vector<std::byte> &xml);  
  	auto &SNodes() { return m_Nodes; }
	  auto &GetNodeToRoadMap() {
      return this->node_to_road;
    }
  	Node& FindClosestNode(float x, float y);
  
  private:
    // Add private RouteModel variables and methods here.
  	vector<Node> m_Nodes;
  	unordered_map<int, vector<const Model::Road*>> node_to_road;

  	void CreateNodeToRoadHashmap();
};

#endif