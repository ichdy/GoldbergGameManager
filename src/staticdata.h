#ifndef STATICDATA_H
#define STATICDATA_H

#include <QString>

class StaticData
{
public:
    static StaticData &instance()
    {
        static StaticData data;
        return data;
    }

    QString loader;

private:
    StaticData() {}
};

#endif // STATICDATA_H
