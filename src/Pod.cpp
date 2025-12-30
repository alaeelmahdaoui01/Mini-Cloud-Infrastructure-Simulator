#include "Pod.hpp"

Pod::Pod(const std::string& name)
    : name(name), replicas(1) // default 1 replica
{}

void Pod::addContainer(const std::shared_ptr<Container>& container) {
    containers.push_back(container);
}

void Pod::scale(int newReplicas) {
    if (newReplicas < 1) {
        replicas = 1; // always at least 1 replica
    } else {
        replicas = newReplicas;
    }
}

int Pod::getTotalCPU() const {
    int total = 0;
    for (const auto& c : containers) {
        total += c->getCPU();
    }
    return total * replicas;
}

int Pod::getTotalMemory() const {
    int total = 0;
    for (const auto& c : containers) {
        total += c->getMemory();
    }
    return total * replicas;
}

std::string Pod::getName() const {
    return name;
}

int Pod::getReplicas() const {
    return replicas;
}

std::vector<std::shared_ptr<Container>> Pod::getContainers() const {
    return containers;
}

