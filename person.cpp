#include "person.h"

Person::Person()
{
 //
}

Person::Person(const Person &prs)
{
    this->_name = prs._name;
    this->_email = prs._email;
    this->_photo = prs._photo;
}
