
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "GenerateLink/FileEnumerator.h"
#include "Networker/NetworkUtils.h"
#include "../../third-party/QrCodeGen/QR-Code-generator/cpp/qrcodegen.hpp"
#include "png.h"

namespace fs = std::filesystem;

void FileEnumerator::enumerate(std::string relativePath) {
    rootDir = fs::current_path().append(relativePath);
    checkFolderHasIpaRecursive(rootDir);
}

void FileEnumerator::checkFolderHasIpaRecursive(const fs::path destinationPath) {
    if (!fs::exists(destinationPath)) {
        std::cerr << "文件夹不存在：" << destinationPath.string() << std::endl;
        return;
    }

    // 传入路径是文件夹，继续递归
    if (fs::is_directory(destinationPath)) {
        for (const fs::directory_entry entry: fs::directory_iterator(destinationPath)) {
            fs::path subPath = entry.path();
            checkFolderHasIpaRecursive(subPath);
        }
        return;
    }

    std::string fileExtension = destinationPath.extension().string();
    transform(fileExtension.begin(),fileExtension.end(),fileExtension.begin(),::tolower);
    if (fileExtension != ".ipa") {
        std::cerr << "找到非 IPA 文件：" << destinationPath.string() << std::endl;
        return;
    }

    std::string fileName = destinationPath.filename().string();
    std::cout << "找到 IPA 文件：" << fileName << std::endl;
    fs::path folderPath = destinationPath.parent_path();
    
    std::string downloadUrl = createManifestFils(folderPath, fileName);
    if (!downloadUrl.empty()) {
        createQrCode(folderPath, downloadUrl, destinationPath.stem());
    }
}

std::string FileEnumerator::createManifestFils(fs::path destinationFolder, std::string ipaName) {
    std::string manifestFileName = "manifest.plist";
    fs::path plistFilePath = destinationFolder / manifestFileName;
    
    // 删除原来的 plist，重新生成
    if (fs::exists(plistFilePath)) {
        fs::remove(plistFilePath);
    }
    std::string ipaUrlPath = NetworkUtils::GetLocalHostUrlString() + "/" + destinationFolder.append(ipaName).lexically_relative(rootDir).string();

    std::ofstream plistFile(plistFilePath);
    if (!plistFile.is_open()) {
        std::cerr << "无法创建 manifest.plist 文件：" << plistFilePath.string() << std::endl;
        return std::string();
    }

    plistFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    plistFile << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" ";
    plistFile << "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    plistFile << "<plist version=\"1.0\">\n";
    plistFile << "<dict>\n";
    plistFile << "    <key>items</key>\n";
    plistFile << "    <array>\n";
    plistFile << "        <dict>\n";
    plistFile << "            <key>assets</key>\n";
    plistFile << "            <array>\n";
    plistFile << "                <dict>\n";
    plistFile << "                    <key>kind</key>\n";
    plistFile << "                    <string>software-package</string>\n";
    plistFile << "                    <key>url</key>\n";
    plistFile << "                    <string>" << ipaUrlPath << "</string>\n";
    plistFile << "                </dict>\n";
    plistFile << "                <dict>\n";
    plistFile << "                    <key>kind</key>\n";
    plistFile << "                    <string>display-image</string>\n";
    plistFile << "                    <key>url</key>\n";
    plistFile << "                    <string></string>\n";
    plistFile << "                </dict>\n";
    plistFile << "                <dict>\n";
    plistFile << "                    <key>kind</key>\n";
    plistFile << "                    <string>full-size-image</string>\n";
    plistFile << "                    <key>url</key>\n";
    plistFile << "                    <string></string>\n";
    plistFile << "                </dict>\n";
    plistFile << "            </array>\n";
    plistFile << "            <key>metadata</key>\n";
    plistFile << "            <dict>\n";
    plistFile << "                <key>bundle-identifier</key>\n";
    plistFile << "                <string>" << "bundleIdentifier" << "</string>\n";
    plistFile << "                <key>kind</key>\n";
    plistFile << "                <string>software</string>\n";
    plistFile << "                <key>title</key>\n";
    plistFile << "                <string>App Title</string>\n";
    plistFile << "            </dict>\n";
    plistFile << "        </dict>\n";
    plistFile << "    </array>\n";
    plistFile << "</dict>\n";
    plistFile << "</plist>\n";
    plistFile.close();
    std::cout << "manifest.plist has been created successfully!" << std::endl;
    return ipaUrlPath;
}

void FileEnumerator::createQrCode(std::filesystem::path destinationFolder, std::string ipaDownloadPath, std::string ipaName) {
    // itms-services://?action=download-manifest&url=https://yourserver.com/path/to/manifest.plist
    std::string downloadUrl = "itms-services://?action=download-manifest&url=" + ipaDownloadPath;

    qrcodegen::QrCode qrCode = qrcodegen::QrCode::encodeText(downloadUrl.c_str(), qrcodegen::QrCode::Ecc::HIGH);
    int size = qrCode.getSize();
    std::vector<std::uint8_t> image(size * size, 255);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qrCode.getModule(x, y)) {
                image[y * size + x] = 0; // 黑色模块
            } else {
                image[y * size + x] = 255; // 白色背景
            }
        }
    }

    std::string qrCodeImageName = ipaName + std::string("_qrcode.png");
    fs::path qrCodePath = destinationFolder.append(qrCodeImageName);
    bool success = writePng(qrCodePath.c_str(), image, size, size);
    if (success) {
        std::cout << "二维码已保存为PNG文件: " << qrCodePath << " ~汪！" << std::endl;
    } else {
        std::cerr << "无法保存PNG文件" << std::endl;
    }
}

bool FileEnumerator::writePng(const char *filename, const std::vector<std::uint8_t> &image, int size, int stride) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return false;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    // 设置PNG图像参数
    png_set_IHDR(png_ptr, info_ptr, size, size, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // 写入PNG头信息
    png_write_info(png_ptr, info_ptr);

    // 写入图像数据行
    for (int y = 0; y < size; y++) {
        png_write_row(png_ptr, &image[y * stride]);
    }

    // 完成写入文件
    png_write_end(png_ptr, nullptr);

    // 清理并关闭文件
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}
