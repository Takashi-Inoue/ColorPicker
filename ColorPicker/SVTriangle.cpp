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
#include "SVTriangle.h"
#include "HSVUtility.h"

#include <QPainter>
#include <QtMath>
#include <QDebug>

SVTriangle::SVTriangle(const QPointF &origin, double circumradius, double initS, double initV) :
    svTriPolygon({HSVUtility::pointFromHue(  0 - drawAngleCorrection, {0, 0}, circumradius),
                  HSVUtility::pointFromHue(120 - drawAngleCorrection, {0, 0}, circumradius),
                  HSVUtility::pointFromHue(240 - drawAngleCorrection, {0, 0}, circumradius)}
                 ),
    vLine(svTriPolygon.at(2), QPointF(svTriPolygon.at(2).x(), svTriPolygon.at(0).y())),
    sLine(svTriPolygon.at(0), svTriPolygon.at(1)),
    origin(origin),
    currentS(initS),
    currentV(initV)
{
    Q_ASSERT(circumradius > 0.0);

    hueGradient.setStart(svTriPolygon.at(0));
    hueGradient.setFinalStop((svTriPolygon.at(1) + svTriPolygon.at(2)) / 2);

    whiteGradient.setStart(svTriPolygon.at(1));
    whiteGradient.setFinalStop((svTriPolygon.at(0) + svTriPolygon.at(2)) / 2);
    whiteGradient.setColorAt(0, QColor::fromHsvF(0, 0, 1, 1));
    whiteGradient.setColorAt(1, QColor::fromHsvF(0, 0, 1, 0));

    blackGradient.setStart(svTriPolygon.at(2));
    blackGradient.setFinalStop((svTriPolygon.at(0) + svTriPolygon.at(1)) / 2);
    blackGradient.setColorAt(0, QColor::fromHsvF(0, 0, 0, 1));
    blackGradient.setColorAt(1, QColor::fromHsvF(0, 0, 0, 0));
}

//bool SVTriangle::contains(const QPointF &pos, double currentHueF) const
//{
//    return svTriPolygon.containsPoint(HSVUtility::rotatePoint(pos, origin, -currentHueF * 360) - origin, Qt::WindingFill);
//}

double SVTriangle::saturation() const
{
    return currentS;
}

double SVTriangle::value() const
{
    return currentV;
}

QPointF SVTriangle::svPos(double hueF) const
{
    double slen = sLine.length() * currentV;
    double s = slen * currentS - slen / 2;

    QPointF sv(s, vLine.pointAt(currentV).y());

    return HSVUtility::rotatePoint(sv + origin, origin, hueF * 360.0 + drawAngleCorrection);
}

void SVTriangle::draw(double hueF, QPainter &painter)
{
    painter.setRenderHints(QPainter::Antialiasing);

    painter.translate(origin);
    painter.rotate(hueF * 360 + drawAngleCorrection);

    painter.setCompositionMode(QPainter::CompositionMode_Multiply);

    painter.setBrush(blackGradient);
    painter.drawPolygon(svTriPolygon);

    painter.setBrush(whiteGradient);
    painter.drawPolygon(svTriPolygon);

    hueGradient.setColorAt(0, QColor::fromHsvF(hueF, 1, 1, 1));
    hueGradient.setColorAt(1, QColor::fromHsvF(hueF, 1, 1, 0));
    painter.setBrush(hueGradient);
    painter.drawPolygon(svTriPolygon);
}

void SVTriangle::updateSV(const QPointF &pos, double currentHueF)
{
    QPoint svPos((HSVUtility::rotatePoint(pos, origin, -currentHueF * 360 - drawAngleCorrection) - origin).toPoint());

    svTriPolygon.containsPoint(svPos, Qt::WindingFill) ? calcSVonInsidePos(svPos)
                                                       : calcSVonOutsidePos(svPos);

    //    qDebug() << currentS << "(" << svPos.x() << "):" << currentV << "(" << svPos.y() << ")";
}

void SVTriangle::setSV(double s, double v)
{
    currentS = s;
    currentV = v;
}

void SVTriangle::calcSVonInsidePos(const QPointF &svPos)
{
    QPointF intersectionPoint;

    QLineF(svTriPolygon.at(2), svPos).intersect(sLine, &intersectionPoint);

    currentS = (intersectionPoint.x() - sLine.p2().x()) / sLine.length();
    setVFromY(svPos.y());
}

void SVTriangle::calcSVonOutsidePos(const QPointF &svPos)
{
    if (checkPosAroundVertex0(svPos)
     || checkPosAroundVertex1(svPos)
     || checkPosAroundVertex2(svPos)) {
        return;
    }

    // equal sides
    if (svPos.y() < qMin(svTriPolygon.at(0).y(), svTriPolygon.at(1).y())) {
        QLineF line(svPos.x() >= 0.0 ? svPos : QPointF(-svPos.x(), svPos.y()), svTriPolygon.at(1));

        line.setAngle(-145);

        QPointF intersectionPoint;

        if (QLineF(svTriPolygon[0], svTriPolygon[2]).intersect(line, &intersectionPoint) == QLineF::UnboundedIntersection) {
            intersectionPoint.y() < 0.0 ? setSVfromVertex(2)
                                        : setSVfromVertex(svPos.x() >= 0.0 ? 0 : 1);
            return;
        }

        setVFromY(intersectionPoint.y());
        currentS = (svPos.x() >= 0.0) ? 1 : 0;

        return;
    }

    // base line
    currentV = 1.0;
    setSFromX(svPos.x());
}

void SVTriangle::setSVfromVertex(int vertexNumber)
{
    if (vertexNumber == 0) {
        currentS = 1.0;
        currentV = 1.0;
    } else if (vertexNumber == 1) {
        currentS = 0.0;
        currentV = 1.0;
    } else if (vertexNumber == 2) {
        currentS = 0.5;
        currentV = 0.0;
    } else {
        Q_ASSERT_X(false, "SVTriangle::setSVfromVertex", ("Illegal vertex number: " + QString::number(vertexNumber)).toUtf8());
    }
}

void SVTriangle::setSFromX(double x)
{
    int sLen = static_cast<int>(svTriPolygon.at(0).x() - svTriPolygon.at(1).x());
    currentS = (x - svTriPolygon.at(1).x()) / sLen;
}

void SVTriangle::setVFromY(double y)
{
    currentV = (y - svTriPolygon.at(2).y()) / vLine.length();
}

bool SVTriangle::checkPosAroundVertex0(const QPointF &svPos)
{
    auto &v0 = svTriPolygon.at(0);

    if ((svPos.x() < v0.x()) | (svPos.y() < v0.y()))
        return false;

    setSVfromVertex(0);

    return true;
}

bool SVTriangle::checkPosAroundVertex1(const QPointF &svPos)
{
    auto &v1 = svTriPolygon.at(1);

    if ((svPos.x() > v1.x()) | (svPos.y() < v1.y()))
        return false;

    setSVfromVertex(1);

    return true;
}

bool SVTriangle::checkPosAroundVertex2(const QPointF &svPos)
{
    auto &v2 = svTriPolygon.at(2);

    if (svPos.y() > v2.y())
        return false;

    QPointF posOnV2(svPos - v2);
    double degree = qRadiansToDegrees(qAtan2(posOnV2.y(), posOnV2.x()));

    if ((degree > -60) | (degree < -120))
        return false;

    setSVfromVertex(2);

    return true;
}
