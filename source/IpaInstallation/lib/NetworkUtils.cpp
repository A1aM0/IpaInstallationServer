
#include "Networker/NetworkUtils.h"
#include <iostream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

int NetworkUtils::port = 9090;

std::string NetworkUtils::localHost = "http://0.0.0.0";

std::string NetworkUtils::GetLocalHostUrlString() {
    std::string localhostString = localHost + ":" + std::to_string(port);
    return localhostString;
}

std::vector<std::string> NetworkUtils::GetLocalIpv4Addresses() {
    std::vector<std::string> localIPs;
    struct ifaddrs *ifaddr, *ifa;
    int family;
    char host[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return localIPs;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET) { // Check if it is IPv4
            void* tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, host, INET_ADDRSTRLEN);
            if (IsPrivateIPv4(host)) {
                localIPs.push_back(std::string(host));
            }
        }
    }

    freeifaddrs(ifaddr);
    return localIPs;
}

std::vector<std::string> NetworkUtils::GetAndPrintLocalIpv4Addresses() {
    std::vector<std::string> addresses = GetLocalIpv4Addresses();
    std::cout << "Local ip address:" << std::endl;
    for (auto address: addresses) {
        std::cout << "http://" << address << ":9090" << std::endl;
    }
    return addresses;
}

bool NetworkUtils::IsPrivateIPv4(const char* ip) {
    return (strncmp(ip, "192.168.", 8) == 0 ||
            strncmp(ip, "10.", 3) == 0 ||
            (strncmp(ip, "172.", 4) == 0 && ip[4] >= '1' && ip[4] <= '9'));
}

