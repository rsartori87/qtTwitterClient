#include "twittertimelinemodel.h"

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

TwitterTimelineModel::TwitterTimelineModel(Twitter *twitter, QObject *parent) : QAbstractTableModel(parent), mTwitter(twitter), mReply(nullptr)
{
    connect(mTwitter, &Twitter::authenticated, this, &TwitterTimelineModel::authenticated);
    connect(mTwitter, &Twitter::authenticated, this, &TwitterTimelineModel::updateTimeline);
}

int TwitterTimelineModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tweets.size();
}

QVariant TwitterTimelineModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    auto it = tweets.begin();
    std::advance(it, index.row());
    switch (index.column()) {
    case 0:
        return QString::number(it->id);
    case 1:
        return it->createdAt; //.toString(Qt::ISODateWithMs);
    case 2:
        return it->user;
    case 3:
        return it->text;
    }
    return QVariant();
}

int TwitterTimelineModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant TwitterTimelineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QStringLiteral("Id");
        case 1:
            return QStringLiteral("Created At");
        case 2:
            return QStringLiteral("User");
        case 3:
            return QStringLiteral("Text");
        }
    }
    return section;
}

void TwitterTimelineModel::authenticate(const QPair<QString, QString> &clientCredentials)
{
    mTwitter->setClientCredentials(clientCredentials);
    mTwitter->grant();
}

QAbstractOAuth::Status TwitterTimelineModel::status() const
{
    return mTwitter->status();
}

void TwitterTimelineModel::updateTimeline()
{
    if (mTwitter->status() != Twitter::Status::Granted)
        QMessageBox::warning(nullptr, qApp->applicationName(), "Not authenticated");

    QUrl url("https://api.twitter.com/1.1/statuses/home_timeline.json");
    QVariantMap parameters;
    if (!tweets.empty()) {
        parameters.insert("since_id", QString::number(tweets.begin()->id));
    }
    mReply = mTwitter->get(url, parameters);
    connect(mReply, &QNetworkReply::finished, this, &TwitterTimelineModel::parseJson);
}

void TwitterTimelineModel::parseJson()
{
    QJsonParseError parseError{};
    Q_ASSERT(mReply);
    const auto data = mReply->readAll();
    mReply->deleteLater();
    const auto document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error) {
        qCritical() << "TwitterTimelineModel::parseJson, Error at:" << parseError.offset
                    << parseError.errorString();
        return;
    }

    if (document.isObject()) {
        const auto object = document.object();
        const auto errorArray = object.value("errors").toArray();
        Q_ASSERT_X(errorArray.size(), "parse", data);
        QStringList errors;
        for (const auto error : errorArray) {
            Q_ASSERT(error.isObject());
            Q_ASSERT(error.toObject().contains("message"));
            errors.append(error.toObject().value("message").toString());
        }
        QMessageBox::warning(nullptr, qApp->applicationName(), errors.join("<br />"));
        return;
    }

    // HAPPY PATH
    Q_ASSERT_X(document.isArray(), "parse", data);
    const auto array = document.array();
    if (!array.empty()) {
        beginInsertRows(QModelIndex(), 0, array.size() - 1);
        for (const auto &value: array) {
            Q_ASSERT(value.isObject());
            const auto object = value.toObject();
//            const auto createdAt = QDateTime::fromString(object.value("created_at").toString(), Qt::RFC2822Date);
            const auto createdAt = object.value("created_at").toString();
            tweets.push_front(Tweet {
                                   object.value("id").toVariant().toULongLong(),
                                   createdAt,
                                   object.value("user").toObject().value("name").toString(),
                                   object.value("text").toString()
                               });
        }
        endInsertRows();
    }
}
