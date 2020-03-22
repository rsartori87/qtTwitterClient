#include "twitter.h"

#include <QtCore>
#include <QtNetwork>

constexpr int REPLY_PORT = 1337;

Twitter::Twitter(QObject *parent) :
    Twitter(QString(), qMakePair(QString(), QString()), parent)
{}

Twitter::Twitter(const QPair<QString, QString> &clientCredentials, QObject *parent) :
    Twitter(QString(), clientCredentials, parent)
{}

Twitter::Twitter(const QString &screenName, const QPair<QString, QString> &clientCredentials, QObject *parent) :
    QOAuth1(clientCredentials.first, clientCredentials.second, nullptr, parent)
{
    replyHandler = new QOAuthHttpServerReplyHandler(REPLY_PORT, this);
    replyHandler->setCallbackPath("callback");
    setReplyHandler(replyHandler);
    setTemporaryCredentialsUrl(QUrl("https://api.twitter.com/oauth/request_token"));
    setAuthorizationUrl(QUrl("https://api.twitter.com/oauth/authenticate"));
    setTokenCredentialsUrl(QUrl("https://api.twitter.com/oauth/access_token"));

    connect(this, &QAbstractOAuth::authorizeWithBrowser, [=](QUrl url) {
        QUrlQuery query(url);
        query.addQueryItem("force_login", "true");

        if (!screenName.isEmpty())
            query.addQueryItem("screen_name", screenName);
        url.setQuery(query);
        emit(authenticate(url));
    });

    connect(this, &QOAuth1::granted, this, &Twitter::authenticated);

    if (!clientCredentials.first.isEmpty() && !clientCredentials.second.isEmpty())
        grant();
}
