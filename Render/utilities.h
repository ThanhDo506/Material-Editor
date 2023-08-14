#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cerrno>
namespace utilities
{
    // Ref: Victor Gordan
    // 12th - July - 2023
    // https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%204%20-%20Organizing/shaderClass.cpp
    inline std::string readGLSL(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (file)
        {
            std::string contents;
            file.seekg(0, std::ios::end);
            contents.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&contents[0], contents.size());
            file.close();
            return (contents);
        }
        throw(errno);
    }
};
