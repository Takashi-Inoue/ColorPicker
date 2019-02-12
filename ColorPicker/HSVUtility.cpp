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
#include "HSVUtility.h"

#include <QString>
#include <QtMath>

QPointF HSVUtility::pointFromHue(double hue, const QPointF &origin, double radius)
{
    Q_ASSERT_X(hue >= 0.0 && hue <= 360.0, "HSVUtility::pointFromHue", ("hue(=" + QString::number(hue) + ") must be (0.0 - 360.0)").toUtf8());

    double radians = qDegreesToRadians(hue);

    return QPointF(qCos(radians) * radius + origin.x(),
                   qSin(radians) * radius + origin.y());
}

QPointF HSVUtility::pointFromHueF(double hueF, const QPointF &origin, double radius)
{
    return pointFromHue(hueF * 360.0, origin, radius);
}

double HSVUtility::hueFFromPoint(const QPointF &pos, const QPointF &origin)
{
    double degrees = qRadiansToDegrees(qAtan2(pos.y() - origin.y(), pos.x() - origin.x()));

    if (degrees < 0)
        degrees += 360;

    return degrees / 360.0;
}

QPointF HSVUtility::rotatePoint(const QPointF &pos, const QPointF &origin, const double degree)
{
    double currentDegree = qRadiansToDegrees(qAtan2(pos.y() - origin.y(), pos.x() - origin.x()));

    if (currentDegree < 0)
        currentDegree += 360;

    double radians = qDegreesToRadians(currentDegree + degree);

    double radius = qSqrt(qPow(pos.x() - origin.x(), 2) + qPow(pos.y() - origin.y(), 2));

    return QPointF(qCos(radians) * radius + origin.x(),
                   qSin(radians) * radius + origin.y());
}
