#ifndef TWITTER_H
#define TWITTER_H

#include <QtCore>
#include <QtNetwork>
#include <QtNetworkAuth>
#include "twitterLib_global.h"

class TWITTERLIB_EXPORT Twitter : public QOAuth1
{
    Q_OBJECT
public:
    Twitter(QObject *parent = nullptr);
    Twitter(const QPair<QString, QString> &clientCredentials, QObject *parent = nullptr);
    Twitter(const QString &screenName,
            const QPair<QString, QString> &clientCredentials,
            QObject *parent = nullptr);
signals:
    void authenticated();
    void authenticate(const QUrl& url);

private:
    Q_DISABLE_COPY(Twitter)

    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};

#endif // TWITTER_H
