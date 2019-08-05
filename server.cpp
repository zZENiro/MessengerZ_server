#include "server.h"
#include "ui_server.h"
#include <QMessageBox>

Server::Server(QWidget *parent, int nPort) :
    QMainWindow(parent),
    _nextBlockSize(0),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, nPort))
    {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unabe to start the server:"
                              + server->errorString()
                              ) ;
        server->close();
        qDebug() << "Error!";
        return;
    }

    //test
    Person prs;
    prs.setName("Nikita");
    prs.setPassword("321");
    prs.setEmail("smtth@mail.com");
    this->dataBase->insert("123", prs);

    connect(server, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection()));
}

void Server::takeLogs(QTcpSocket* sock)
{
    QHash<QString, Person>::iterator it = this->dataBase->begin();

    for( ; it != dataBase->end(); ++it)
    {
        if(((it).key()) == this->login && it.value().password() == this->password)
        {
            qDebug() << "Found";
            //sendToClient(sock, it.value());
            sendToClient(sock, Person(), false, true);
            break;
        }
    }
    if(it == dataBase->end())
    {
        qDebug() << "not found!";
        //sendToClient();
        sendToClient(sock, Person(), true, false);
    }
}

void Server::addToDB()
{


    //dataBase.insert();
}

void Server::slotNewConnection()
{
    qDebug() << "new Conn!";
    QTcpSocket* clientSocket = server->nextPendingConnection();
    connect(clientSocket , SIGNAL(disconnected()), clientSocket, SLOT(deleteLater())) ;
    connect(clientSocket , SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void Server::slotReadClient()
{
    // получаю логин-пароль
    QTcpSocket* pClientSocket = dynamic_cast<QTcpSocket*>(sender());
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_12);
    for (; ;) {
        if (!_nextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> _nextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < _nextBlockSize) {
            break;
        }

        in >> login >> password >> this->person;
        takeLogs(pClientSocket);
        _nextBlockSize = 0;
    }

    qDebug() << login << password;
}

void Server::sendToClient(QTcpSocket *sock, const Person &prs,
                          bool RegNeeded, bool badPasswordEx)
{
    QByteArray arrblck;
    QDataStream out(&arrblck, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Version::Qt_5_12);
    out << quint16(0) << prs << RegNeeded << badPasswordEx;
    out.device()->seek(0);
    out << quint16(arrblck.size() - sizeof(quint16));
    sock->write(arrblck);
}

Server::~Server()
{
    delete ui;
}
