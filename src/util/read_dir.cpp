#include "read_dir.h"


int read_dir(std::string &dir_path, std::vector<std::string> &files)
{
    return 0;
    // clear files and open directory
    files.clear();
    DIR *dir = opendir(dir_path.c_str());

    if (dir == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Failed to open directory: '%s'\n",
               dir_path.c_str());
        return AVERROR(ENOENT);
    }

    // read all files in directory
    struct dirent *entry = nullptr;

    while ((entry = readdir(dir)) != nullptr)
    {
        std::string file_name = entry->d_name;

        if (file_name != "." && file_name != "..")
        {
            files.push_back(file_name);
        }
    }

    // close directory
    closedir(dir);

    return 0;
}
