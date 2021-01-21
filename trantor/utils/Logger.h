/**
 *
 *  @file Logger.h
 *  @author An Tao
 *
 *  Public header file in trantor lib.
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the License file.
 *
 *
 */

#pragma once

#include <trantor/utils/NonCopyable.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/LogStream.h>
#include <fmt/core.h>
#include <string.h>
#include <functional>
#include <iostream>
#include <memory>

namespace trantor
{
/**
 * @brief This class implements log functions.
 *
 */
class Logger : public NonCopyable
{
  public:
    enum LogLevel
    {
        kTrace = 0,
        kDebug,
        kInfo,
        kWarn,
        kError,
        kFatal,
        kNumberOfLogLevels
    };

    /**
     * @brief Calculate of basename of source files in compile time.
     *
     */
    class SourceFile
    {
      public:
        template <int N>
        inline SourceFile(const char (&arr)[N]) : data_(arr), size_(N - 1)
        {
            // std::cout<<data_<<std::endl;
            const char *slash = strrchr(data_, '/');  // builtin function
            if (slash)
            {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char *filename) : data_(filename)
        {
            const char *slash = strrchr(filename, '/');
            if (slash)
            {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

        const char *data_;
        int size_;
    };
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, bool isSysErr);
    Logger(SourceFile file, int line, LogLevel level, const char *func);
    ~Logger();
    LogStream &stream();

    /**
     * @brief Set the log level. Logs below the level are not printed.
     *
     * @param level
     */
    static void setLogLevel(LogLevel level)
    {
        logLevel_() = level;
    }

    /**
     * @brief Get the current log level.
     *
     * @return LogLevel
     */
    static LogLevel logLevel()
    {
        return logLevel_();
    }

  protected:
    static LogLevel &logLevel_()
    {
#ifdef RELEASE
        static LogLevel logLevel = LogLevel::kInfo;
#else
        static LogLevel logLevel = LogLevel::kTrace;
#endif
        return logLevel;
    }

    LogStream logStream_;

    SourceFile sourceFile_;
    int fileLine_;
    LogLevel level_;
    const char *func_{nullptr};
};

extern const char *logLevelStr[];
#ifdef NDEBUG
#define LOG_TRACE                                                          \
    if (0)                                                                 \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__) \
        .stream()
#else
#define LOG_TRACE                                                          \
    if (trantor::Logger::logLevel() <= trantor::Logger::kTrace)            \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__) \
        .stream()
#endif
#define LOG_DEBUG                                                          \
    if (trantor::Logger::logLevel() <= trantor::Logger::kDebug)            \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kDebug, __func__) \
        .stream()
#define LOG_INFO                                               \
    if (trantor::Logger::logLevel() <= trantor::Logger::kInfo) \
    trantor::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kWarn).stream()
#define LOG_ERROR \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kError).stream()
#define LOG_FATAL \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kFatal).stream()
#define LOG_SYSERR trantor::Logger(__FILE__, __LINE__, true).stream()

#define LOG_TRACE_IF(cond)                                                  \
    if ((trantor::Logger::logLevel() <= trantor::Logger::kTrace) && (cond)) \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__)  \
        .stream()
#define LOG_DEBUG_IF(cond)                                                \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
        .stream()
#define LOG_INFO_IF(cond)                                                \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN_IF(cond) \
    if (cond)             \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define LOG_ERROR_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define LOG_FATAL_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#ifdef NDEBUG
#define DLOG_TRACE                                                         \
    if (0)                                                                 \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG                                                       \
    if (0)                                                               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO \
    if (0)        \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
    if (0)        \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
    if (0)         \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
    if (0)         \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                \
    if (0)                                                                 \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG_IF(cond)                                              \
    if (0)                                                               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO_IF(cond) \
    if (0)                 \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
    if (0)                 \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
    if (0)                  \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
    if (0)                  \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#else
