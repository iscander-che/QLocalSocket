#ifndef MYLOCALSOCKET_H
#define MYLOCALSOCKET_H

#include <QWidget>
#include <QLocalSocket>
#include <QTextEdit>
#include <QPushButton>

class MyLocalSocket : public QWidget
{
    Q_OBJECT

public:
    MyLocalSocket(QString serverName, QWidget* parent = 0);
    ~MyLocalSocket();

private:
    // Указатель на QLocalSocket
    QLocalSocket* localSocket;

    // Указатели на элементы интерфейса
    QTextEdit* textEdit;
    QPushButton* sendRevision;

    // Размер принимаемого от сервера блока
    quint16 nextBlockSize;

    // Номер ревизии, отправляемый серверу
    // Увеличивается при каждом нажатии QPushButton
    int revision;

private slots:
    // Слот чтения информации, получаемой от сервера
    void slotReadyRead();

    // Слот обработки ошибок сокета
    void slotError(QLocalSocket::LocalSocketError error);

    // Слот передачи информации на сервер
    void slotSendToServer();

    // Слот обработки сигнала соединения с сервером
    void slotConnected();
};

#endif // MYLOCALSOCKET_H
