#include "webdaq.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QEventLoop>
#include <QCoreApplication>
#include <QDebug>
#include <QDataStream>
#include <QVector>

WebDAQ::WebDAQ(QObject *parent) : QObject(parent) {}

WebDAQ::~WebDAQ() {}

WebDAQ::ScheduleStatus WebDAQ::getScheduleStatus(const QString &base_url)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;

    request.setUrl(QUrl(base_url + "/schedule/status"));

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Schedule Status Request Error: " << reply->errorString();
        return ScheduleStatus::scheduleerror;
    }

    QByteArray data = reply->readAll();
    QJsonDocument scheduleStatusJson = QJsonDocument::fromJson(data);
    int statusCode = scheduleStatusJson.object()["statusCode"].toInt();

    return static_cast<ScheduleStatus>(statusCode);
}

QString WebDAQ::getJobName(const QString &base_url)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;

    request.setUrl(QUrl(base_url + "/schedule/status"));

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Job Name Request Error: " << reply->errorString();
        return "";
    }

    QByteArray data = reply->readAll();
    QJsonDocument scheduleStatusJson = QJsonDocument::fromJson(data);
    QString jobName = scheduleStatusJson.object()["currentJobName"].toString();

    return jobName;
}

WebDAQ::JobStatus WebDAQ::getJobStatus(const QString &base_url, const QString &job_name)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;

    request.setUrl(QUrl(base_url + "/schedule/jobs/" + job_name + "/status"));
    qDebug() << base_url + "/schedule/jobs/" + job_name + "/status";
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Job Status Request Error: " << reply->errorString();
        return JobStatus::joberror;
    }

    QByteArray data = reply->readAll();
    QJsonDocument jobStatusJson = QJsonDocument::fromJson(data);
    qDebug() << jobStatusJson;
    int statusCode = jobStatusJson.object()["statusCode"].toInt();

    return static_cast<JobStatus>(statusCode);
}

QJsonObject WebDAQ::getJobStatusJson(const QString &base_url, const QString &job_name)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;

    request.setUrl(QUrl(base_url + "/schedule/jobs/" + job_name + "/status"));

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Job Status JSON Request Error: " << reply->errorString();
        return QJsonObject();
    }

    QByteArray data = reply->readAll();
    QJsonDocument jobStatusJson = QJsonDocument::fromJson(data);
    return jobStatusJson.object();
}

bool WebDAQ::isScheduleValid(const QJsonDocument &scheduleDescriptorJson)
{
    QJsonObject scheduleDescriptor = scheduleDescriptorJson.object();
    QJsonArray jobs = scheduleDescriptor["jobs"].toArray();

    return !jobs.isEmpty();
}

QString WebDAQ::createScheduleWaitingMsg(const QJsonDocument &scheduleDescriptorJson)
{
    QJsonObject scheduleDescriptor = scheduleDescriptorJson.object();
    QJsonArray jobs = scheduleDescriptor["jobs"].toArray();

    QString msg = "Waiting for schedule...\n";
    for (int i = 0; i < jobs.size(); ++i)
    {
        msg += "Job " + QString::number(i) + ": " + jobs[i].toString() + "\n";
    }

    return msg;
}

QString WebDAQ::createJobWaitingMsg(const QJsonDocument &jobDescriptorJson)
{
    QJsonObject jobDescriptor = jobDescriptorJson.object();
    QJsonArray channels = jobDescriptor["channels"].toArray();

    QString msg = "Waiting for job...\n";
    for (int i = 0; i < channels.size(); ++i)
    {
        msg += "Channel " + QString::number(i) + ": " + channels[i].toObject()["number"].toString() +  " " + channels[i].toObject()["unit"].toString() + "\n";
    }

    return msg;
}

QVector<double> WebDAQ::readJobData(const QString &base_url, const QString &job_name, int sample_index, int sample_count)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QVector<double> dataRetrieved;
    QString url = base_url + "/schedule/jobs/" + job_name + "/samples/" + QString::number(sample_index) + "/" + QString::number(sample_count) + "/bin";

    request.setUrl(QUrl(url));

    QNetworkReply *reply = manager.get(request);
    QEventLoop loop;

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        loop.quit();
    });

    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Read Job Data Error: " << reply->errorString();
        return QVector<double>();
    }

    QByteArray responseData = reply->readAll();

    QDataStream dStream(responseData);

    dStream.setByteOrder(QDataStream::LittleEndian);

    while(!dStream.atEnd()){
        double dVal;
        dStream >> dVal;
        if(!dStream.status()){
            dataRetrieved.append(dVal);
        }
        else{
            qWarning() << "failed to read data";
            break;
        }
    }
    qDebug() << "read data size is: " << dataRetrieved.size() << " double values from bin file";
    for(const auto& v: dataRetrieved){
        qDebug() << v;
    }
    qDebug() << "till here values retrieved from the variable";


    return dataRetrieved;
}
