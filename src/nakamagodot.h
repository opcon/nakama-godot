#ifndef NAKAMAGODOT_H
#define NAKAMAGODOT_H

#include <Godot.hpp>
#include <Reference.hpp>

#include "nakama-cpp/Nakama.h"
#include "NGodotLogSink.h"

using namespace Nakama;

namespace godot {
    class NakamaGodot : public Reference {
        GODOT_CLASS(NakamaGodot, Reference);

        float time_since_last_tick;

        // std:shared_ptr<Nakama::NClient> _NClient;
        DefaultClientParameters parameters;
        NClientPtr _NClient;
            
        public:
            static void _register_methods();

            NakamaGodot();
            ~NakamaGodot();

            void _init(); // our initializer called by Godot
            void _process(float delta);

            void create_client();
            void login_or_register();
    };
}

#endif
