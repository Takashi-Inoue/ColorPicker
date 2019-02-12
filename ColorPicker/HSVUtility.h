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
#ifndef HSVUTILITY_H
#define HSVUTILITY_H

#include <QPointF>

class HSVUtility
{
public:
    HSVUtility() = default;
    ~HSVUtility() = default;

    static QPointF pointFromHue(double hue, const QPointF &origin, double radius);
    static QPointF pointFromHueF(double hueF, const QPointF &origin, double radius);
    static double hueFFromPoint(const QPointF &pos, const QPointF &origin);
    static QPointF rotatePoint(const QPointF &pos, const QPointF &origin, const double degree);
};

#endif // HSVUTILITY_H
