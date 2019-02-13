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
#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QWidget>

namespace Ui {
class ColorPickerWidget;
}

class ColorPickerWidget : public QWidget
{
    Q_OBJECT

signals:
    void colorChanged(const QColor &color);

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);
    ~ColorPickerWidget() override;

    void setColor(const QColor &color);
    QColor getColor() const;

private slots:
    void onHsvChanged(double h, double s, double v);

    void on_lineEditARGB_textEdited(const QString &arg1);
    void on_sliderAlpha_actionTriggered(int action);

private:
    Ui::ColorPickerWidget *ui;

    QColor currentColor;
};

#endif // COLORPICKERWIDGET_H
