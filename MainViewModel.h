#pragma once

#include <QObject>
#include <QUrl>
#include <QtQmlIntegration>

// Packet信息
struct PktInfo {
    int64_t pts;
    int size;
    int flags;
};

class MainViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT

    // 是否成功加载视频
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)

    // 加载的视频文件路径
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)

    // Packet信息列表的个数
    Q_PROPERTY(int pktInfosSize READ pktInfosSize NOTIFY pktInfosSizeChanged)

    // 所选Packet的信息字符串
    Q_PROPERTY(QString selectedPktInfo READ selectedPktInfo NOTIFY selectedPktInfoChanged)
public:
    MainViewModel(QObject* parent = nullptr);

    bool loaded() const;

    QString filepath() const;

    int pktInfosSize() const;

    QString selectedPktInfo() const;

    // 加载一个视频
    Q_INVOKABLE void load(QUrl filepath);

    // 卸载当前的视频
    Q_INVOKABLE void unload();

    Q_INVOKABLE void selectPacket(int index);

    // Packet信息列表
    const QList<PktInfo>& pktInfos() const;

    // 最大的Packet大小
    int maxPktSize() const;

signals:
    void loadedChanged();
    void filepathChanged();
    void pktInfosSizeChanged();
    void selectedPktInfoChanged();

private:
    bool demux(const char* file);

    bool m_loaded = false;
    QString m_filepath;
    QList<PktInfo> m_pktInfos;
    int m_maxPktSize = 0;
    QString m_selectedPktInfo;
};
