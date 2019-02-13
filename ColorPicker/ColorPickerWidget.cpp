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
#include "ColorPickerWidget.h"
#include "ui_ColorPickerWidget.h"

#include <QProxyStyle>
#include <QDebug>

class DirectJumpSliderStyle : public QProxyStyle
{
public:
    DirectJumpSliderStyle(QStyle *style = nullptr) :
        QProxyStyle(style)
    {
    }

    ~DirectJumpSliderStyle() override = default;

    int styleHint(StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override
    {
        if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
            return Qt::LeftButton | Qt::MidButton;

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);

    ui->sliderAlpha->setStyle(new DirectJumpSliderStyle(ui->sliderAlpha->style()));

    onHsvChanged(0, 1, 1);

    connect(ui->hsvWidget, &HSVWidget::hsvChanged, this, &ColorPickerWidget::onHsvChanged);
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}

void ColorPickerWidget::onHsvChanged(double h, double s, double v)
{
    currentColor = QColor::fromHsvF(h, s, v);
    currentColor.setAlpha(ui->sliderAlpha->value());

    ui->colorDisplay->setColor(currentColor);
    ui->lineEditARGB->setText(currentColor.name(QColor::HexArgb).toUpper());

    emit colorChanged(currentColor);
}

void ColorPickerWidget::on_lineEditARGB_textEdited(const QString &arg1)
{
    if (arg1.size() < 9)
        return;

    currentColor.setNamedColor(arg1);

    ui->colorDisplay->setColor(currentColor);
    ui->sliderAlpha->setValue(currentColor.alpha());
    ui->hsvWidget->setColor(currentColor);

    emit colorChanged(currentColor);
}

void ColorPickerWidget::on_sliderAlpha_actionTriggered(int /*action*/)
{
    currentColor.setAlpha(ui->sliderAlpha->sliderPosition());

    ui->colorDisplay->setColor(currentColor);
    ui->lineEditARGB->setText(currentColor.name(QColor::HexArgb).toUpper());

    emit colorChanged(currentColor);
}
