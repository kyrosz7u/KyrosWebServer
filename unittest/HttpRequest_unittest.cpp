//
// Created by 樱吹雪 on 2022/4/22.
//

#include "gtest/gtest.h"

#include "net/HttpRequest.h"
#include "base/Logger.h"
#include "base/Buffer.h"

using namespace std;
using namespace base;
using namespace net::http;

char httprequest0[]="GET /index.html HTTP/1.1\r\n"
                    "Host: 127.0.0.1\r\n"
                    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                    "Upgrade-Insecure-Requests: 1\r\n"
                    "Cookie: _ga=GA1.1.1715912313.1650113672; localauth=localapif7461590909de300:; isNotIncognito=true\r\n"
                    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.4 Safari/605.1.15\r\n"
                    "Accept-Language: zh-CN,zh-Hans;q=0.9\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Connection: keep-isAlive\r\n\r\n";


char httprequest1[]="GET /index.html HTTP/1.";

char httprequest2[] = "1\r\n";

char httprequest3[]= "Host: 127.0.0.1";

char httprequest4[]="\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Connection: keep-isAlive\r\n";

char httprequest5[]="\r\n";


/*模拟接收不连续的http报文数据包*/
TEST(HttpTest, RequestWithoutBody){
    FixedBuffer buf;
    HttpRequest req;

    buf.append(httprequest1,sizeof httprequest1-1);
    auto ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(), HttpRequest::LINE_OPEN);
    EXPECT_EQ(req.getCheckState(), HttpRequest::CHECK_STATE_REQUEST);

    buf.append(httprequest2,sizeof httprequest2-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(), HttpRequest::LINE_OK);
    EXPECT_EQ(req.getCheckState(), HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest3,sizeof httprequest3-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(), HttpRequest::LINE_OPEN);
    EXPECT_EQ(req.getCheckState(), HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest4,sizeof httprequest4-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(), HttpRequest::LINE_OK);
    EXPECT_EQ(req.getCheckState(), HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest5,sizeof httprequest5-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,GET_REQUEST);
    EXPECT_EQ(req.method,GET);
    EXPECT_EQ(req.url,"/index.html");
    EXPECT_EQ(req.version,"HTTP/1.1");
    EXPECT_EQ(req.host,"127.0.0.1");
    EXPECT_EQ(req.contentLength,0);
    EXPECT_EQ(req.isAlive, true);

}

char httprequest11[]="POST /index.html HTTP/1.";

char httprequest12[] = "1\r\n";

char httprequest13[]= "Host: 127.0.0.1";

char httprequest14[]="\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Content-length: 17\r\n"
                    "Connection: keep-isAlive\r\n";

char httprequest15[]="\r\n";

char httprequest16[]="Hello Http World";

/*模拟接收不连续的http报文数据包*/
TEST(HttpTest, RequestWithBody){
    FixedBuffer buf;
    HttpRequest req;

    buf.append(httprequest11,sizeof httprequest11-1);
    auto ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(),HttpRequest::LINE_OPEN);
    EXPECT_EQ(req.getCheckState(),HttpRequest::CHECK_STATE_REQUEST);

    buf.append(httprequest12,sizeof httprequest12-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(),HttpRequest::LINE_OK);
    EXPECT_EQ(req.getCheckState(),HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest13,sizeof httprequest13-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(),HttpRequest::LINE_OPEN);
    EXPECT_EQ(req.getCheckState(),HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest14,sizeof httprequest14-1);
    ret = req.processRead(buf);
    EXPECT_EQ(ret,NO_REQUEST);
    EXPECT_EQ(req.getLineState(),HttpRequest::LINE_OK);
    EXPECT_EQ(req.getCheckState(),HttpRequest::CHECK_STATE_HEADER);

    buf.append(httprequest15,sizeof httprequest15-1);
    ret = req.processRead(buf);
    buf.append(httprequest16,sizeof httprequest16);
    ret = req.processRead(buf);

    EXPECT_EQ(ret,::GET_REQUEST);
    EXPECT_EQ(req.method,POST);
    EXPECT_EQ(req.url,"/index.html");
    EXPECT_EQ(req.version,"HTTP/1.1");
    EXPECT_EQ(req.host,"127.0.0.1");
    EXPECT_EQ(req.contentLength,17);
    EXPECT_EQ(req.isAlive, true);
    EXPECT_EQ(req.body, "Hello Http World");

    LOG_INFO << req.body;
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv); // 初始化，所有测试都是这里启动的
    return RUN_ALL_TESTS(); // 运行所有测试用例
}

