/*!
   \file BarChartView.h
   \author Dane Gardner <gardner@krellinst.org>

   \section LICENSE
   This file is part of the QCustomPlot Views project
   Copyright (C) 2013-2014 Argo Navis Technologies, LLC

   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef PLUGINS_BARCHARTVIEW_BARCHARTVIEW_H
#define PLUGINS_BARCHARTVIEW_BARCHARTVIEW_H

#include <QAbstractItemView>

#include <ViewManager/IView.h>

#include "BarChartViewLibrary.h"
#include "qcustomplot/qcustomplot.h"

namespace Plugins {
namespace BarChartView {

class SyntaxHighlighter;

class BARCHARTVIEW_EXPORT BarChartView : public QAbstractItemView, public Core::ViewManager::IView
{
    Q_OBJECT
    Q_DISABLE_COPY(BarChartView)
    Q_INTERFACES(Core::ViewManager::IView)

public:
    explicit BarChartView(QWidget *parent = 0);
    ~BarChartView();

    /* BEGIN IView interface */
    virtual bool hasLegend();
    virtual bool legendVisible();
    virtual void setLegendVisible(bool visible);
    /* END IView interface */


    /* BEGIN QAbstractItemView interface */
public:
    virtual void setModel(QAbstractItemModel *model);
    virtual QRect visualRect(const QModelIndex &index) const;
    virtual void scrollTo(const QModelIndex &index,ScrollHint hint=EnsureVisible);
    virtual QModelIndex indexAt(const QPoint &point) const;
protected:
    virtual QModelIndex moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden(const QModelIndex &index) const;
    virtual void setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command);
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const;
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    /* END QAbstractItemView interface */


protected:
    double getKey(int rowIndex);

    void updateLabels();

protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void columnsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    void columnsRemoved(const QModelIndex &parent, int start, int end);
    void headerDataChanged(Qt::Orientation orientation, int start, int end);



private:
    QCustomPlot *m_CustomPlot;
    QList<QCPBars *> m_BarCharts;
    QCPAxis *m_KeyAxis;
    QCPAxis *m_ValueAxis;

};

} // namespace BarChartView
} // namespace Plugins

#endif // PLUGINS_BARCHARTVIEW_BARCHARTVIEW_H
