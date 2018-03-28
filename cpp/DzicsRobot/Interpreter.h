#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>

class Interpreter : public QObject
{
	Q_OBJECT

public:
	Interpreter(QObject *parent);
	~Interpreter();

	int linnerMove();
	int arcMove();

private:
	
};

#endif // INTERPRETER_H
