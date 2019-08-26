#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int count = 0;
  for(auto node : this->Nodes()) {
  	auto r_node = Node(count, this, node); 
    m_Nodes.push_back(r_node);
    count++;
  }
  this->CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap() {
  for(auto &road : Roads()) {
  	if(road.type != Model::Road::Type::Footway) {
      for(auto node_idx : Ways()[road.way].nodes) {
      	auto map_index = this->node_to_road.find(node_idx);
        if(map_index == this->node_to_road.end()) {
          this->node_to_road[node_idx] = vector<const Model::Road*>();
          //this->node_to_road[node_idx].push_back(&road);  //THIS WAS AN ERROR
        }
        this->node_to_road[node_idx].push_back(&road); 
      }
    }
  }
}

RouteModel::Node* RouteModel::Node::FindNeighbor(vector<int> node_indices) {
  Node* closest_node = nullptr;
  Node node;
  for(int node_index : node_indices) {
    node = parent_model->SNodes()[node_index];
    float distance_to_node = distance(node);
    if(distance_to_node != 0 && !node.visited ) {
      if(closest_node == nullptr || distance_to_node < distance(*closest_node)) {
      	closest_node = &parent_model->SNodes()[node_index];  //ERROR WAS &node(local variable than launch ABORTSEG)
      }
    }
  }
  
  return closest_node;
}

//THERE ARE DIFFERENCES BUT LOOK OK
void RouteModel::Node::FindNeighbors() {
  auto roads = parent_model->node_to_road[this->index];
  for(auto &road : roads) {
	auto node_indices = parent_model->Ways()[road->way].nodes;
    RouteModel::Node* closets_node = this->FindNeighbor(node_indices);
    if(closets_node) {
    	this->neighbors.emplace_back(closets_node);
    }
  }
}
//THERE ARE DIFFERENCES BUT LOOK OK
RouteModel::Node& RouteModel::FindClosestNode(float x, float y) {
  Node tmp_node; 
  tmp_node.x = x;
  tmp_node.y = y;
  float min_dist = std::numeric_limits<float>::max();
  float dist;
  int closets_idx;
  for(auto &road : Roads()) {
  	if(road.type != Model::Road::Type::Footway) {
      for(int node_idx : Ways()[road.way].nodes) {
      	dist = tmp_node.distance(SNodes()[node_idx]);
        if(dist < min_dist) {
          closets_idx = node_idx;
          min_dist = dist;
        }
      }
    }
  }
  
  return SNodes()[closets_idx];
}

float RouteModel::Node::GetFValue() {
  return this->h_value + this->g_value;
}

