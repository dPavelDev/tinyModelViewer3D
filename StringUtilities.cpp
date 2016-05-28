#include "StringUtilities.h"


QVector<QStringRef> stringSplitBySpace(const QString &s, int count)
{
    QVector<QStringRef> result;

    for (int l = 0, r = 0; l < s.length(); ++l, r = std::max(l, r))
    {
        if (result.size() == count - 1) {
            result.push_back(QStringRef(&s, l, s.length() - l));
        }

        if (s[l].isSpace())
            continue;

        while (r < s.length() && !s[r].isSpace())
            ++r;

        result.push_back(QStringRef(&s, l, r - l));

        l = r - 1;
    }

    return result;
}
