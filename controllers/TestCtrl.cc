#include "TestCtrl.h"
#include <drogon/HttpAppFramework.h>
#include <stdlib.h>
#include <mutex>
#include <ctime>

int gw = 0;
int pw = 0;

void api::v1::TestCtrl::getItem(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,
               std::string key) const
{
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
        // gw++;
        // std::cout << "gw " << gw << std::endl;
        time_t timestamp;
        time(&timestamp);
        LOG_DEBUG << "GET " << key << " " << value << " " << ctime(&timestamp);

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setBody(value);
        callback(resp);
    }
    else
    {
        time_t timestamp;
        time(&timestamp);
        LOG_DEBUG << "GET " << key << " value_not_found " << ctime(&timestamp);

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
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
        time_t timestamp;
        time(&timestamp);
        LOG_DEBUG << "PUT " << key << " json_value_not_found " << ctime(&timestamp);

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

    // pw++;
    // std::cout << "pw " << pw << std::endl;

    time_t timestamp;
    time(&timestamp);
    LOG_DEBUG << "PUT " << key << " " << val << " " << ctime(&timestamp);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setBody("Key stored successfully");
    callback(resp);
}

void api::v1::TestCtrl::delItem(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback,
                std::string key)
{
    std::string value;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        auto it = kv_store.find(key);
        if(it != kv_store.end())
        {
            value = it->second;
            kv_store.erase(key);
        }
    }

    if(!value.empty())
    {
        // gw++;
        // std::cout << "gw " << gw << std::endl;

        time_t timestamp;
        time(&timestamp);
        LOG_DEBUG << "DEL " << key << " " << value << " " << ctime(&timestamp);

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setBody(value);
        callback(resp);
    }
    else
    {
        time_t timestamp;
        time(&timestamp);
        LOG_DEBUG << "DEL " << key << " key_value_pair_not_found " << ctime(&timestamp);

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Key not found");
        callback(resp);
    }
}
