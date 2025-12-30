#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Server.hpp"
#include "Pod.hpp"
#include "SchedulerStrategy.hpp"


class Cluster {
    private:
        std::vector<std::shared_ptr<Server>> servers;
        std::unique_ptr<SchedulerStrategy> scheduler;

        // track pods by logical name
        std::unordered_map<std::string, std::vector<std::shared_ptr<Pod>>> deployments;

    public:

        explicit Cluster(std::unique_ptr<SchedulerStrategy> schedulerStrategy);

        void addServer(const std::shared_ptr<Server>& server);

        bool deployPod(const std::shared_ptr<Pod>& pod);
        bool scalePod(const std::string& podName, int replicas);

        void showStatus() const;

        void setScheduler(std::unique_ptr<SchedulerStrategy> schedulerStrategy);
};