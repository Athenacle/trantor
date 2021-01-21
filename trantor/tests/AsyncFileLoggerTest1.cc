#include <trantor/utils/Logger.h>
#include <trantor/utils/AsyncFileLogger.h>
#include <thread>
#include <chrono>
#include <stdlib.h>

using namespace std::chrono_literals;
int main()
{
    trantor::logger::LoggerManager::setLoggerImplement<
        trantor::AsyncFileLogger>("async_test")
        ->setFileSizeLimit(100000000);
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
        std::this_thread::sleep_for(1s);
    }
}
