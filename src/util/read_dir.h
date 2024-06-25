#ifndef READ_DIR_H
#define READ_DIR_H

extern "C"
{
#include <libavutil/log.h>
#include <libavutil/error.h>
}
#include <string>
#include <vector>
#include <dirent.h>

int read_dir(std::string &dir_path,
             std::vector<std::string> &files);

#endif // READ_DIR_H
