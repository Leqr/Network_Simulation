//
//  network.cpp
//  NetSim
//
//  Created by LEqr on 20.10.18.
//

#include <stdio.h>
#include "network.h"
#include <list>
#include <set>
#include "random.h"
#include <algorithm>
#include <random>

void Network::resize(const size_t &size){
    values.clear();
    for(size_t i(0); i < size ; ++i){
        values[i] = RNG.uniform_double(-100,100);
    }
}

bool Network::add_link(const size_t &a, const size_t &b){
    try{
        links.insert({{a,b}, {b,a}});
        return true;
    }
    catch(const std::bad_alloc &e){
        return false;
    }
}

size_t Network::random_connect(const double &mean){
    std::set<size_t> nodestorage;
    std::vector<size_t> vector_nodestorage(nodestorage.begin(), nodestorage.end()); // store every nodes size_t
    for(auto &e : links) nodestorage.insert(e.first);
    links.clear();
    unsigned int link_sum(0);
    for(auto &e: nodestorage){
        unsigned int degree(RNG.poisson(mean));
        for(int i(0); i < degree ; ++i){
            int randomIndex;
            do{
                randomIndex = rand() % vector_nodestorage.size();
                links.insert({e,randomIndex});
            }while(randomIndex == e);
        }
        link_sum += degree;
    }
    return link_sum;
}

size_t Network::set_values(const std::vector<double> &new_nodes){
    for(size_t i(0); i < new_nodes.size() ; ++i){
        values[i] = new_nodes[i];
    }
    int diff = values.size()-new_nodes.size();
    if (diff >= 0) return diff;
    else return values.size();
}

size_t Network::size() const{
    std::set<size_t> nodestorage;
    for(auto &e : links) nodestorage.insert(e.first);
    return nodestorage.size();
    
    //values.size(); should give the same result
}

size_t Network::degree(const size_t &_n) const {
    return links.count(_n);
}

double Network::value(const size_t &_n) const {
    return values[_n];
}

std::vector<double> Network::sorted_values() const {
    std::vector<double> sorted_values = values;
    std::sort(sorted_values.begin(), sorted_values.end());
    return sorted_values;
}

std::vector<size_t> Network::neighbors(const size_t &) const {
    std::vector<size_t> neighbors;
    std::pair <std::multimap<size_t,size_t>::iterator, std::multimap<size_t,size_t>::iterator> ret;
    for (std::multimap<size_t,size_t>::iterator it=ret.first; it!=ret.second; ++it){
        neighbors.push_back(it->second);
    }
    return neighbors;

}


