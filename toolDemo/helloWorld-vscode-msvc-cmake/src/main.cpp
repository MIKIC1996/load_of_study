#include <iostream>

#include "algorithm/basic.h"
#include "mysql/mysql.h"
#include <QDebug>


int main(int, char**){
    std::cout << "Hello, from helloworld_cmake!\n"<<std::endl;
    std::cout << aerial::add(5,5) <<std::endl;
    qDebug()<<"hahahaha";

    MYSQL* server = new MYSQL();
    mysql_init(server);
    char val = 1;
    mysql_options(server, MYSQL_OPT_RECONNECT, &val);

    getchar();
    return 0;
}
