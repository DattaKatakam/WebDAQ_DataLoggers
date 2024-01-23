#ifndef WEBDAQ_H
#define WEBDAQ_H

#include "WebDAQ_global.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonArray>
#include <QEventLoop>
#include <QCoreApplication>
#include <QDebug>

class WEBDAQ_EXPORT WebDAQ: public QObject
{

    Q_OBJECT

public:
    enum ScheduleStatus
    {
        schedulecompleted,
        schedulestopped,
        empty,
        scheduleerror,
        running,
        waiting,
    };

    enum JobStatus
    {
        acquiring,
        waitingForTrig,
        joberror,
        jobcompleted,
        jobstopped,
        jumped,
        queued,
    };
    WebDAQ(QObject *parent = nullptr);
    ~WebDAQ();
    QString rawJsonResponse;

    ScheduleStatus getScheduleStatus(const QString &base_url);
    QString getJobName(const QString &base_url);
    JobStatus getJobStatus(const QString &base_url, const QString &job_name);
    QJsonObject getJobStatusJson(const QString &base_url, const QString &job_name);
    bool isScheduleValid(const QJsonDocument &scheduleDescriptorJson);
    QString createScheduleWaitingMsg(const QJsonDocument &scheduleDescriptorJson);
    QString createJobWaitingMsg(const QJsonDocument &jobDescriptorJson);
    QVector<double> readJobData(const QString &base_url, const QString &job_name, int sample_index, int sample_count);

//private:
//    QNetworkAccessManager manager;
};

#endif // WEBDAQ_H
