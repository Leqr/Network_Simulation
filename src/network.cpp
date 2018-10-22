//
//  network.cpp
//  NetSim
//
//  Created by Leqr on 20.10.18.
//

#include <iostream>
#include "network.h"
#include <list>
#include <set>
#include "random.h"
#include <algorithm>
#include <string>

void Network::resize(const size_t &size){
    values.clear();
    for(size_t i(0); i < size ; ++i){
        values.push_back(RNG.uniform_double(-100,100));
    }
}

bool Network::add_link(const size_t &a, const size_t &b){
    try{
        if(a == b) throw std::string("ERROR : can't link nodes with themselves");
        if((a > values.size()-1) or (b > values.size()-1)) throw std::string("ERROR : a or b is not an index of the values vector") ;
        if(links.size() != 0){
            auto search_a = links.equal_range(a);
            auto search_b = links.equal_range(b);
            for (auto i = search_a.first; i != search_a.second; ++i){
                if(i->second == b) throw std::string("ERROR : a already linked to b");
            }
            for (auto i = search_b.first; i != search_b.second; ++i){
                if(i->second == a) throw std::string("ERROR : b already linked to a");
            }
        }
        links.insert({{a,b}, {b,a}});
        return true;
    }
    catch(const std::string &e){
        std::cerr << e << std::endl;
        return false;
    }
}

size_t Network::random_connect(const double &mean){
    links.clear();
    unsigned int link_sum(0);
    for(size_t e(0); e < values.size()-1;++e){
        unsigned int degree(RNG.poisson(mean));
        bool added;
        for(int i(0); i < degree ; ++i){
            std::vector<int> res{1};
                RNG.uniform_int(res,0,values.size()-1);
                added = this->add_link(e, res[0]);
        }
        if(added) link_sum += 2*degree;
    }
    return link_sum;
}

size_t Network::set_values(const std::vector<double> &new_nodes){
    if(new_nodes.size() <= values.size()){
        for(size_t i(0); i < new_nodes.size() ; ++i){
            values[i] = new_nodes[i];
        }
        return new_nodes.size();
    } else {
        for(size_t i(0); i < values.size() ; ++i){
            values[i] = new_nodes[i];
        }
        return values.size();
    }
}

size_t Network::size() const{
    
    return values.size();
    
    /*
    std::set<size_t> nodestorage;
    for(auto &e : links) nodestorage.insert(e.first);
    return nodestorage.size();
    unused part but should give the same result if links is updated with respect to values and if every node has at least one link.
     */
}

size_t Network::degree(const size_t &_n) const {
    return links.count(_n);
}

double Network::value(const size_t &_n) const {
    return values[_n];
}

std::vector<double> Network::sorted_values() const {
    std::vector<double> sorted_values = values;
    std::sort(sorted_values.begin(), sorted_values.end(),std::greater<double>());
    return sorted_values;
}

std::vector<size_t> Network::neighbors(const size_t &n) const {
    std::vector<size_t> neighbors;
    auto range = links.equal_range(n);
    for (auto it=range.first; it!=range.second; ++it){
        neighbors.push_back(it->second);
    }
    return neighbors;
}


