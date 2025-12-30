#include "Cluster.hpp"
#include <iostream>


// Cluster only orchestrates 


// Constructor
Cluster::Cluster(std::unique_ptr<SchedulerStrategy> schedulerStrategy): scheduler(std::move(schedulerStrategy)) {}

// Add a server to the cluster
void Cluster::addServer(const std::shared_ptr<Server>& server) {
    servers.push_back(server);
}

// Deploy a pod using the selected scheduler strategy
bool Cluster::deployPod(const std::shared_ptr<Pod>& pod) {
    if (!scheduler) {
        std::cerr << "No scheduler configured.\n";
        return false;
    }

    auto server = scheduler->selectServer(*pod, servers);
    if (!server) {
        std::cerr << "Deployment failed: no available server for pod "<< pod->getName() << "\n";
        return false;
    }

    if (!server->deployPod(pod)) {
        std::cerr << "Deployment failed on server " << server->getId() << "\n";
        return false;
    }

    std::cout << "Pod " << pod->getName() << " deployed on server " << server->getId() << "\n";

    return true;
}



// Simplified scaling model (not how Kubernetes actually scales) since this is just a simulator
// it removes old pod, creates a scaled version, and redeploy using scheduler

// Scale an existing pod
// bool Cluster::scalePod(const std::string& podName, int replicas) {
//     // Find the pod and the server hosting it
//     for (const auto& server : servers) {
//         // We rely on Server to manage its pods
//         server->releasePod(podName);
//     }

//     // In a real system we would track pods globally
//     // here we simply redeploy a scaled pod
//     auto scaledPod = std::make_shared<Pod>(podName);
//     scaledPod->scale(replicas);

//     return deployPod(scaledPod);
// }

// Improved scalePod() implementation
// scaling is incremental not destructive and pods are tracked logically 
bool Cluster::scalePod(const std::string& podName, int replicas) {
    if (replicas < 0) return false;

    auto& pods = deployments[podName];
    int current = pods.size();

    // SCALE UP
    if (replicas > current) {
        int toCreate = replicas - current;

        for (int i = 0; i < toCreate; ++i) {
            auto pod = std::make_shared<Pod>(podName + "-" + std::to_string(current + i + 1));

            // Clone container template (simple version)
            for (const auto& c : pods.front()->getContainers()) {
                pod->addContainer(std::make_shared<Container>(*c));
            }

            if (!deployPod(pod)) {
                std::cout << "Scaling failed: insufficient resources\n";
                return false;
            }

            pods.push_back(pod);
        }
    }

    // SCALE DOWN
    else if (replicas < current) {
        int toRemove = current - replicas;

        for (int i = 0; i < toRemove; ++i) {
            auto pod = pods.back();
            pods.pop_back();

            for (auto& server : servers) {
                if (server->removePod(pod)) {
                    break;
                }
            }
        }
    }

    return true;
}




// Display cluster status
void Cluster::showStatus() const {
    std::cout << "\n--- Cluster Status ---\n";

    for (const auto& server : servers) {
        std::cout << "Server " << server->getId() << " | Load: " << server->getLoad() << "\n";
    }

    std::cout << "----------------------\n";
}

// Change scheduling strategy at runtime
void Cluster::setScheduler(std::unique_ptr<SchedulerStrategy> schedulerStrategy) {
    scheduler = std::move(schedulerStrategy);
}
