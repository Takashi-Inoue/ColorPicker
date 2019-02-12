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
#ifndef SVTRIANGLE_H
#define SVTRIANGLE_H

#include <QPolygonF>
#include <QLinearGradient>

class QPainter;

class SVTriangle
{
public:
    SVTriangle(const QPointF &origin, double circumradius, double initS = 1.0, double initV = 1.0);
    virtual ~SVTriangle() = default;

//    bool contains(const QPointF &pos, double currentHueF) const;
    double saturation() const;
    double value() const;
    QPointF svPos(double hueF) const;

    void draw(double hueF, QPainter &painter);
    void updateSV(const QPointF &pos, double currentHueF);
    void setSV(double s, double v);

private:
    void calcSVonInsidePos(const QPointF &svPos);
    void calcSVonOutsidePos(const QPointF &svPos);
    void setSVfromVertex(int vertexNumber);
    void setSFromX(double x);
    void setVFromY(double y);
    bool checkPosAroundVertex0(const QPointF &svPos); // vertex0(h:x, s:1.0, v:1.0) in svTriPolygon
    bool checkPosAroundVertex1(const QPointF &svPos); // vertex1(h:x, s:0.0, v:1.0)
    bool checkPosAroundVertex2(const QPointF &svPos); // vertex2(h:x, s:0.5, v:0.0)

    static constexpr int drawAngleCorrection = -30;
    const QPolygonF svTriPolygon;
    const QLineF vLine;
    const QLineF sLine;

    QPointF origin;

    double currentS;
    double currentV;

    QLinearGradient hueGradient;
    QLinearGradient blackGradient;
    QLinearGradient whiteGradient;
};

#endif // SVTRIANGLE_H
