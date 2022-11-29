#include "MainViewModel.h"

#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
}

MainViewModel::MainViewModel(QObject* parent)
    : QObject(parent)
{
}

bool MainViewModel::loaded() const { return m_loaded; }

QString MainViewModel::filepath() const { return m_filepath; }

void MainViewModel::open(QUrl filepath)
{
    close();
    QString localFilepath = filepath.toLocalFile();
    qDebug() << "Open " << localFilepath;

    if (!demux(localFilepath.toStdString().c_str())) {
        qCritical() << "Open failed!";
        return;
    }

    m_loaded = true;
    emit loadedChanged();
    m_filepath = localFilepath;
    emit filepathChanged();
}

void MainViewModel::close()
{
    if (!m_loaded) {
        return;
    }
    m_pktInfos.clear();

    m_loaded = false;
    emit loadedChanged();
    m_filepath = "";
    emit filepathChanged();
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
                // for (int i = 0; i < pkt->side_data_elems; i++) {
                //     if (pkt->side_data[i].type == AVPacketSideDataType::AV_PKT_DATA_QUALITY_STATS) {
                //         AVPictureType frameType = (AVPictureType)pkt->side_data[i].data[4];
                //     }
                // }

                PktInfo info;
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