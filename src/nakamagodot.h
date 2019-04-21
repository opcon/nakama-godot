#ifndef NAKAMAGODOT_H
#define NAKAMAGODOT_H

#include <Godot.hpp>
//#include <Reference.hpp>
#include <Node.hpp>

#include "nakama-cpp/Nakama.h"
#include "NGodotLogSink.h"

using namespace Nakama;

namespace godot {
    class NakamaGodot : public Node {
        GODOT_CLASS(NakamaGodot, Node);

        float time_since_last_tick;

        protected:
            DefaultClientParameters parameters;
            NClientPtr _NClient;
            NSessionPtr _NSession;
            NRtClientPtr _NRtClient;
            NRtDefaultClientListener _NRtListener;

            std::function<void(NSessionPtr)> authenticated_success_callback = [this](NSessionPtr session) {
                authenticated(session); 
            };

            std::function<void(const NError&)> authenticated_error_callback = [this](const NError& error) {
                Godot::print(godot::String("Authentication unsuccesful. Check the logs!"));
            };

        private:
            void authenticated(NSessionPtr);

        public:
            static void _register_methods();

            NakamaGodot();
            ~NakamaGodot();

            void _init(); // our initializer called by Godot
            void _process(float delta);

            void create_client_default();
            void create_client(String server_key, String server_host, int port);
            void login_or_register();
            void authenticate_email(String email, String password, String username);
            void connect_realtime_client();

            void join_chat_room(String roomName);
            void write_chat_message(String channelId, String content);

            bool is_realtime_client_connected();
            bool is_session_expired();
    };
}

#endif
