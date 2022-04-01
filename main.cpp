#include <bits/stdc++.h>
#define CITY_GRAPH_FILE_NAME "city_graph.txt"
using namespace std;

vector<vector<pair<int,int>>> graph; /// first -> weight, second -> node

// Mapping from integer to string and vice versa
map<string,int> city_name_to_ind;
vector<string> ind_to_city_name;
int ind = 0;  //Variable for obtaining the index of each node

//The function receives the reference of the city name string and map it to an index
//Input: The city string reference
//Output: An index
int get_ind(const string & city_name){
  auto it = city_name_to_ind.find(city_name);
  if(it==city_name_to_ind.end()){
    graph.push_back({});
    ind_to_city_name.push_back(city_name);
    return city_name_to_ind[city_name]=ind++;
  }else
    return it->second;
}

//To print the graph
//Input: None
//Output: Graph
void read_city_graph(){
    ifstream in (CITY_GRAPH_FILE_NAME);
    string a,b;
    int w;
    while(in >> a >> b >> w){
      auto a_ind = get_ind(a);
      auto b_ind = get_ind(b);
      graph[a_ind].push_back({w,b_ind});
      graph[b_ind].push_back({w,a_ind});
    }
    for(auto & l:graph)
      sort(l.begin(),l.end());
}

//Greedy algorithm
//Input: Source node int, Destination node, Array of visited, The best last node selected
//Output: The distance if the node 
int greedy_f(int a,const int & b,vector<bool> & v,vector<int> & last){
  if(a==b)
    return 0;
  for(auto edge : graph[a])
    if(!v[edge.second]){
      v[edge.second]=true;
      auto s = greedy_f(edge.second,b,v,last);
      if(s!=INT_MAX){
        last[edge.second]=a;
        return s + edge.first;
      }
    }
  return INT_MAX;
}

//Wrapper function for the greedy algorithm
//Input: Source node string, Destination node string
//Output: The distance between a and b node
int greedy(const string & a, const string & b){
  auto a_ind = get_ind(a);
  auto b_ind = get_ind(b);
  vector<bool> v(ind);
  v[a_ind]=true;
  cout << a << ' ';
  vector<int> last(ind);
  
  int ans = greedy_f(a_ind,b_ind,v,last);
  stack<int> s;

  //Push the nodes to a stack to turn it around
  for(;b_ind!=a_ind;b_ind=last[b_ind])
    s.push(b_ind);
  for(;!s.empty();s.pop())
    cout << ind_to_city_name[s.top()] << ' ';
  
  return ans;
}

//A* Algorithm
//Input: Source node string, Destination node string
//Output: The distance between a and b node
int a_star(const string & a, const string & b){
  auto a_ind = get_ind(a);
  auto b_ind = get_ind(b);
  vector<int> costs(ind);
  vector<int> last(ind);
  fill(costs.begin(),costs.end(),INT_MAX);
  costs[a_ind]=0;  //Total distance
  vector<bool> v(ind);
  priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q; /// first -> weight, second -> node
  
  q.push({graph[a_ind][0].first,a_ind});
  while(!q.empty()){
    auto c = q.top();
    if(c.second==b_ind)
      break;
    q.pop();
    if(!v[c.second]){
        //cout << "c: " << c.first << ' ' << ind_to_city_name[c.second] << '\n';
        v[c.second]=true;
        for(auto & edge:graph[c.second])
        if(!v[edge.second] && costs[c.second]+edge.first<costs[edge.second]){
            last[edge.second] = c.second;  //The best last visited node
            q.push({(costs[edge.second] = costs[c.second]+edge.first)+ graph[edge.second][0].first,edge.second});
        }
    }
  }

  //Push the nodes to a stack to turn it around
  stack<int> s;
  auto result = costs[b_ind];
  for(;b_ind!=a_ind;b_ind=last[b_ind])
    s.push(b_ind);
  s.push(b_ind);
  for(;!s.empty();s.pop())
      cout << ind_to_city_name[s.top()] << ' ';
  
  return result;
}

//Main function
int main() {
  read_city_graph();
  string a,b;
  cout << "Source City: ",cin >> a;
  cout << "Objective City: ",cin >> b;
  cout << "Greedy solution: \n";
  cout << greedy(a,b) << '\n';
  cout << "A* solution: \n";
  cout << a_star(a,b) << '\n';
  return 0;
}