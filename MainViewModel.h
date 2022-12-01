#pragma once

#include <QObject>
#include <QUrl>
#include <QtQmlIntegration>

struct PktInfo {
    int64_t pts;
    int size;
    int flags;
};

class MainViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)
public:
    MainViewModel(QObject* parent = nullptr);
    bool loaded() const;
    QString filepath() const;
    Q_INVOKABLE void open(QUrl filepath);
    Q_INVOKABLE void close();
    const QList<PktInfo>& pktInfos() const;
    int maxPktSize() const;

signals:
    void loadedChanged();
    void filepathChanged();

private:
    bool demux(const char* file);

    bool m_loaded = false;
    QString m_filepath;
    QList<PktInfo> m_pktInfos;
    int m_maxPktSize = 0;
};
