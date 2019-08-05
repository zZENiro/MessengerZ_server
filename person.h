#ifndef PERSON_H
#define PERSON_H

#include <QPixmap>
#include <QtCore>

class Person : public QString
{
    QPixmap* _photo;
    QString _name;
    QString _email;
    QString _password;

public:
    explicit Person();
    explicit Person(const Person&);

    void setPhoto(const QPixmap& map)
    {
        this->_photo = new QPixmap(map);
    }
    void setName(const QString& name)
    {
        this->_name = QString(name);
    }
    void setEmail(const QString& mail)
    {
        this->_email = QString(mail);
    }
    void setPassword(const QString& pass)
    {
        this->_password = QString(pass);
    }

    QString& password() {return this->_password; }
    QPixmap& photo() { return *this->_photo; }
    QString& name() { return this->_name; }
    QString& email() { return this->_email; }
};

#endif // PERSON_H
