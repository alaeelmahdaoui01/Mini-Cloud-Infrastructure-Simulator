#include <iostream>
#include <sstream>
#include <memory>

#include "Cluster.hpp"
#include "Server.hpp"
#include "Pod.hpp"
#include "Container.hpp"

#include "LeastLoadedScheduler.hpp"

int main() {
    // Create cluster with LeastLoaded scheduler
    Cluster cluster(std::make_unique<LeastLoadedScheduler>());

    // Add some servers
    cluster.addServer(std::make_shared<Server>("server-1", 16, 16000));
    cluster.addServer(std::make_shared<Server>("server-2", 8, 8000));

    std::cout << "Mini Cloud Infrastructure Simulator\n";
    std::cout << "Commands: deploy | scale | status | exit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "exit") {
            break;
        }

        else if (command == "deploy") {
            std::string podName;
            int cpu, memory;

            ss >> podName >> cpu >> memory;

            auto pod = std::make_shared<Pod>(podName);
            auto container = std::make_shared<Container>("c1", cpu, memory);
            pod->addContainer(container);

            if (!cluster.deployPod(pod)) {
                std::cout << "Deployment failed.\n";
            }
        }

        else if (command == "scale") {
            std::string podName;
            int replicas;

            ss >> podName >> replicas;

            if (!cluster.scalePod(podName, replicas)) {
                std::cout << "Scaling failed.\n";
            }
        }

        else if (command == "status") {
            cluster.showStatus();
        }

        else {
            std::cout << "Unknown command.\n";
        }
    }

    std::cout << "Exiting simulator.\n";
    return 0;
}
