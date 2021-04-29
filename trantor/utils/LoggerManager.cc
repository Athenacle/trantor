#include <trantor/utils/Logger.h>
#include <trantor/utils/Date.h>
#include <mutex>

using namespace trantor::logger;

LoggerManager LoggerManager::manager_;

LoggerManager::level LoggerManager::level_ =
#ifndef NDEBUG
    ::trantor::Logger::LogLevel::kTrace;
#else
    ::trantor::Logger::LogLevel::kInfo;
#endif

namespace
{
class SimpleStdoutLogger : public MarkLogger
{
    std::mutex m;

  public:
    virtual void print(const char *msg, size_t length) override
    {
        std::lock_guard<std::mutex> l(m);
        fwrite(msg, 1, length, stdout);
    }
    virtual void flush() override
    {
        fflush(stdout);
    }
    virtual ~SimpleStdoutLogger()
    {
    }
};

struct DefaultLogManagerInstaller
{
    DefaultLogManagerInstaller()
    {
        LoggerManager::setLoggerImplement<SimpleStdoutLogger>();
    }
};

static DefaultLogManagerInstaller installer;
}  // namespace

void LoggerManager::output(level l,
                           const std::string &msg,
                           const char *file,
                           int line,
                           const char *func)
{
    if (manager_.implement_ != nullptr)
    {
        std::string funcName, fileLine;

        if (file != nullptr && line != -1)
        {
            auto end = file;
            fileLine = fmt::format("({}:{})", end, line);
        }
        if (func != nullptr)
        {
            funcName = fmt::format("[{}]: ", func);
        }
        auto result = fmt::format(manager_.implement_->newLine() ? "{}{} {}\n"
                                                                 : "{}{} {}",
                                  funcName,
                                  msg,
                                  fileLine);
        manager_.implement_->output(l, result);
    }
}