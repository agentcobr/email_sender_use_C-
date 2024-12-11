#include <QCoreApplication>
#include "email_sender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    EmailSender emailSender;
    emailSender.sendEmail("_____@gmail.com", "2FA Code", "Your verification code is: 123456");

    return a.exec();
}
