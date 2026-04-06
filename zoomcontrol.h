#ifndef ZOOMCONTROL_H
#define ZOOMCONTROL_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

/**
 * @brief The ZoomControl class provides a slider and label for zoom control.
 *        It uses logarithmic mapping between slider position and actual zoom factor.
 *        Emits zoomRequested(qreal) when the user changes the slider.
 */
class ZoomControl : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomControl(QWidget *parent = nullptr);

    // Set the current zoom factor (called from outside)
    void setZoom(qreal zoom);

signals:
    void zoomRequested(qreal zoom);   // emitted when user moves the slider

private slots:
    void onSliderMoved(int value);    // converts slider value to zoom factor

private:
    // Logarithmic conversion functions
    static qreal zoomFromSlider(int sliderValue);
    static int sliderFromZoom(qreal zoom);

    QSlider *m_slider;
    QLabel *m_label;

    // Zoom limits (should match ImageViewer's constants)
    static constexpr qreal MIN_ZOOM = 0.05;
    static constexpr qreal MAX_ZOOM = 10.0;
};

#endif // ZOOMCONTROL_H