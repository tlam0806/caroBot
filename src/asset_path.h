#pragma once
#include <filesystem>
#include <string>
#include <mach-o/dyld.h>

static std::filesystem::path executablePath()
{
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string tmp(size, '\0');
    _NSGetExecutablePath(tmp.data(), &size);
    return std::filesystem::weakly_canonical(std::filesystem::path(tmp));
}

static std::filesystem::path assetDir()
{
    auto exe = executablePath();
    auto exeDir = exe.parent_path();          // .../Contents/MacOS
    auto contents = exeDir.parent_path();     // .../Contents
    return contents / "Resources" / "assets";
}

static std::string getAssetPath(const std::string& file)
{
    return (assetDir() / file).string();
}
