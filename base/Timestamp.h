//
// Created by 樱吹雪 on 2022/2/3.
//

#ifndef KYROSWEBSERVER_TIMESTAMP_H
#define KYROSWEBSERVER_TIMESTAMP_H
#include "Copyable.h"
#include <string>

using namespace std;

namespace base {
class Timestamp {
public:
    Timestamp(int64_t microSecondsSinceEpochArg)
            : microSecondsSinceEpoch_(microSecondsSinceEpochArg) {
    }
    uint64_t getMicroSecondsSinceEpoch(){return microSecondsSinceEpoch_;}

    string toString() const;

    string toFormattedString(bool showMicroseconds) const;

    static Timestamp now();
    inline static double timeDifference(Timestamp end, Timestamp start);

private:
    static const int kMicroSecondsPerSecond = 1000 * 1000;
    uint64_t microSecondsSinceEpoch_;
};

// 内联函数必须凡在头文件中！内联函数必须凡在头文件中！内联函数必须凡在头文件中！
// 静态成员函数同样可以访问类的私有成员函数
inline double Timestamp::timeDifference(Timestamp end, Timestamp start){
    uint64_t d=end.microSecondsSinceEpoch_-start.microSecondsSinceEpoch_;
    return static_cast<double>(d)/Timestamp::kMicroSecondsPerSecond;
}
}//namespace base
#endif //KYROSWEBSERVER_TIMESTAMP_H
