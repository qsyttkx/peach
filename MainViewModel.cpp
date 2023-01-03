#include "MainViewModel.h"

#include <QDebug>
#include <QTextStream>

extern "C" {
#include <libavformat/avformat.h>
}

MainViewModel::MainViewModel(QObject* parent)
    : QObject(parent)
{
}

bool MainViewModel::loaded() const { return m_loaded; }

QString MainViewModel::filepath() const { return m_filepath; }

int MainViewModel::pktInfosSize() const { return m_pktInfos.size(); }

QString MainViewModel::selectedPktInfo() const { return m_selectedPktInfo; }

void MainViewModel::load(QUrl filepath)
{
    unload();
    QString localFilepath = filepath.toLocalFile();
    qDebug() << "Load " << localFilepath;

    if (!demux(localFilepath.toStdString().c_str())) {
        qCritical() << "Load failed!";
        return;
    }

    m_loaded = true;
    m_filepath = localFilepath;

    emit loadedChanged();
    emit filepathChanged();
    emit pktInfosSizeChanged();
}

void MainViewModel::unload()
{
    if (!m_loaded) {
        return;
    }
    m_pktInfos.clear();
    m_maxPktSize = 0;
    m_loaded = false;
    m_filepath = "";

    emit pktInfosSizeChanged();
    emit loadedChanged();
    emit filepathChanged();
}

void MainViewModel::selectPacket(int index)
{
    if (index >= 0 && index < m_pktInfos.size()) {
        m_selectedPktInfo.clear();
        QTextStream ts(&m_selectedPktInfo);
        ts << "index: " << index << "\n";
        ts << "dts: " << m_pktInfos[index].dts << "\n";
        ts << "pts: " << m_pktInfos[index].pts << "\n";
        ts << "size: " << m_pktInfos[index].size << "\n";
        ts << "flags: " << m_pktInfos[index].flags << "\n";
        emit selectedPktInfoChanged();
    }
}

const QList<PktInfo>& MainViewModel::pktInfos() const { return m_pktInfos; }

int MainViewModel::maxPktSize() const { return m_maxPktSize; }

// TODO: 这得异步才行
bool MainViewModel::demux(const char* file)
{
    bool succ = false;
    AVFormatContext* fmt = nullptr;
    AVPacket* pkt = nullptr;
    do {
        int ret = avformat_open_input(&fmt, file, nullptr, nullptr);
        if (ret != 0) {
            qCritical() << "Call avformat_open_input failed: " << ret;
            break;
        }
        ret = avformat_find_stream_info(fmt, nullptr);
        if (ret < 0) {
            qCritical() << "Call avformat_find_stream_info failed: " << ret;
            break;
        }
        int idx = -1;
        for (int i = 0; i < int(fmt->nb_streams); i++) {
            if (fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                qInfo() << "Find a video steram, index " << i;
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            qCritical() << "Video stream not found!";
            break;
        }

        pkt = av_packet_alloc();

        while (av_read_frame(fmt, pkt) >= 0) {
            if (pkt->stream_index == idx) {
                PktInfo info;
                info.dts = pkt->dts;
                info.pts = pkt->pts;
                info.size = pkt->size;
                info.flags = pkt->flags;
                m_pktInfos.push_back(info);
                if (info.size > m_maxPktSize) {
                    m_maxPktSize = info.size;
                }
            }
            av_packet_unref(pkt);
        }
        succ = true;
    } while (false);

    if (pkt) {
        av_packet_free(&pkt);
    }
    if (fmt) {
        avformat_close_input(&fmt);
    }
    return succ;
}
