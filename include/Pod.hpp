#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Container.hpp"


class Pod {
    private:
        std::string name;
        int replicas;
        std::vector<std::shared_ptr<Container>> containers;  // shared_ptr for memory safety

    public:
        Pod(const std::string& name);


        void addContainer(const std::shared_ptr<Container>& container);
        void scale(int newReplicas);


        // sum of all containers multiplied by replicas
        int getTotalCPU() const;
        int getTotalMemory() const;


        std::string getName() const;
        int getReplicas() const;
        std::vector<std::shared_ptr<Container>> getContainers() const; 
};