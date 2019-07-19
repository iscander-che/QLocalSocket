#include "mylocalsocket.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // Создаём и показываем объект класса MyLocalSocket, определённого далее,
    // для запуска клиента, подключаемого к серверу с именем "MyLocalServer"
    MyLocalSocket socket("MyLocalServer");
    socket.show();

    return app.exec();
}
