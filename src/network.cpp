//
//  network.cpp
//  NetSim
//
//  Created by Leqr on 20.10.18.
//

#include <iostream>
#include "network.h"
#include "random.h"
#include <string>

void Network::resize(const size_t &size){
    links.clear();
    values.clear();
    for(size_t i(0); i < size ; ++i){
        values.push_back(RNG.normal(0,1));
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
        //std::cerr << e << std::endl;  //displays errors on the terminal
        return false;
    }
}

//Method 1, pass the test, for every node add degree(n) links
size_t Network::random_connect(const double &mean){
    links.clear();
    unsigned int total_link_sum(0);
    std::vector<size_t> res;    //vector with the indexes of the nodes that the current node can link to.
    for(size_t i(0); i < values.size() ; ++i){
        res.push_back(i);
    }
    for(size_t e(0); e < values.size(); ++e){
        try{
            RNG.shuffle(res);
            unsigned int degree;
            unsigned int count_poisson(0);
            do{
                degree = RNG.poisson(mean);
                ++count_poisson;
                if(count_poisson > 100) throw std::runtime_error("ERROR : can't make that many links for a node");
            }while(degree > res.size());
            // the max number of links in the network for n nodes is (n-1)+(n-2)+...+2+1 = (n-1)n/2
            unsigned int index(0);
            unsigned int linked(0);
            while(linked < degree){
                bool added(false);
                do{
                    added = this->add_link(e, res[index]);
                    ++index;
                    if(index >= values.size()-1) throw std::out_of_range("ERROR : node has (n-1) links");
                }while(!added);
                ++linked;
            }
            total_link_sum += linked;
        } catch(const std::exception& e){
            //std::cerr << e.what() << std::endl; //displays errors on the terminal
        }
    }
    return total_link_sum;
}

//Method 2, doesn't pass EXPECT_NEAR connect, for every node takes into account what is already created and which nodes still have space for new links.
/*size_t Network::random_connect(const double &mean){
    links.clear();
    unsigned int total_link_sum(0);
    std::vector<size_t> res;    //vector with the indexes of the nodes that the current node can link to.
    std::vector<unsigned int> poisson_degree; //all the degrees for every node ordered by indexes.
    for(size_t i(0); i < values.size() ; ++i){
        res.push_back(i);
        poisson_degree.push_back(RNG.poisson(mean));
    }
    for(size_t e(0); e < values.size(); ++e){
        try{
            RNG.shuffle(res);
            // the max number of links in the network for n nodes is (n-1)+(n-2)+...+2+1 = (n-1)n/2
            unsigned int index(0);
                while(this->degree(e) < poisson_degree[e]){
                    bool added(false);
                    do{
                        if(this->degree(res[index]) < poisson_degree[res[index]]){
                            added = this->add_link(e, res[index]);
                        }
                        ++index;
                        if(index >= values.size()-1) throw std::out_of_range("ERROR : node has (n-1) links");
                    }while(!added);
                    ++total_link_sum;
            }
        } catch(const std::exception& e){
            //std::cerr << e.what() << std::endl; //displays errors on the terminal
        }
    }
    return total_link_sum;
}*/

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
