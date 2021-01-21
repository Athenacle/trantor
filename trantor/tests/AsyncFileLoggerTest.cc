#include <trantor/utils/Logger.h>
#include <trantor/utils/AsyncFileLogger.h>
#include <stdlib.h>
#include <thread>
int main()
{
    auto ptr = trantor::logger::LoggerManager::setLoggerImplement<
        trantor::AsyncFileLogger>("async_test");
    ptr->setFileSizeLimit(100000000);
    //    LOG_DEBUG<<"debug log!"<<1;
    //    LOG_TRACE<<"trace log!"<<2;
    //    LOG_INFO<<"info log!"<<3;
    //    LOG_WARN<<"warning log!"<<4;
    //    if(1)
    //        LOG_ERROR<<"error log!"<<5;
    //    std::thread thread_([](){
    //        LOG_FATAL<<"fatal log!"<<6;
    //    });
    //
    //    FILE *fp=fopen("/notexistfile","rb");
    //    if(fp==NULL)
    //    {
    //        LOG_SYSERR<<"syserr log!"<<7;
    //    }

    int i = 0;
    while (i < 1000000)
    {
        ++i;
        if (i % 100 == 0)
        {
            LOG_ERROR << "this is the " << i << "th log";
            continue;
        }
        LOG_INFO << "this is the " << i << "th log";
        ++i;
        LOG_DEBUG << "this is the " << i << "th log";
    }
}
