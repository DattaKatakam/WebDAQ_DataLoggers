The WebDAQ header file encapsulates the functionality for the WebDAQ algorithm in C++. It provides a class with methods and attributes to initialize, use, and access the data from Data logger.

Public methods
• Constructor (WebDAQ):
– Currently, i did not add code in this constructor but can add code if needed in future.

• getScheduleStatus Method:
– This method sends a request to the specified base URL to retrieve the status of the schedule. The schedule status can be one of the following:
–   ∗ schedulecompleted
    ∗ schedulestopped
    ∗ empty
    ∗ scheduleerror
    ∗ running
    ∗ waiting

• getJobName Method:
– Retrieves the current job name from the specified base URL, providing information about the ongoing job.

• getJobStatus Method:
– This method retrieves the status of a specific job identified by its name from the given base URL. Job status can be one of the following:
–   ∗ acquiring
    ∗ waitingForTrig
    ∗ joberror
    ∗ jobcompleted
    ∗ jobstopped
    ∗ jumped
    ∗ queued

• getJobStatusJson Method:
– Similar to getJobStatus, this method returns the status of a specific job but in JSON format.

• isScheduleValid Method:
– Checks if the provided schedule descriptor in JSON format is valid by examining the list of jobs.

• createScheduleWaitingMsg Method:
– Creates a user-friendly waiting message for the schedule based on the provided schedule descriptor.

• createJobWaitingMsg Method:
– Generates a waiting message for a job, displaying information about channels and their units.

• readJobData Method:
– Fetches data from a specific job identified by its name, starting from the specified sample index and reading a given number of samples when downloaded manually.Since we are accessing the data from c++ code we get response in BIN format only with temperature readings and we convert that BIN to readable format with in this method in the form of a QVector.

Dependencies i used
• iostream
• QObject
• QVector
• QJsonDocument
• QJsonObject
• QtNetwork/QNetworkRequest
• QJsonArray
• QEventLoop
• QDebug
• QDataStream• QtNetwork/QNetworkReply
• unordered map

Usage Include this header file in your project to utilize the WebDAQ sensor data logging and all other methods provided by the WebDAQ class. This library file will be useful when the Datalogger and PC are in same network.