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
    for (int i = 0; i < infos.size(); i++) {
        qreal x = i * m_lineWidth;
        if (x > width()) {
            break;
        }
        qreal y = (qreal)infos[i].size / (qreal)maxSize * height();
        pen.setColor(infos[i].flags != 0 ? Qt::cyan : Qt::lightGray);
        painter->setPen(pen);
        painter->drawLine(QPointF(x, height() - y), QPointF(x, height()));
    }
}

qreal PacketHistogram::lineWidth() const { return m_lineWidth; }

void PacketHistogram::setLineWidth(qreal width)
{
    m_lineWidth = width;
    emit lineWidthChanged();
    update();
}
