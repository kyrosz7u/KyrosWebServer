//
// Created by 樱吹雪 on 2022/2/3.
//

#ifndef KYROSWEBSERVER_TIMESTAMP_H
#define KYROSWEBSERVER_TIMESTAMP_H
#include "utils.h"

using namespace std;

class Timestamp{
public:
    Timestamp(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
    {
    }
    string toString() const;
    string toFormattedString(bool showMicroseconds) const;
    Timestamp now();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    uint64_t microSecondsSinceEpoch_;
};

#endif //KYROSWEBSERVER_TIMESTAMP_H
