#include "IAsyncTask.h"


using namespace GuiUtils;

IAsyncTask::IAsyncTask(QObject *parent)
    : QObject(parent),
      mWorkerThread( nullptr ),
      mIsRunning( false ),
      mWasCancelled( false )
{
    init();
}

IAsyncTask::~IAsyncTask()
{
    if (mWorkerThread != nullptr) {
        if (!mWorkerThread->isFinished()) {
            emit onFailed();
        }

        mWorkerThread->quit();
        mWorkerThread->wait();
    }
}

void IAsyncTask::init()
{
    if (mWorkerThread == nullptr && !mIsRunning) {
        mWorkerThread = new ExecuteThread(this, this);
        connect(mWorkerThread, SIGNAL(done()), this, SLOT(onBackgroundWorkDone()), Qt::QueuedConnection );
    }
}

void IAsyncTask::execute()
{
    if (mIsRunning) {
        std::cerr << "AsyncTask is already running!";
        return;
    }

    init();
    mWasCancelled = false;
    mIsRunning = true;
    onPreExecute();
    emit onStart();

    mWorkerThread->start();
}

void GuiUtils::IAsyncTask::cancel()
{
    if (!mIsRunning) {
        std::cerr << "AsyncTask either finished execution or wasn't yet executed!";
        return;
    }

    mWasCancelled = true;
    mWorkerThread->terminate();
    mWorkerThread->wait();
    mIsRunning = false;

    delete mWorkerThread;
    mWorkerThread = nullptr;

    onPostExecute();
    emit onCancelled();
}

void IAsyncTask::onBackgroundWorkDone()
{
    disconnect(mWorkerThread, SIGNAL(done()), this, SLOT(onBackgroundWorkDone()));

    mWorkerThread->deleteLater();
    mWorkerThread = nullptr;

    mIsRunning = false;
    onPostExecute();
    emit onDone();
}
