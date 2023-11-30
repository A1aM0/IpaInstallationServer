
#include <string>
#include <vector>

class NetworkUtils {

public:
    static std::string GetLocalHostUrlString();

    /// @brief 获取局域网 ipv4 地址
    /// @return 地址数组
    static std::vector<std::string> GetLocalIpv4Addresses();

    /// @brief 获取并打印 ipv4 地址
    /// @return 地址数组
    static std::vector<std::string> GetAndPrintLocalIpv4Addresses();

private:

    int static port;
    std::string static localHost;
    static bool IsPrivateIPv4(const char* ip);

};
