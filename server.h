#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QDataStream>
#include <QTcpSocket>
#include <QSettings>
#include <QHash>
#include "person.h"

namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

    QTcpServer* server;
    Person person;
    QSettings setts;
    quint16 _nextBlockSize;
    QString login, password;
    QHash<QString, Person> dataBase[10];

    void sendToClient(QTcpSocket* sock, const Person&, bool regNeededEx,  bool badPasswordEx);
    void addToDB();

public:
    explicit Server(QWidget *parent = nullptr, int nPort = 1234);
    ~Server();

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void takeLogs(QTcpSocket* sock);

private:
    Ui::Server *ui;
};

#endif // SERVER_H
