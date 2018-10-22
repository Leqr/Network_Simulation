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
#include <random>

void Network::resize(const size_t &size){
    
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
    std::vector<size_t> vector_nodestorage(nodestorage.begin(), nodestorage.end());
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
    
}

size_t Network::size() const{
    std::set<size_t> nodestorage;
    for(auto &e : links) nodestorage.insert(e.first);
    return nodestorage.size();
}

size_t Network::degree(const size_t &_n) const {
    
}


