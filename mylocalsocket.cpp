#include "mylocalsocket.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>

MyLocalSocket::MyLocalSocket(QString serverName, QWidget* parent)
    : QWidget(parent), nextBlockSize(0), revision(0)
    // Устанавливаем nextBlockSize и revision равными нулю
{
    // Инициализируем сокет
    localSocket = new QLocalSocket(this);

    // Устанавливаем соединение между сигналом ошибки сокета с обработчиком ошибок
    connect(localSocket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
        this, &MyLocalSocket::slotError);

    // Устанавливаем имя сервера, к которому сокет должен подключаться
    localSocket->setServerName(serverName);

    // Устанавливаем соединение между сигналом подключения сокета к серверу
    // и обработчиком сигнала
    connect(localSocket, SIGNAL(connected()), SLOT(slotConnected()));
    // Соединяем сигнал сокета о готовности приёма данных данных с обработчиком данных
    connect(localSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    // Инициализируем элементы интерфейса
    textEdit = new QTextEdit;
    sendRevision = new QPushButton("Send next revision");

    // Соединяем нажатие кнопки с обработчиком, передающим информацию о ревизии на сервер
    connect(sendRevision, SIGNAL(clicked()), this, SLOT(slotSendToServer()));

    // Настраиваем элементы интерфейса и формируем вид окна клиента
    textEdit->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Sender revisions"));
    layout->addWidget(textEdit);
    layout->addWidget(sendRevision);
    setLayout(layout);

    // Подключаем сокет к серверу
    localSocket->connectToServer();
}

MyLocalSocket::~MyLocalSocket()
{

}

// Слот чтения информации, получаемой от сервера
void MyLocalSocket::slotReadyRead()
{
    // Всё аналогично приёму информации на стороне сервера
    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!nextBlockSize)
        {
            if(localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
        }
        in >> nextBlockSize;

        if(localSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString string;
        in >> time >> string;

        textEdit->append(time.toString() + " " + string);
        nextBlockSize = 0;
    }
}

// Слот обработки ошибок сокета
void MyLocalSocket::slotError(QLocalSocket::LocalSocketError error)
{
    QString strError =
        "Error: " + (error == QLocalSocket::ServerNotFoundError ?
                     "The server was not found." :
                     error == QLocalSocket::PeerClosedError ?
                     "The server is closed." :
                     error == QLocalSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(localSocket->errorString()));
    textEdit->append(strError);
}

// Слот передачи информации на сервер
void MyLocalSocket::slotSendToServer()
{
    // Блок для передачи формируется аналогично тому, как это делается на сервере
    QByteArray arrayBlock;
    QDataStream out(&arrayBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    ++revision;
    QString message = "Revision: " + QString("%1").arg(revision);
    out << quint16(0) << QTime::currentTime() << message;

    out.device()->seek(0);
    out << quint16(arrayBlock.size() - sizeof(quint16));

    localSocket->write(arrayBlock);
}

// Слот обработки сигнала соединения с сервером
void MyLocalSocket::slotConnected()
{
    textEdit->append("Received the connected() signal");
}
