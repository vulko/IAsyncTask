IAsyncTask is an interface that allows to easily implement async behavior with resulting in something.

This extends QObject, so it's possible to connect to it's signals and stick to Qt objects lifetime paradigm.

Main use of this class is for UI being reactive to result of async work done on a separate thread.
No need to have pain and loads of useless code.


**API consists of 3 simple methods to be overridden:**
```
    1) virtual void onPreExecute()      This is called on CALLER thread before execution starts.
                                        Can be used to update UI before doing some async operation.
                                        For instance: disable UI element before result of async operation is known.

    2) virtual void runInBackground()   This is called on WORKER thread. Implement this to do some background work.
                                        Use this for heavy load or blocking operations that shouldn't be done on
                                        UI thread.

    3) virtual void onPostExecute()     This is called on CALLER thread after background work is done.
                                        Can be used to update UI when result of background work is available.
                                        For instance: enable and update UI element when result of async operation
                                                      is known.
 
 Instead of using pure virtual methods 1 and 2, following signals can be used. In case if things to be done inside
  those methods are affecting another component which should be QObject, that it's simple better to used signal
  connection and implement that code in the QObject that has access to objects that should be affected by the code:
 
    1) onStart()     Emited along with onPreExecute()
 
    2) onDone()      Emited along with onPostExecute()

    2) onFailed()    Emited when failed to do some work. Don't forget to emit it, if background
                     work failed to be done.
```

 **EXAMPLES OF USAGE:**

```
class MyTask : public IAsyncTask {
Q_OBJECT

public:
    MyTask(QObject *parent = nullptr) : IAsyncTask(parent) {}

    virtual void onPreExecute() override {
        // runs on caller thread
        qDebug() << "onPreExecute()";
    }

    virtual void runInBackground() override {
        // runs on worker thread
        for(int i = 0; i < std::numeric_limits<int>::max(); ++i) {
            std::vector<int> vec;
            for(int j = 0; j < 10000; ++j) {
                vec.push_back(j);
            }
            qDebug() << "iteration = " << i;
        }
    }

    virtual void onPostExecute() override {
        // runs on caller thread
        qDebug() << "onPostExecute()";
    }

};
```
 
```
     QPushButton btnConnect;

         class MyTask : public IAsyncTask {
         Q_OBJECT

         public:
             MyTask(QObject *parent = nullptr) : IAsyncTask(parent) {}

             virtual void onPreExecute() override { do nothing }
             virtual void runInBackground() override {
                 DO SOME BACKGROUND WORK HERE
             }
             virtual void onPostExecute() override { do nothing }
         };

     void onBtnConnectClicked() {
         IAsyncTask *asyncTask = new MyTask( qobject_cast<QObject*>(this) );
         connect( asyncTask, SIGNAL(onStart()), this, SLOT(beforeAsyncTaskExecute()) );
         connect( asyncTask, SIGNAL(onDone()), this, SLOT(afterAsyncTaskExecute()) );
         asyncTask->execute();
     }

     void beforeAsyncTaskExecute()
     {
         btnConnect.setText(QString("working in background..."));
     }
     void afterAsyncTaskExecute()
     {
         btnConnect.setText(QString("done!"));
     }
```

Author (c) Kirill Volkov 2017. https://github.com/vulko/
