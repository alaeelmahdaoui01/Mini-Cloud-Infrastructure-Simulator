#include "Server.hpp"
#include <algorithm>

Server::Server(const std::string& id, int cpu, int memory)
    : id(id), totalCPU(cpu), totalMemory(memory), usedCPU(0), usedMemory(0)  {}

// Check if the server has enough free resources
bool Server::canHost(const Pod& pod) const {
    return (usedCPU + pod.getTotalCPU() <= totalCPU) && (usedMemory + pod.getTotalMemory() <= totalMemory);
}

// Deploy a pod if resources allow
bool Server::deployPod(const std::shared_ptr<Pod>& pod) {
    if (!canHost(*pod)) {
        return false;
    }

    pods.push_back(pod);
    usedCPU += pod->getTotalCPU();
    usedMemory += pod->getTotalMemory();

    return true;
}

// Remove a pod and free resources
void Server::releasePod(const std::string& podName) {
    auto it = std::remove_if(pods.begin(),pods.end(),
                [&](const std::shared_ptr<Pod>& pod) {return pod->getName() == podName;} );

    // in case Pod names aren't unique
    for (auto itr = it; itr != pods.end(); ++itr) {
        usedCPU -= (*itr)->getTotalCPU();
        usedMemory -= (*itr)->getTotalMemory();
    }

    pods.erase(it, pods.end());
}

// Load = max(CPU usage %, Memory usage %)
float Server::getLoad() const {
    float cpuLoad = static_cast<float>(usedCPU) / totalCPU;
    float memLoad = static_cast<float>(usedMemory) / totalMemory;
    return std::max(cpuLoad, memLoad);
} // this method is important for scheduling bcs a server is loaded if either CPU or memory is tight

std::string Server::getId() const {
    return id;
}

bool Server::removePod(const std::shared_ptr<Pod>& pod) {
    auto it = std::find(pods.begin(), pods.end(), pod);
    if (it == pods.end()) return false;

    usedCPU -= pod->getTotalCPU();
    usedMemory -= pod->getTotalMemory();

    pods.erase(it);
    return true;
}

