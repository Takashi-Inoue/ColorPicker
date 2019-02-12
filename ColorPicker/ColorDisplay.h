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
#ifndef COLORDISPLAY_H
#define COLORDISPLAY_H

#include <QWidget>

namespace Ui {
class ColorDisplay;
}

class ColorDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit ColorDisplay(QWidget *parent = nullptr);
    ~ColorDisplay() override;

public slots:
    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void paintPixmap();

    Ui::ColorDisplay *ui;

    QPixmap pixmap;
    QBrush checkBrush;
    QColor color;
};

#endif // COLORDISPLAY_H
