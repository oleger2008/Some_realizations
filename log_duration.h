#pragma once

#include <chrono>
#include <iostream>

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION_STREAM(x, y) LogDuration UNIQUE_VAR_NAME_PROFILE(x, y)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

template<typename IdType>
class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration(const IdType& id)
    : id_(static_cast<std::string>(id))
    {
    }

    LogDuration(const IdType& id, std::ostream& output)
    : id_(static_cast<std::string>(id))
    , output_(output)
    {
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        output_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const std::string id_;
    std::ostream& output_ = std::cerr;
    const Clock::time_point start_time_ = Clock::now();
};
