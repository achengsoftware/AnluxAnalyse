#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectDB();
}

MainWindow::~MainWindow()
{
    m_dbconn.close();
    delete ui;
}

//创建数据库连接
void MainWindow::queryData()
{
    bool isOK = m_dbconn.open();
    QSqlQueryModel  q;
    q.setQuery(tr("SELECT * FROM CheckRecord"),m_dbconn);
    qDebug()<<Q_FUNC_INFO<<isOK<<q.columnCount();
    for(int i=0;i<q.rowCount();++i){
        QSqlRecord record = q.record(i);
        qDebug()<<Q_FUNC_INFO<<"RecordID:"<<record.value("RecordID").toString();
        qDebug()<<Q_FUNC_INFO<<"DataBlob:"<<record.value("DataBlob").toByteArray();
    }
    q.clear();
}

void MainWindow::connectDB()
{
    //获取是否存在有数据库连接
    m_dbconn = QSqlDatabase::database("AnluxDB", false);
    if(m_dbconn.isValid())//存在连接直接退出
    {
        qDebug()<<Q_FUNC_INFO<<"has mdb.";
        m_dbconn.close();
        return;
    }
    //创建一连接名为"test"的数据库连接
    m_dbconn = QSqlDatabase::addDatabase("QODBC", "AnluxDB");
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=");
    QString dbPath = QApplication::applicationDirPath()+"/AnluxDB.mdb";
    dsn.append(dbPath);
    qDebug()<<Q_FUNC_INFO<<"dbPath:"<<dbPath;

    m_dbconn.setDatabaseName(dsn);
    m_dbconn.setUserName("");//设置登陆数据库的用户名
    m_dbconn.setPassword("ipass");//设置密码
    queryData();
}
