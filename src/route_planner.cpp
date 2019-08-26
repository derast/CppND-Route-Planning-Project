#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
  start_x *= 0.01;
  start_y *= 0.01;
  end_x /= 100;
  end_y /= 100;
  start_node = &m_Model.FindClosestNode(start_x, start_y);
  end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::GetDistance() const{
  return distance;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_node) {
  std::cout<<"here";
  std::vector<RouteModel::Node> path_found;
  this->distance = 0.0f;
  while(current_node->parent != nullptr) {
    path_found.push_back(*current_node);
  	distance += current_node->distance(*(current_node->parent));
    current_node = current_node->parent;
  }
  path_found.push_back(*current_node);
  distance *= m_Model.MetricScale();
  
  return path_found;
}

void RoutePlanner::AStarSearch() {
  this->start_node->visited = true;
  open_list.push_back(start_node);
  RouteModel::Node* current_node = nullptr;
  while(open_list.size() > 0) {
  	current_node = this->NextNode();
    if(current_node->distance(*end_node) == 0) {
      m_Model.path = ConstructFinalPath(current_node);
      return;
    }
    this->AddNeighbors(current_node);
  }
}

float RoutePlanner::CalculateHValue(RouteModel::Node* node) {
  return node->distance(*end_node);
}

RouteModel::Node* RoutePlanner::NextNode() {
  std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd){
  	return _1st->GetFValue() < _2nd->GetFValue();
  });
  
  RouteModel::Node* lowest_node = open_list.front();
  open_list.erase(open_list.begin());

  return lowest_node;
}

bool RoutePlanner::Compare(const auto &n1, const auto &n2) {
  return n1->GetFValue() < n2->GetFValue();
}

void RoutePlanner::AddNeighbors(RouteModel::Node* current_node) {
  current_node->FindNeighbors();
    for (auto neighbor : current_node->neighbors) {
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        neighbor->h_value = CalculateHValue(neighbor);
        open_list.push_back(neighbor);
        neighbor->visited = true;
    } 
}
