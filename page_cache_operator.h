#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <bitset>
#include <errno.h>

using namespace std;
class PageCacheOperator
{
public:
    PageCacheOperator();

    int clear_file_cache(const char *pFileName);

    void GetPageInfo(const char *pFileName);

    void PageMap(uint64_t virIndex, uint64_t offset);

    void LoadDataToPageCache(const char *pFileName);

    void PrintPageState(const char *pFileName, const uint64_t pageIdx);

    bool Init(const char *pFileName);
    
    void Clear();

private:
    int m_fd;
    off_t m_len;
    char *m_pDat;
    uint32_t m_pageCount;
    static const uint32_t PAGE_SIZE;
};

