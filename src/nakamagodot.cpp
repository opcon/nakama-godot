#include "nakamagodot.h"

using namespace godot;

void NakamaGodot::_register_methods() {
    register_method("_process", &NakamaGodot::_process);
    register_method("create_client_default", &NakamaGodot::create_client_default);
    register_method("create_client", &NakamaGodot::create_client);
    register_method("authenticate_email", &NakamaGodot::authenticate_email);
    register_method("connect_realtime_client", &NakamaGodot::connect_realtime_client);
    register_method("join_chat_room", &NakamaGodot::join_chat_room);
    register_method("write_chat_message", &NakamaGodot::write_chat_message);

    register_method("is_realtime_client_connected", &NakamaGodot::is_realtime_client_connected);
    register_method("is_session_expired", &NakamaGodot::is_session_expired);

    register_signal<NakamaGodot>("authenticated");
    register_signal<NakamaGodot>("chat_message_recieved", "channel_id", GODOT_VARIANT_TYPE_STRING, "message_id", GODOT_VARIANT_TYPE_STRING, "message_code", GODOT_VARIANT_TYPE_INT, "sender_id", GODOT_VARIANT_TYPE_STRING, "username", GODOT_VARIANT_TYPE_STRING, "content", GODOT_VARIANT_TYPE_STRING);
}

void NakamaGodot::_init() {
    NLogger::init(std::make_shared<NGodotLogSink>(), NLogLevel::Debug);
}

void NakamaGodot::_process(float delta) {
    if (!_NClient) return;
    time_since_last_tick += delta;

    if (time_since_last_tick > 0.05) {
        _NClient->tick();
        if (_NRtClient)
            _NRtClient->tick();
        time_since_last_tick = 0;
    }
}

NakamaGodot::NakamaGodot() {
}

NakamaGodot::~NakamaGodot() {
    Godot::print("Destructing");
}

void NakamaGodot::create_client_default() {
    create_client("defaultkey", "127.0.0.1", 7349);
}

void NakamaGodot::create_client(String server_key, String server_host, int port) {
    parameters.serverKey = server_key.utf8().get_data();
    parameters.host = server_host.utf8().get_data();
    parameters.port = port;
    _NClient = createDefaultClient(parameters);
}

void NakamaGodot::authenticate_email(String email, String password, String username) {

    if (!_NClient) return;

    bool create = true;

    _NClient->authenticateEmail(std::string(email.utf8().get_data()), std::string(password.utf8().get_data()), username.utf8().get_data(), create, authenticated_success_callback, authenticated_error_callback);
}

void NakamaGodot::authenticated(NSessionPtr session) {
    _NSession = session;
    Godot::print(godot::String("Session token: ") + session->getAuthToken().c_str());
    Godot::print(godot::String("Authenticated succesfully. User ID: ") + session->getUserId().c_str());
    emit_signal("authenticated");
}

void NakamaGodot::connect_realtime_client() {
    if (!_NSession) return;
    if (_NSession->isExpired()) return;
    if (_NRtClient) return;

    int port = 7350;
    bool createStatus = true;
    _NRtClient = _NClient->createRtClient(port);

    _NRtListener.setConnectCallback([]() {
                Godot::print("Realtime client connected");
            });
    _NRtClient->setListener(&_NRtListener);
    _NRtClient->connect(_NSession, createStatus);
}

void NakamaGodot::join_chat_room(String roomName) {
    _NRtListener.setChannelMessageCallback([this](const NChannelMessage& message) {
                Godot::print("Recieved message on channel {0}", message.channelId.c_str());
                Godot::print("Message content: {0}", message.content.c_str());

                emit_signal("chat_message_recieved", message.channelId.c_str(), message.messageId.c_str(), message.code, message.senderId.c_str(), message.username.c_str(), message.content.c_str());
            });

    auto sucessJoinCallback = [this](NChannelPtr channel) {
        Godot::print("Joined chat: {0}", channel->id.c_str());
        std::string content = "{\"message\":\"Hello world\"}";

        _NRtClient->writeChatMessage(channel->id, content);
    };

    auto errorJoinCallback = [this](const NRtError& error) {
        Godot::print("Joining chat room unsuccesful, check logs");
    };

    _NRtClient->joinChat(
            roomName.utf8().get_data(),
            NChannelType::ROOM,
            {},
            {},
            sucessJoinCallback,
            errorJoinCallback);
}

void NakamaGodot::write_chat_message(String channelId, String content) {
    _NRtClient->writeChatMessage(channelId.utf8().get_data(), content.utf8().get_data());
}

bool NakamaGodot::is_realtime_client_connected() {
    return (_NRtClient && _NRtClient->isConnected());
}

bool NakamaGodot::is_session_expired() {
    return !(_NSession && !_NSession->isExpired());
}
