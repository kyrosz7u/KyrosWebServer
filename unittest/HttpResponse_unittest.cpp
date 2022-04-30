//
// Created by 樱吹雪 on 2022/4/22.
//

#include "gtest/gtest.h"

#include "net/HttpResponse.h"
#include "base/Logger.h"
#include "base/Buffer.h"

using namespace std;
using namespace base;
using namespace net::http;

char httpResponse0[]="HTTP/1.1 200 OK\r\n"
                    "Connection: keep-isAlive\r\n"
                    "Content-Length: 10\r\n"
                    "Accept-Ranges: bytes\r\n"
                    "Cache-Control: no-cache\r\n"
                    "Content-Type: text/html\r\n"
                    "Date: Mon, 25 Apr 2022 08:35:51 GMT\r\n\r\n"
                    "1234567890";

/*模拟构建http响应*/
TEST(HttpTest, RequestWithBody){
    FixedBuffer buf;
    HttpResponse resp;
    resp.setStatusCode(HttpResponse::SUCCESS_OK);
    resp.setHeader("Accept-Ranges","bytes");
    resp.setHeader("Cache-Control","no-cache");
    resp.isAlive= true;
    resp.setContentLength(10);
    resp.setContentType("text/html");
    resp.setHeader("Date","Mon, 25 Apr 2022 08:35:51 GMT");
    resp.setBody("1234567890");
    resp.appendToBuffer(buf);

    string str1;
    string str2;
    str1.assign(buf.readPtr(), buf.used());
    str2 = httpResponse0;
    EXPECT_EQ(str1, str2);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv); // 初始化，所有测试都是这里启动的
    return RUN_ALL_TESTS(); // 运行所有测试用例
}

