#ifndef NGODOTLOGSINK_H
#define NGODOTLOGSINK_H

#include <Godot.hpp>

#include "nakama-cpp/log/NLogSinkInterface.h"

namespace Nakama {
    class NAKAMA_API NGodotLogSink : public NLogSinkInterface {
        public:
            ~NGodotLogSink() { }
            void log(NLogLevel level, const std::string& message, const char* func) override;

            void flush() override;
    };
}

#endif
