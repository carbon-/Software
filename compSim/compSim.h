#ifndef COMPSIM_H
#define COMPSIM_H
#include <QObject>

class computer : public computer
{
    Q_OBJECT

public:
    explicit computer(QObject *parent = 0);
    ~computer();

protected:

};

#endif // COMPSIM_H
