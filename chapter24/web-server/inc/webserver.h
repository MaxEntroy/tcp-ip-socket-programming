#ifndef WEBSERVER_INC_WEBSERVER_H_
#define WEBSERVER_INC_WEBSERVER_H_
#include <iostream>
#include <string>

void webserver(const std::string& ip, int port, int backlog);

void* request_handler(void* arg);
void error_handler(const std::string& msg);

#endif
