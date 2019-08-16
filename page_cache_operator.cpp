#include "page_cache_operator.h"

using namespace std;

const uint32_t PageCacheOperator::PAGE_SIZE = sysconf(_SC_PAGESIZE);

PageCacheOperator::PageCacheOperator():m_fd(0), m_len(0){}
int PageCacheOperator::clear_file_cache(const char *filename)
{
    struct stat st;
    if(stat(filename , &st) < 0) {
        fprintf(stderr , "stat localfile failed, path:%s\n",filename);
        return -1;
    }

    int fd = open(filename, O_RDONLY);
    if( fd < 0 ) {
        fprintf(stderr , "open localfile failed, path:%s\n",filename);
        return -1;
    }

    //clear cache by posix_fadvise

    if( posix_fadvise(fd,0,st.st_size,POSIX_FADV_DONTNEED) != 0) {
        printf("Cache FADV_DONTNEED failed\n");
    }
    else {
        printf("Cache FADV_DONTNEED done\n");
    }

    return 0;
}

void PageCacheOperator::GetPageInfo(const char *pFieldName)
{
    if (!Init(pFieldName))
    {
        Clear();
        return;
    }

    unsigned char *pVec = new unsigned char[m_pageCount];

    if (-1 == mincore(m_pDat, m_len, pVec)) {
        Clear();
        return ;
    }

    uint32_t hitCnt = 0;
    uint32_t i = 0;
    for (; i < m_pageCount; ++i) {
        if (pVec[i] == 1)
        {
            hitCnt += 1;
        }
    }
    delete[] pVec;
    struct rusage tmpRusage;

    getrusage(RUSAGE_SELF, &tmpRusage);

    cout << hitCnt << ", " << m_pageCount << ", " << tmpRusage.ru_majflt << endl;
    Clear();
}

void PageCacheOperator::PageMap(uint64_t virIndex, uint64_t offset)
{
    int fd = open("/proc/self/pagemap", O_RDONLY);

    lseek(fd, virIndex * 8, SEEK_SET);
    cout << "offset : " << virIndex * 8 << endl;
    uint64_t pageInfo;
    read(fd, &pageInfo, sizeof(uint64_t));
    if (pageInfo & 1LL << 63)
    {
        cout << "find it in physical memory ." << endl;
    }
    else
    {
        cout << "not in physical memory ." << endl;
    }
    close(fd);
}

void PageCacheOperator::LoadDataToPageCache(const char *fileName)
{
    if (!Init(fileName))
    {
        Clear();
        return ;
    }
    int sum = 0; 
    for (uint32_t i = 0; i < m_len; ++i)
    {
        sum += *(m_pDat + i);
    }
    cout << sum << endl;   
    Clear();
}

void PageCacheOperator::PrintPageState(const char *fileName, const uint64_t pageIdx)
{
    if (!Init(fileName))
    {
        Clear();
        return ;
    }
    char *pFindPage = m_pDat + pageIdx * 4096;
    PageMap((uint64_t)pFindPage / PAGE_SIZE, 0);
    Clear();
}

bool PageCacheOperator::Init(const char *pFileName)
{
    m_fd = open(pFileName, O_RDWR);
    if (m_fd == -1)
    {
        cout << "open err ..." << endl;
        return false;
    }
    struct stat stat;
    m_len = lseek(m_fd, 0, SEEK_END);

    m_pageCount = (m_len + PAGE_SIZE - 1) / PAGE_SIZE;
    m_pDat = (char *)mmap(NULL, m_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, m_fd, 0);
}

void PageCacheOperator::Clear()
{
    close(m_fd);
    m_len = 0;
    m_pageCount = 0;
    m_pDat = nullptr;
}
