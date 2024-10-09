#pragma once

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpController.h>
#include <unordered_map>
#include "string.h"
#include <mutex>

using namespace drogon;

namespace api
{
namespace v1
{
class TestCtrl : public drogon::HttpController<TestCtrl>
{
  public:

    mutable std::mutex mtx;
    mutable std::unordered_map<std::string, std::string> kv_store;
    // std::mutex lock;
    // void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    METHOD_LIST_BEGIN
    // list path definitions here;
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    // ADD_METHOD_TO(cone::dumFun, "/users", Post);
    // ADD_METHOD_VIA_REGEX(TestCtrl::getItem, "/{key}", Get);
        ADD_METHOD_TO(TestCtrl::getItem,"/{key}",Get);
        ADD_METHOD_TO(TestCtrl::putItem, "/{key}", Post);
        // METHOD_ADD(TestCtrl::getItem, "/{k}", Get);
        // METHOD_ADD(TestCtrl::putItem, "/{k}", Post);
    METHOD_LIST_END

    void getItem(const HttpRequestPtr &req,
                   std::function<void (const HttpResponsePtr &)> &&callback,
                   std::string key) const;

    void putItem(const HttpRequestPtr &req,
                   std::function<void (const HttpResponsePtr &)> &&callback,
                   std::string key);
};
}
}
