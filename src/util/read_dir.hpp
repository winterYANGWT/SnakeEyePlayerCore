#ifndef SNAKE_EYE_READ_DIR_HPP
#define SNAKE_EYE_READ_DIR_HPP

extern "C"
{
#include <libavutil/log.h>
#include <libavutil/error.h>
}
#include <string>
#include <vector>
#include <dirent.h>

namespace SnakeEye
{
    int read_dir(std::string &dir_path,
                 std::vector<std::string> &files);
}

#endif // SNAKE_EYE_READ_DIR_HPP
