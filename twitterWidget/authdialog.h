#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QWidget>
#include <QUrl>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

public slots:
    void openUrl(const QUrl& url);

private:
    Ui::AuthDialog *ui;
};

#endif // AUTHDIALOG_H
