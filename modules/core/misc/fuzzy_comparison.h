#ifndef FUZZY_COMPARISON
#define FUZZY_COMPARISON

#include <qstring.h>
#include <qlist.h>
#include <qregularexpression.h>


class FuzzyComparison {
    static QStringList wordLetterPairs(const QString & str) {
        QStringList allPairs;

        QStringList words = str.split(QRegularExpression(QLatin1String("\\s")), QString::SkipEmptyParts);

        while(!words.isEmpty())
            allPairs.append(letterPairs(words.takeFirst()));

        return allPairs;
    }

    static QStringList letterPairs(const QString & str) {
        int numPairs = str.size() - 1;

        QStringList pairs;
        pairs.reserve(numPairs);

        for (int i = 0; i < numPairs; i++)
            pairs.append(str.mid(i, 2));

        return pairs;
    }
public:
    static float compareStrings(const QString & str1, const QString & str2) {
        QStringList pairs1 = wordLetterPairs(str1.toUpper());
        QStringList pairs2 = wordLetterPairs(str2.toUpper());

        int intersection = 0;
        int pairsAmount = pairs1.size() + pairs2.size();

        for (int i = 0; i < pairs1.size(); i++) {
            for (int j = 0; j < pairs2.size(); j++) {
                if (pairs1[i] == pairs2[j]) {
                    intersection++;
                    pairs2.removeAt(j);
                    break;
                }
            }
        }

        return ((2.0 * intersection) / pairsAmount) * 100;
    }

    static float compareStrings2(const QString & str1, const QString & str2, int group_length = 2, Qt::CaseSensitivity caseSense = Qt::CaseInsensitive) {
        if (str1.isEmpty() || str2.isEmpty()) return 100;

        qreal hits = 0;
        int padd = group_length - 1;
        QString buff = QStringLiteral(" ").repeated(padd);
        QString source = buff % str1 % buff;
        QString target = buff % str2 % buff;

        for (int i = 0; i < source.count() - padd; i++)
            if (target.contains(source.mid(i, group_length), caseSense)) hits++;

        return (100 * hits / (qMin(target.length(), source.length()) - padd));
    }
};

#endif // FUZZY_COMPARISON
