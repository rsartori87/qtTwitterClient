#ifndef TWITTERTIMELINEMODEL_H
#define TWITTERTIMELINEMODEL_H

#include "twitter.h"

#include <QtCore>
#include <QtNetwork>
#include <deque>

class TwitterTimelineModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TwitterTimelineModel(Twitter* mTwitter, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void authenticate(const QPair<QString, QString> &clientCredentials);
    QAbstractOAuth::Status status() const;

public slots:
    void updateTimeline();

signals:
    void authenticated();

private:
    Q_DISABLE_COPY(TwitterTimelineModel)

    void parseJson();

    struct Tweet {
        quint64 id;
//        QDateTime createdAt;
        QString createdAt;
        QString user;
        QString text;
    };

    std::deque<Tweet> tweets;
    Twitter* mTwitter;
    QNetworkReply* mReply;
};

#endif // TWITTERTIMELINEMODEL_H
