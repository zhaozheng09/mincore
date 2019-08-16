#include <iostream>
#include "page_cache_operator.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("usage : \n"
                   "1.clear page cache : ./main clear files;\n"
                   "2.load file to page cache : ./main all file;\n"
                   "3. x page in file is in page cache or not .\n");
    }

    PageCacheOperator pcOperator;
    if (argc >= 3 && strcmp(argv[1], "clear") == 0)
    {
        for (uint32_t i = 2; i < argc; ++i)
        {
            pcOperator.clear_file_cache(argv[i]);
        }
        return -1;
    }
    
    if (argc >= 3 && strcmp(argv[1], "all") == 0)
    {
        for (uint32_t i = 2; i < argc; ++i)
        {
            pcOperator.LoadDataToPageCache(argv[i]);
        }
    }
    else if (argc == 3)
    {
        pcOperator.PrintPageState(argv[2], atoi(argv[1]));
    }
    pcOperator.GetPageInfo(argv[2]);
    return 0;
}
