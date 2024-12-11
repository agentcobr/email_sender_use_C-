#include "email_sender.h"
#include <QDebug>

EmailSender::EmailSender(QObject *parent) : QObject(parent), step(0)
{
    smtpServer = "smtp.gmail.com";
    smtpPort = 465;
    emailFrom = "_______@gmail.com";      // changet to your email
    emailPassword = "---- ---- ---- ----";         // change your password

    socket = new QSslSocket(this);
    connect(socket, &QSslSocket::connected, this, &EmailSender::onConnected);
    connect(socket, &QSslSocket::encrypted, this, &EmailSender::onEncrypted);
    connect(socket, &QSslSocket::readyRead, this, &EmailSender::onReadyRead);
}

void EmailSender::sendEmail(const QString &toEmail, const QString &subject, const QString &body)
{
    emailTo = toEmail;
    emailSubject = subject;
    emailBody = body;

    socket->connectToHostEncrypted(smtpServer, smtpPort);
}

void EmailSender::onConnected()
{
    qDebug() << "Connected to SMTP server";
}

void EmailSender::onEncrypted()
{
    qDebug() << "Connection encrypted";

    // Начало SMTP-коммуникации
    socket->write("EHLO localhost\r\n");
    step = 1;
}

void EmailSender::onReadyRead()
{
    QString response = socket->readAll();
    qDebug() << "SMTP Response:" << response;

    switch (step) {
    case 1:
        if (response.contains("250")) {
            socket->write("AUTH LOGIN\r\n");
            step = 2;
        }
        break;
    case 2:
        if (response.contains("334")) {
            socket->write(QString("%1\r\n").arg(QString(emailFrom).toUtf8().toBase64()).toUtf8());
            step = 3;
        }
        break;
    case 3:
        if (response.contains("334")) {
            socket->write(QString("%1\r\n").arg(QString(emailPassword).toUtf8().toBase64()).toUtf8());
            step = 4;
        }
        break;
    case 4:
        if (response.contains("235")) {
            socket->write(QString("MAIL FROM:<%1>\r\n").arg(emailFrom).toUtf8());
            step = 5;
        }
        break;
    case 5:
        if (response.contains("250")) {
            socket->write(QString("RCPT TO:<%1>\r\n").arg(emailTo).toUtf8());
            step = 6;
        }
        break;
    case 6:
        if (response.contains("250")) {
            socket->write("DATA\r\n");
            step = 7;
        }
        break;
    case 7:
        if (response.contains("354")) {
            QString message = QString("Subject: %1\r\n\r\n%2\r\n.\r\n").arg(emailSubject, emailBody);
            socket->write(message.toUtf8());
            step = 8;
        }
        break;
    case 8:
        if (response.contains("250")) {
            socket->write("QUIT\r\n");
            step = 9;
        }
        break;
    case 9:
        qDebug() << "Email sent successfully!";
        socket->disconnectFromHost();
        break;
    default:
        break;
    }
}
