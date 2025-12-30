#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Pod.hpp"

// Server behavior 

class Server {
    private:
        std::string id;
        int totalCPU;
        int totalMemory;
        int usedCPU;
        int usedMemory;


        std::vector<std::shared_ptr<Pod>> pods;


    public:
        Server(const std::string& id, int cpu, int memory);


        bool canHost(const Pod& pod) const;
        bool deployPod(const std::shared_ptr<Pod>& pod);
        void releasePod(const std::string& podName);


        float getLoad() const;
        std::string getId() const;

        bool removePod(const std::shared_ptr<Pod>& pod);
};