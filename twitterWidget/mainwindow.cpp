#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include "twitter.h"
#include "authdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mTwitter = new Twitter(this);
    model = new TwitterTimelineModel(mTwitter, this);
    ui->tableView->setModel(model);
    AuthDialog* dialog = new AuthDialog(this);
    dialog->move(this->width() / 2 - dialog->width() / 2, this->height() / 2 - dialog->height() / 2);
    dialog->setHidden(true);

    connect(mTwitter, &Twitter::authenticate, [=](const QUrl& url) {
        dialog->openUrl(url);
        dialog->show();
    });

    connect(mTwitter, &Twitter::authenticated, [=]() {
        dialog->close();
        delete dialog;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete mTwitter;
}


void MainWindow::on_connectPushButton_clicked()
{
    if (ui->appKeyLineEdit->text().isEmpty() ||
            ui->appSecretLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, qApp->applicationName(), "The key are mandatory");
        return;
    }
    if (model->status() == Twitter::Status::Granted)
        model->updateTimeline();
    else {
        const auto clientIdentifier = ui->appKeyLineEdit->text();
        const auto clientSharedSecret = ui->appSecretLineEdit->text();
        model->authenticate(qMakePair(clientIdentifier, clientSharedSecret));
    }
}
