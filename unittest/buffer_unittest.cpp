//
// Created by 樱吹雪 on 2022/4/17.
//

#include <gtest/gtest.h>
#include "base/Buffer.h"
#include <string>

using namespace std;
using namespace base;

/*  */
TEST(testCase0, test0){
    FixedBuffer a;
    a.append("123",3);

    string s1 = a.readPtr();
    EXPECT_EQ(s1,"123");

    a.append("456",3);
    s1 = a.readPtr();
    EXPECT_EQ(s1,"123456");

    a.append("789",3);
    s1 = a.readPtr();
    EXPECT_EQ(s1,"123456789");

}

//空间腾挪测试
TEST(testCase1, test0) {
    FixedBuffer a;
    string s1(500, 'x');

    a.append(s1);
    EXPECT_EQ(a.used(), 501);
    a.append(s1);
    EXPECT_EQ(a.used(), 1002);

    a.readPtrMove(602);
    // 将const char*转化为char*
    char *p1 = const_cast<char *>(a.readPtr());
    char *p2 = const_cast<char *>(a.start());

    EXPECT_EQ(p1, p2 + 602);   // 测试指针位置
    EXPECT_EQ(a.used(), 400);

    a.append(s1);
    EXPECT_EQ(a.used(), 901);
    EXPECT_EQ(a.readPtr(), a.start());  // 测试指针位置
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv); // 初始化，所有测试都是这里启动的
    return RUN_ALL_TESTS(); // 运行所有测试用例
}
