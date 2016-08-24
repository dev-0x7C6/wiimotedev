#ifndef WIIMOTEMESSSAGETHREAD_H
#define WIIMOTEMESSSAGETHREAD_H

#include <QThread>

class WiimoteMesssageThread : public QThread {
	Q_OBJECT
public:
	explicit WiimoteMesssageThread(QObject *parent = 0);

signals:

public slots:
};

#endif // WIIMOTEMESSSAGETHREAD_H
