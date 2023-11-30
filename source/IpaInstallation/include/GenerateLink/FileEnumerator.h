
#include <string>
#include <filesystem>

class FileEnumerator {
public:

    void enumerate(std::string relativePath);

private:

    /// @brief 根目录
    std::filesystem::path rootDir;

    /// @brief 递归检查文件夹内是否有 IPA 文件
    /// 通过后缀判断
    /// @param destinationPath 指定路径（相对）
    void checkFolderHasIpaRecursive(std::filesystem::path destinationPath);

    /**
     * @brief 根据 IPA 文件名和所在文件夹生成 Manifest.plist 文件
     * @warning 不会覆盖已有文件，建议更新 IPA 文件及文件名，作为一个新的文件被识别
     * @param destinationFolder ipa 文件所在文件夹（相对 rootDir）
     * @param ipaName ipa 文件名，用来生成 Manifest.plist
     * @return manifest 文件的下载链接
    */
    std::string createManifestFils(std::filesystem::path destinationFolder, std::string ipaName);

    /**
     * @brief 根据 IPA 文件名和所在文件夹生成 Manifest.plist 文件对应的下载链接二维码
     * @warning 不会覆盖已有文件，建议更新 IPA 文件及文件名，作为一个新的文件被识别
    */
    void createQrCode(std::filesystem::path destinationFolder, std::string ipaDownloadPath, std::string ipaName);

    bool writePng(const char *filename, const std::vector<std::uint8_t> &image, int size, int stride);
};
