#pragma once

#include <QQuickPaintedItem>

class MainViewModel;

class PacketHistogram : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    PacketHistogram(QQuickItem* parent = nullptr);
    ~PacketHistogram() override;
    Q_INVOKABLE void setViewModel(MainViewModel* viewModel);
    void paint(QPainter* painter) override;

private:
    QPointer<MainViewModel> m_viewModel = nullptr;
};