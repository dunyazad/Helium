#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

    const char* logl_root = "C:/Users/Mickey/Desktop/Reference/LearnOpenGL";

    class FileSystem
    {
    private:
        typedef std::string(*Builder) (const std::string& path);

    public:
        static std::string getPath(const std::string& path);

    private:
        static std::string const& getRoot();
        static Builder getPathBuilder();
        static std::string getPathRelativeRoot(const std::string& path);
        static std::string getPathRelativeBinary(const std::string& path);
    };

}
