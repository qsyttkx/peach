#pragma once

#include <QQuickPaintedItem>

class MainViewModel;

class PacketHistogram : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT

    // 柱状图每条柱子的宽度
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
public:
    PacketHistogram(QQuickItem* parent = nullptr);
    ~PacketHistogram() override;
    Q_INVOKABLE void setViewModel(MainViewModel* viewModel);
    void paint(QPainter* painter) override;
    qreal lineWidth() const;
    void setLineWidth(qreal width);

signals:
    void lineWidthChanged();

private:
    QPointer<MainViewModel> m_viewModel = nullptr;
    qreal m_lineWidth = 2;
};
