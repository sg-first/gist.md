#include <QCoreApplication>
#include <QRegExp>
#include <QDebug>

void RegexCaptureExample()
{
    QString pattern("(.*)=(.*)");
    QRegExp rx(pattern);

    QString str("a=100");
    int pos = str.indexOf(rx);              // 0, position of the first match.
                                            // Returns -1 if str is not found.
                                            // You can also use rx.indexIn(str);
    if ( pos >= 0 )
    {
        qDebug() << rx.matchedLength();     // 5, length of the last matched string
                                            // or -1 if there was no match
        qDebug() << rx.capturedTexts();     // QStringList(“a=100″, ”a”, ”100″),
                                            //   0: text matching pattern
                                            //   1: text captured by the 1st ()
                                            //   2: text captured by the 2nd ()

        qDebug() << rx.cap(0);              // a=100, text matching pattern
        qDebug() << rx.cap(1);              // a, text captured by the nth ()
        qDebug() << rx.cap(2);              // 100,
    }
}

void RegexReplaceExample()
{
    QString s = "a=100";
    s.replace(QRegExp("(.*)="), "b=");
    qDebug() << s;                          // b=100
}

void RegexReplaceExample2()
{
    QString s = "a=100";
    s.replace(QRegExp("(.*)=(.*)"), "\\1\\2=\\2");  // \1 is rx.cap(1), \2 is rx.cap(2)
    qDebug() << s;                                  // a100=100
}

void RegexMatchExample()
{
    QString pattern(".*=.*");
    QRegExp rx(pattern);

    bool match = rx.exactMatch("a=3");
    qDebug() << match;                      // True
}