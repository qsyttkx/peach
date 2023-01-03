#include "PacketHistogram.h"

#include <QDebug>
#include <QPainter>
#include <QPen>

#include "MainViewModel.h"

PacketHistogram::PacketHistogram(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
}

PacketHistogram::~PacketHistogram() { }

void PacketHistogram::setViewModel(MainViewModel* viewModel)
{
    m_viewModel = viewModel;
    connect(m_viewModel, &MainViewModel::loadedChanged, this, [this]() { update(); });
}

void PacketHistogram::seek(int index)
{
    if (index >= 0) {
        m_startIndex = index;
        update();
    }
}

void PacketHistogram::paint(QPainter* painter)
{
    if (!m_viewModel || !m_viewModel->loaded()) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::PenStyle::SolidLine);
    pen.setWidthF(m_lineWidth);

    auto& infos = m_viewModel->pktInfos();
    int maxSize = m_viewModel->maxPktSize();
    qreal x = m_lineWidth / 2;
    for (int i = m_startIndex; i < infos.size(); i++) {
        if (x > width()) {
            break;
        }
        qreal y = (qreal)infos[i].size / (qreal)maxSize * height();
        if (infos[i].flags == 1) {
            pen.setColor(infos[i].dts == infos[i].pts ? Qt::cyan : Qt::red);
        } else {
            pen.setColor(Qt::lightGray);
        }
        painter->setPen(pen);
        painter->drawLine(QPointF(x, height() - y), QPointF(x, height()));
        x += m_lineWidth;
    }
    if (x < width()) {
        painter->fillRect(QRectF(x, 0, width() - x, height()), QBrush(Qt::black));
    }
}

qreal PacketHistogram::lineWidth() const { return m_lineWidth; }

void PacketHistogram::setLineWidth(qreal width)
{
    m_lineWidth = width;
    emit lineWidthChanged();
    update();
}