#define DLOG_TRACE                                                         \
    if (trantor::Logger::logLevel() <= trantor::Logger::kTrace)            \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG                                                       \
    if (Tensor::Logger::logLevel() <= Tensor::Logger::kDebug)            \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO                                            \
    if (Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                 \
    if ((trantor::Logger::logLevel() <= trantor::Logger::kTrace) && (cond)) \
    trantor::Logger(__FILE__, __LINE__, trantor::Logger::kTrace, __func__)  \
        .stream()
#define DLOG_DEBUG_IF(cond)                                               \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
        .stream()
#define DLOG_INFO_IF(cond)                                               \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
    if (cond)               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
    if (cond)               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#endif

const char *strerror_tl(int savedErrno);

namespace logger
{
class AbstractLogger
{
  protected:
    using level = trantor::Logger::LogLevel;

  public:
    virtual void setup()
    {
    }
    virtual void output(level, const char *, size_t) = 0;
    virtual void flush() = 0;
    virtual ~AbstractLogger() = default;
};

class MarkLogger : public trantor::logger::AbstractLogger
{
  public:
    virtual void print(const char *msg, size_t length) = 0;

    virtual void output(level level, const char *msg, size_t length) override;
    virtual ~MarkLogger()
    {
    }
};

class LoggerManager : public NonCopyable
{
    using level = trantor::Logger::LogLevel;

    static LoggerManager manager_;

    static level level_;

    bool newline_{true};

    std::shared_ptr<trantor::logger::AbstractLogger> implement_;

  public:
    template <class L, class... Args>
    static std::shared_ptr<L> setLoggerImplement(Args... args)
    {
        static_assert(
            std::is_base_of<trantor::logger::AbstractLogger, L>::value,
            "setLoggerImplement must be called with class based on "
            "trantor::logger::AbstractLogger");

        auto ptr = std::make_shared<L>(std::forward<Args>(args)...);
        manager_.implement_ = ptr;
        ptr->setup();
        return ptr;
    }

    static void setLevel(level l)
    {
        level_ = l;
    }
    static void output(level, const char *, size_t);
    static void output(level,
                       const char *,
                       size_t,
                       const char * = nullptr,
                       int = -1,
                       const char * = nullptr);

    static level getLevel()
    {
        return level_;
    }

    static void flush()
    {
        manager_.implement_->flush();
    }
};

#ifndef TRANTOR_INLINE
#if defined(__GNUC__) || defined(__clang__)
#define TRANTOR_INLINE inline __attribute__((always_inline))
#else
#define TRANTOR_INLINE inline
#endif
#endif

#define BUILD_LOGGER_FUNC(level, name)                                         \
    template <typename S, typename... Args>                                    \
    TRANTOR_INLINE void name(const S &format_str, Args &&...args)              \
    {                                                                          \
        const auto level = ::trantor::Logger::LogLevel::level;                 \
        if (LoggerManager::getLevel() <= level)                                \
        {                                                                      \
            LoggerManager::output(                                             \
                level,                                                         \
                fmt::format(format_str, std::forward<Args>(args)...).c_str()); \
        }                                                                      \
    }                                                                          \
    template <int line, typename S, typename... Args>                          \
    TRANTOR_INLINE void name(const char *filename,                             \
                             const S &format_str,                              \
                             Args &&...args)                                   \
    {                                                                          \
        const auto level = ::trantor::Logger::LogLevel::level;                 \
        if (LoggerManager::getLevel() <= level)                                \
        {                                                                      \
            LoggerManager::output(                                             \
                level,                                                         \
                fmt::format(format_str, std::forward<Args>(args)...).c_str(),  \
                filename,                                                      \
                line);                                                         \
        }                                                                      \
    }

BUILD_LOGGER_FUNC(kTrace, trace)
BUILD_LOGGER_FUNC(kDebug, debug)
BUILD_LOGGER_FUNC(kInfo, info)
BUILD_LOGGER_FUNC(kWarn, warn)
BUILD_LOGGER_FUNC(kError, error)
BUILD_LOGGER_FUNC(kFatal, fatal)

#undef BUILD_LOGGER_FUNC

#define TRANTOR_TRACE(...) \
    (::trantor::logger::trace<__LINE__>(__FILE__, __VA_ARGS__))
#define TRANTOR_DEBUG(...) \
    (::trantor::logger::debug<__LINE__>(__FILE__, __VA_ARGS__))
#define TRANTOR_INFO(...) \
    (::trantor::logger::info<__LINE__>(__FILE__, __VA_ARGS__))
#define TRANTOR_WARN(...) \
    (::trantor::logger::warn<__LINE__>(__FILE__, __VA_ARGS__))
#define TRANTOR_ERROR(...) \
    (::trantor::logger::error<__LINE__>(__FILE__, __VA_ARGS__))
#define TRANTOR_FATAL(...) \
    (::trantor::logger::fatal<__LINE__>(__FILE__, __VA_ARGS__))

}  // namespace logger

}  // namespace trantor
