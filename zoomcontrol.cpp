#include "zoomcontrol.h"
#include <cmath>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>

ZoomControl::ZoomControl(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    m_label = new QLabel(tr("Zoom:"), this);
    layout->addWidget(m_label);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, 1000);
    m_slider->setValue(500);
    m_slider->setFixedWidth(150);
    layout->addWidget(m_slider);

    connect(m_slider, &QSlider::valueChanged, this, &ZoomControl::onSliderMoved);
}

void ZoomControl::setZoom(qreal zoom)
{
    m_slider->blockSignals(true);
    m_slider->setValue(sliderFromZoom(zoom));
    m_slider->blockSignals(false);
}

void ZoomControl::onSliderMoved(int value)
{
    qreal zoom = zoomFromSlider(value);
    emit zoomRequested(zoom);
}

qreal ZoomControl::zoomFromSlider(int sliderValue)
{
    double t = sliderValue / 1000.0;
    return MIN_ZOOM * pow(MAX_ZOOM / MIN_ZOOM, t);
}

int ZoomControl::sliderFromZoom(qreal zoom)
{
    double t = log(zoom / MIN_ZOOM) / log(MAX_ZOOM / MIN_ZOOM);
    return qRound(t * 1000);
}