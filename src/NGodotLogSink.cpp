#include "NGodotLogSink.h"

namespace Nakama {
    using namespace std;

    void NGodotLogSink::log(NLogLevel level, const std::string& message, const char* func) {
        godot::String tmp = "";

        if (func && func[0]) {
            tmp += "[";
            tmp += func;
            tmp += "]";
        }

        tmp = tmp + message.c_str();

        godot::Godot::print(tmp);
    }
    
    void NGodotLogSink::flush() {
    }
}
