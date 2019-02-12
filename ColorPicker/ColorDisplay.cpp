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
#include "ColorDisplay.h"
#include "ui_ColorDisplay.h"

#include <QPainter>
#include <QResizeEvent>

ColorDisplay::ColorDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorDisplay)
{
    ui->setupUi(this);

    QPixmap pixmapCheck = QPixmap(16, 16);
    pixmapCheck.fill(Qt::white);

    QPainter painter(&pixmapCheck);

    painter.setPen(Qt::lightGray);
    painter.fillRect(0, 0,  8,  8, Qt::lightGray);
    painter.fillRect(8, 8, 16, 16, Qt::lightGray);
    painter.end();

    checkBrush.setTexture(pixmapCheck);
}

ColorDisplay::~ColorDisplay()
{
    delete ui;
}

void ColorDisplay::setColor(const QColor &color)
{
    if (this->color == color)
        return;

    this->color = color;

    paintPixmap();
    update();
}

void ColorDisplay::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.drawPixmap(rect(), pixmap);
}

void ColorDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    pixmap = QPixmap(event->size());

    paintPixmap();
}

void ColorDisplay::paintPixmap()
{
    if (pixmap.isNull())
        return;

    QPainter painter(&pixmap);

    painter.fillRect(pixmap.rect(), checkBrush);
    painter.fillRect(pixmap.rect(), color);
}
