#include <string>
#include <filesystem>
#include <cstring>

std::string getCurrentPath() {
    return std::filesystem::current_path().string();
}

std::string getParentDirectory(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.parent_path().string();
}

std::string getFileName(const std::string& name) {
    std::string currentPath = getCurrentPath();
    std::string filePath = getParentDirectory(currentPath) + "\\" + name;
    return filePath;
}

char* getCharFileName(const std::string& name) {
    std::string currentPath = getCurrentPath();
    std::string filePath = getParentDirectory(currentPath) + "\\" + name;
    char* charFileName = new char[filePath.length() + 1];
    strcpy(charFileName, filePath.c_str());
    return charFileName;
}
