/*
 *  Copyright (c) 2020 Sharaf Zaman <sharafzaz121@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef SVGMESHARRAY_H
#define SVGMESHARRAY_H

#include <QVector>

#include "SvgMeshPatch.h"

struct SvgMeshPosition {
    int row;
    int col;
    SvgMeshPatch::Type segmentType;

    SvgMeshPosition()
        : row(-1)
        , col(-1)
        , segmentType(SvgMeshPatch::Size)
    {
    }

    SvgMeshPosition(int row, int col, SvgMeshPatch::Type type)
        : row(row)
        , col(col)
        , segmentType(type)
    {
    }

    bool isValid() const {
        return row >= 0 && col >= 0;
    }
};

class KRITAFLAKE_EXPORT SvgMeshArray
{
public:
    SvgMeshArray();

    SvgMeshArray(const SvgMeshArray& other);

    ~SvgMeshArray();

    /// creates a default mesh in OBB coordinates (because it's easier and more logical in this case)
    void createDefaultMesh(const int nrows, const int ncols, const QColor color, const QSizeF size);

    void newRow();

    bool addPatch(QList<QPair<QString, QColor>> stops, const QPointF initialPoint);

    /// Get the point of a node in mesharray
    SvgMeshStop getStop(const SvgMeshPatch::Type edge, const int row, const int col) const;

    SvgMeshStop getStop(const SvgMeshPosition &pos) const;

    /// Get the Path Points for a segment of the meshpatch
    std::array<QPointF, 4> getPath(const SvgMeshPatch::Type edge, const int row, const int col) const;

    // overload
    SvgMeshPath getPath(const SvgMeshPosition &pos) const;

    SvgMeshPatch* getPatch(const int row, const int col) const;

    int numRows() const;
    int numColumns() const;

    void setTransform(const QTransform& matrix);

    QRectF boundingRect() const;

    /// Return the paths connected to the corner. Can be thought of as edges connected to a vertex
    QVector<SvgMeshPosition> getConnectedPaths(const SvgMeshPosition &position) const;

    void modifyHandle(const SvgMeshPosition &position, const std::array<QPointF, 4> &newPath);
    void modifyCorner(const SvgMeshPosition &position, const QPointF &newPos);

    void modifyColor(const SvgMeshPosition &position, const QColor &color);

private:
    /// return the shared path between two patches.
    /// NOTE: Not to be confused with getConnectedPaths
    QVector<SvgMeshPosition> getSharedPaths(const SvgMeshPosition &position) const;

    //  get color of a stop
    QColor getColor(SvgMeshPatch::Type edge, int row, int col) const;

private:
    /// where each vector is a meshrow
    QVector<QVector<SvgMeshPatch*>> m_array;
};

#endif // SVGMESHARRAY_H
