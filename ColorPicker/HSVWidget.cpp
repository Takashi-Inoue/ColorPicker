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
#include "HSVWidget.h"
#include "ui_HSVWidget.h"

#include "HSVUtility.h"
#include "SVTriangle.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QDebug>

HSVWidget::HSVWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HSVWidget),
    minRadius(0.0),
    currentH(0.0),
    isKeepHCircle(false),
    isKeepSVTriangle(false)
{
    ui->setupUi(this);

    Q_ASSERT(width() == height());

    pixmap = QPixmap(size());
    pixmap.fill(Qt::white);

    pixmapHCircle = pixmap.copy();

    origin = pixmap.rect().center();

    minRadius = origin.x() * minScale;

    svTriangle = std::make_unique<SVTriangle>(origin, minRadius);

    minCircle.addEllipse(origin, minRadius, minRadius);
    hCircle.addEllipse(pixmap.rect());
    hCircle = hCircle.subtracted(minCircle);

    createHCirclePixmap();
    updateTriangle();
}

HSVWidget::~HSVWidget()
{
    delete ui;
}

void HSVWidget::setColor(const QColor &color)
{
    if (color.hueF() >= 0.0)
        currentH = color.hueF();

    svTriangle->setSV(color.saturationF(), color.valueF());

    updateTriangle();
    update();
}

void HSVWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter;

    painter.begin(this);
    painter.drawPixmap(QPoint(0, 0), pixmap);

    painter.setRenderHints(QPainter::Antialiasing);

    QPointF hPickerCenter(HSVUtility::pointFromHueF(currentH, origin, pixmap.width() * circleCenterScale / 2.0));
    QPointF svPickerCenter(svTriangle->svPos(currentH));

    painter.setPen(Qt::black);
    painter.drawEllipse(hPickerCenter, 6, 6);
    painter.drawEllipse(svPickerCenter, 6, 6);

    painter.setPen(Qt::white);
    painter.drawEllipse(hPickerCenter, 5, 5);
    painter.drawEllipse(svPickerCenter, 5, 5);

    painter.end();
}

void HSVWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if (isKeepHCircle) {
        updateCurrentHue(event->pos());
        updateTriangle();
        update();

        emit hsvChanged(currentH, svTriangle->saturation(), svTriangle->value());
    }

    if (isKeepSVTriangle) {
        svTriangle->updateSV(event->pos(), currentH);
        update();

        emit hsvChanged(currentH, svTriangle->saturation(), svTriangle->value());
    }
}

void HSVWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (hCircle.contains(event->pos())) {
        isKeepHCircle = true;
        isKeepSVTriangle = false;
        updateCurrentHue(event->pos());
        updateTriangle();
        update();

        emit hsvChanged(currentH, svTriangle->saturation(), svTriangle->value());

        return;
    }

    if (minCircle.contains(event->pos())) {
        isKeepHCircle = false;
        isKeepSVTriangle = true;
        svTriangle->updateSV(event->pos(), currentH);
        update();

        emit hsvChanged(currentH, svTriangle->saturation(), svTriangle->value());

        return;
    }
}

void HSVWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    isKeepHCircle = false;
    isKeepSVTriangle = false;
}

void HSVWidget::updateCurrentHue(const QPoint &cursorPos)
{
    currentH = HSVUtility::hueFFromPoint(cursorPos, origin);
}

void HSVWidget::updateTriangle()
{
    QPainter painter(&pixmap);

    painter.setPen(Qt::transparent);
    painter.drawPixmap(QPoint(0, 0), pixmapHCircle);

    svTriangle->draw(currentH, painter);
}

void HSVWidget::createHCirclePixmap()
{
    Q_ASSERT(!hCircle.isEmpty());
    Q_ASSERT(!pixmap.isNull());

    QPainter painter(&pixmapHCircle);

    int pixW = pixmap.width();
    int pixH = pixmap.height();

    for (int x = 0; x <= pixW; ++x) {
        painter.setPen(QColor::fromHsvF(HSVUtility::hueFFromPoint(QPointF(x, 0.0), origin), 1.0, 1.0));
        painter.drawLine(origin, QPointF(x, 0.0));

        painter.setPen(QColor::fromHsvF(HSVUtility::hueFFromPoint(QPointF(x, pixH), origin), 1.0, 1.0));
        painter.drawLine(origin, QPointF(x, pixH));
    }

    for (int y = 0; y <= pixH; ++y) {
        painter.setPen(QColor::fromHsvF(HSVUtility::hueFFromPoint(QPointF(0.0, y), origin), 1.0, 1.0));
        painter.drawLine(origin, QPointF(0.0, y));

        painter.setPen(QColor::fromHsvF(HSVUtility::hueFFromPoint(QPointF(pixW, y), origin), 1.0, 1.0));
        painter.drawLine(origin, QPointF(pixW, y));
    }

    QPainterPath path;
    path.addRect(pixmap.rect());

    painter.setPen(Qt::transparent);
    painter.setBrush(Qt::white);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawPath(path.subtracted(hCircle));
}
