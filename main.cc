#include <drogon/drogon.h>
int main() {
    //Set HTTP listener address and port
    drogon::app().setLogPath("./").setLogLevel(trantor::Logger::kDebug);
    // drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().addListener("0.0.0.0", 8080);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
