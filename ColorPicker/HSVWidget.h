/*
 * Copyright 2019 Takashi Inoue
 *
 * This file is part of ColorPicker.
 *
 * ColorPicker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ColorPicker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ColorPicker.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HSVWIDGET_H
#define HSVWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QRegion>
#include <memory>

class SVTriangle;

namespace Ui {
class HSVWidget;
}

class HSVWidget : public QWidget
{
    Q_OBJECT

signals:
    void hsvChanged(double h, double s, double v);

public:
    explicit HSVWidget(QWidget *parent = nullptr);
    ~HSVWidget() override;

    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    constexpr static double minScale = 0.8;
    constexpr static double circleCenterScale = 0.9;

    void updateCurrentHue(const QPoint &cursorPos);
    void updateTriangle();
    void createHCirclePixmap();

    Ui::HSVWidget *ui;

    QPointF origin;
    double minRadius;

    QPixmap pixmap;
    QPixmap pixmapHCircle;
    double currentH;

    QPainterPath hCircle;
    QPainterPath minCircle;
    std::unique_ptr<SVTriangle> svTriangle;

    bool isKeepHCircle;
    bool isKeepSVTriangle;
};

#endif // HSVWIDGET_H
