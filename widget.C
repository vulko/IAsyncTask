#include "widget.h"
#include "ui_widget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_startBtn_clicked()
{
    connect( &mTimer, SIGNAL(timeout()), this, SLOT(onTimer()) );

    mAsyncTask = new MyTask( qobject_cast<QObject*>(this) );
    connect( mAsyncTask, SIGNAL(onStart()), this, SLOT(onPreExecute()) );
    connect( mAsyncTask, SIGNAL(onDone()), this, SLOT(onPostExecute()) );
    connect( mAsyncTask, SIGNAL(onCancelled()), this, SLOT(onCancelled()) );
    mAsyncTask->execute();
}

void MyWidget::onTimer()
{
    counter++;

    QString label = ui->startBtn->text();
    ui->startBtn->setText(label + QString("\nElapsed ") + QString::number(counter));
}

void MyWidget::onPreExecute()
{
    ui->startBtn->setText(QString("Working in background..."));
    ui->cancelBtn->setText(QString("cancel"));
    mTimer.start(1000);
}

void MyWidget::onPostExecute()
{
    mTimer.stop();
    ui->startBtn->setText(QString("done!"));
}

void MyWidget::onCancelled()
{
    mTimer.stop();
    ui->startBtn->setText(QString("cancelled!"));
    ui->cancelBtn->setText(QString("not working"));
}


void MyWidget::on_cancelBtn_clicked()
{
    mAsyncTask->cancel();
}
