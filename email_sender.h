#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

#include <QObject>
#include <QSslSocket>

class EmailSender : public QObject
{
    Q_OBJECT

public:
    explicit EmailSender(QObject *parent = nullptr);
    void sendEmail(const QString &toEmail, const QString &subject, const QString &body);

private slots:
    void onConnected();
    void onEncrypted();
    void onReadyRead();

private:
    QSslSocket *socket;
    QString smtpServer;
    int smtpPort;
    QString emailFrom;
    QString emailPassword;
    QString emailTo;
    QString emailSubject;
    QString emailBody;
    QStringList emailQueue;
    int step;
};

#endif // EMAIL_SENDER_H
