#ifndef WIDGET_H
#define WIDGET_H

#include "IAsyncTask.h"

#include <ui_widget.h>

#include <QWidget>
#include <QDebug>
#include <QTimer>

using namespace GuiUtils;

class MyTask : public IAsyncTask {
Q_OBJECT

public:
    MyTask(QObject *parent = nullptr) : IAsyncTask(parent) {}

    virtual void onPreExecute() override {
        qDebug() << "onPreExecute()";
    }

    virtual void runInBackground() override {
        for(int i = 0; i < std::numeric_limits<int>::max(); ++i) {
            std::vector<int> vec;
            for(int j = 0; j < 10000; ++j) {
                vec.push_back(j);
            }
            qDebug() << "iteration = " << i;
        }
    }

    virtual void onPostExecute() override {
        qDebug() << "onPostExecute()";
    }

};

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void on_startBtn_clicked();
    void on_cancelBtn_clicked();
    void onTimer();
    void onPreExecute();
    void onPostExecute();
    void onCancelled();

private:
    Ui::Widget *ui;
    QTimer mTimer;
    MyTask *mAsyncTask;
    int counter = 0;


};

#endif // WIDGET_H
