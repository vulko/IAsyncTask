# IAsyncTask

Qt implemetation of AsyncTask (line in Android SDK) that allows to easily run code in worker thread with pre and post notify on caller thread.


1. preExecute()          _caller thread_
2. runInBackground()     _worker thread_
3. postExecute()         _caller thread_


Example of usage:
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
