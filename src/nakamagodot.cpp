#include "nakamagodot.h"

using namespace godot;

void NakamaGodot::_register_methods() {
    register_method("_process", &NakamaGodot::_process);
    register_method("create_client", &NakamaGodot::create_client);
    register_method("login_or_register", &NakamaGodot::login_or_register);
}

void NakamaGodot::_init() {
    NLogger::init(std::make_shared<NGodotLogSink>(), NLogLevel::Debug);
}

void NakamaGodot::_process(float delta) {
    if (!_NClient) return;
    time_since_last_tick += delta;
    if (time_since_last_tick > 0.05) {
        _NClient->tick();
        time_since_last_tick = 0;
    }
}

NakamaGodot::NakamaGodot() {
}

NakamaGodot::~NakamaGodot() {
    Godot::print("Destructing");
}

void NakamaGodot::create_client() {
    parameters.serverKey = "defaultkey";
    parameters.host = "127.0.0.1";
    parameters.port = 7349;
    _NClient = createDefaultClient(parameters);

    Godot::print("Hello from Nakama!");
}

void NakamaGodot::login_or_register() {
    auto successCallback = [](NSessionPtr session) {
        Godot::print(godot::String("Authenticated succesfully. User ID: ") + session->getUserId().c_str());
    };
    auto errorCallback = [](const NError& error) {
        Godot::print(godot::String("Authentication unsuccesful. Check the logs!"));
    };

    if (!_NClient) return;

    Godot::print("Connecting");

    std::string email = "test-email@test.com";
    std::string password = "testtesttest";
    std::string username = "test-user";
    bool create = true;

    _NClient->authenticateEmail(email, password, username, create, successCallback, errorCallback);
}

