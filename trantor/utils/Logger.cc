/**
 *
 *  Logger.cc
 *  An Tao
 *
 *  Public header file in trantor lib.
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the License file.
 *
 *
 */

#include <trantor/utils/Logger.h>
#include <stdio.h>
#include <thread>
#ifndef _WIN32
#include <unistd.h>
#include <sys/syscall.h>
#else
#include <sstream>
#endif
#ifdef __FreeBSD__
#include <pthread_np.h>
#endif

namespace trantor
{
// helper class for known string length at compile time
class T
{
  public:
    T(const char *str, unsigned len) : str_(str), len_(len)
    {
        assert(strlen(str) == len_);
    }

    const char *str_;
    const unsigned len_;
};

const char *strerror_tl(int savedErrno)
{
#ifndef _MSC_VER
    return strerror(savedErrno);
#else
    static thread_local char errMsg[64];
    (void)strerror_s<sizeof errMsg>(errMsg, savedErrno);
    return errMsg;
#endif
}

inline LogStream &operator<<(LogStream &s, T v)
{
    s.append(v.str_, v.len_);
    return s;
}

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v)
{
    s.append(v.data_, v.size_);
    return s;
}
}  // namespace trantor
using namespace trantor;

static thread_local uint64_t lastSecond_{0};
static thread_local char lastTimeString_[32] = {0};

const char *trantor::logLevelStr[Logger::LogLevel::kNumberOfLogLevels] = {
    " TRACE ",
    " DEBUG ",
    " INFO  ",
    " WARN  ",
    " ERROR ",
    " FATAL ",
};
Logger::Logger(SourceFile file, int line)
    : sourceFile_(file), fileLine_(line), level_(kInfo)
{
    // formatTime();
    // logStream_ << T(logLevelStr[level_], 7);
}
Logger::Logger(SourceFile file, int line, LogLevel level)
    : sourceFile_(file), fileLine_(line), level_(level)
{
    // formatTime();
    // logStream_ << T(logLevelStr[level_], 7);
}
Logger::Logger(SourceFile file, int line, LogLevel level, const char *func)
    : sourceFile_(file), fileLine_(line), level_(level), func_(func)
{
    // formatTime();
    //   logStream_ << T(logLevelStr[level_], 7) << "[" << func << "] ";
}
Logger::Logger(SourceFile file, int line, bool)
    : sourceFile_(file), fileLine_(line), level_(kFatal)
{
    // formatTime();
    //   logStream_ << T(logLevelStr[level_], 7);
    if (errno != 0)
    {
        logStream_ << strerror_tl(errno) << " (errno=" << errno << ") ";
    }
}
Logger::~Logger()
{
    trantor::logger::LoggerManager::output(level_,
                                           logStream_.bufferData(),
                                           logStream_.bufferLength(),
                                           sourceFile_.data_,
                                           fileLine_,
                                           func_);
    if (level_ >= kError)
        trantor::logger::LoggerManager::flush();
    // logStream_.resetBuffer();
}
LogStream &Logger::stream()
{
    return logStream_;
}

thread_local uint64_t tid;

namespace
{
void setTid()
{
#ifdef __linux__
    if (tid == 0)
        tid = static_cast<pid_t>(::syscall(SYS_gettid));
#elif defined __FreeBSD__
    if (tid == 0)
    {
        tid = pthread_getthreadid_np();
    }
#elif defined __OpenBSD__
    if (tid == 0)
    {
        tid = getthrid();
    }
#elif defined _WIN32
    if (tid == 0)
    {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        tid = std::stoull(ss.str());
    }
#else
    if (tid == 0)
    {
        pthread_threadid_np(NULL, &tid);
    }
#endif
}
}  // namespace

void trantor::logger::MarkLogger::output(level level,
                                         const char *msg,
                                         size_t length)
{
    if (tid == 0)
    {
        setTid();
    }
    auto message = fmt::format("{} [{}] {} - {}",
                               trantor::Date::now(),
                               trantor::logLevelStr[level],
                               tid,
                               msg);
    print(message.c_str(), message.length());
}