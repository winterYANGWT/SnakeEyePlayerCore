#include "util.h"

std::string err2str(int errnum)
{
    char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
    return av_make_error_string(errbuf,
                                AV_ERROR_MAX_STRING_SIZE,
                                errnum);
}