#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class ex
{
public:
	string error;
	ex(string message) : error(message) { }
};

#endif

