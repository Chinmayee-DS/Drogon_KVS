#include "TestCtrl.h"
#include <drogon/HttpAppFramework.h>
#include <stdlib.h>
#include <mutex>

int gw = 0;
int pw = 0;

void api::v1::TestCtrl::getItem(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,
               std::string key) const
{
    // Json::Value ret;
    // ret["message"] = "Hello, World from get!";
    // auto resp = HttpResponse::newHttpJsonResponse(ret);
    // callback(resp);
    std::string value;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        auto it = kv_store.find(key);
        if(it != kv_store.end())
        {
            value = it->second;
        }
    }

    if(!value.empty())
    {
        gw++;
        std::cout << "gw " << gw << std::endl;

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setBody(value);
        callback(resp);
    }
    else
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k403Forbidden);
        resp->setBody("Key not found");
        callback(resp);
    }
}

void api::v1::TestCtrl::putItem(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,
               std::string key)
{
    // std::cout << "hitting the put " << std::endl;

    // std::cout << "  Token: " << key << std::endl;
    auto jo = req->getJsonObject();
    std::string val;
    if(jo)
    {
        val = (*jo)["value"].asString();
    }
    else
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Json unavailable");
        callback(resp);
    }
    // std::cout << "value " << val << std::endl;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        kv_store[key] = val;
    }

    pw++;
    std::cout << "gw " << pw << std::endl;

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setBody("Key stored successfully");
    callback(resp);

    // Json::Value ret;
    // ret["message"] = "Hello, World!";
    // auto resp = HttpResponse::newHttpJsonResponse(ret);
    // callback(resp);
}
