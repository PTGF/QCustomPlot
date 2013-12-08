/*!
   \file BarChartView.cpp
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

#include "BarChartView.h"

#include <QVBoxLayout>
#include <QDebug>

namespace Plugins {
namespace BarChartView {

/*!
   \brief BarChartView::BarChartView

   You should not create a view from it's constructor directly, but rather from its associated IViewFactory
   through the ViewManager.  For example:

   \code
   // Grab the instance of the ViewManager
   Core::ViewManager::ViewManager &viewManager = Core::ViewManager::ViewManager::instance();

   // Create your own data model
   QAbstractItemModel *model = new DataModel(data);

   // Check to see if this view can handle your model
   if(viewManager.viewNames(model).contains("BarChartView")) {

       // Create the view using the model, and display it to the user
       QAbstractItemView *view = viewManager.viewWidget("BarChartView", model);
       this->layout()->insertWidget(view);

   }
   \endcode

   \param parent
 */
BarChartView::BarChartView(QWidget *parent) :
    QAbstractItemView(parent),
    m_CustomPlot(new QCustomPlot(this))
{
    m_KeyAxis = m_CustomPlot->yAxis;
    m_ValueAxis = m_CustomPlot->xAxis;

    m_CustomPlot->setupFullAxesBox();

    m_CustomPlot->legend->setVisible(true);

    m_CustomPlot->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    m_CustomPlot->setRangeZoom(Qt::Horizontal|Qt::Vertical);

    connect(m_CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->yAxis2, SLOT(setRange(QCPRange)));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_CustomPlot);
    setLayout(layout);
}

/*!
   \internal
   \brief BarChartView::~BarChartView
 */
BarChartView::~BarChartView()
{
}


bool BarChartView::hasLegend()
{
    return true;
}

bool BarChartView::legendVisible()
{
    return m_CustomPlot->legend->visible();
}

void BarChartView::setLegendVisible(bool visible)
{
    m_CustomPlot->legend->setVisible(visible);
}


double BarChartView::getKey(int row)
{
    // QCustomPlot requires a double for the key; if the conversion fails, default to the index count

    bool okay;

    double key = model()->headerData(row, Qt::Vertical).toDouble(&okay);

    if(!okay) {
        key = (double)row;
    }

    return key;
}



/*!
   \brief BarChartView::setModel
   \param model
 */
void BarChartView::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = this->model();

    if(model == oldModel) {
        return;
    }

    if(oldModel) {
//        disconnect(oldModel, SIGNAL(rowsInserted(QModelIndex,int,int)),          this, SLOT(rowsInserted(QModelIndex,int,int)));
        disconnect(oldModel, SIGNAL(columnsInserted(QModelIndex,int,int)),       this, SLOT(columnsInserted(QModelIndex,int,int)));
        disconnect(oldModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),           this, SLOT(rowsRemoved(QModelIndex,int,int)));
        disconnect(oldModel, SIGNAL(columnsRemoved(QModelIndex,int,int)),        this, SLOT(columnsRemoved(QModelIndex,int,int)));
        disconnect(oldModel, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged(Qt::Orientation,int,int)));
    }

    m_CustomPlot->clearGraphs();
    m_CustomPlot->clearPlottables();

    if(model) {
//        connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),          this, SLOT(rowsInserted(QModelIndex,int,int)));
        connect(model, SIGNAL(columnsInserted(QModelIndex,int,int)),       this, SLOT(columnsInserted(QModelIndex,int,int)));
        connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)),           this, SLOT(rowsRemoved(QModelIndex,int,int)));
        connect(model, SIGNAL(columnsRemoved(QModelIndex,int,int)),        this, SLOT(columnsRemoved(QModelIndex,int,int)));
        connect(model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged(Qt::Orientation,int,int)));
    }

    QAbstractItemView::setModel(model);

    if(model->columnCount() > 0) {
        columnsInserted(QModelIndex(), 0, model->columnCount() - 1);
    }

    if(model->rowCount() > 0) {
        rowsInserted(QModelIndex(), 0, model->rowCount() - 1);
    }

    for(int i = 0; i < m_CustomPlot->graphCount(); ++i) {
        m_CustomPlot->graph(i)->rescaleAxes((bool)i);
    }

    foreach(QCPBars *barChart, m_BarCharts) {
        barChart->rescaleAxes((bool)m_BarCharts.indexOf(barChart));
    }

    m_CustomPlot->replot();
}


void BarChartView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    if(parent.isValid()) {
        return;
    }

    for(int column = 0; column < model()->columnCount(); ++column) {
        QCPBars *barChart = m_BarCharts.at(column);

        for(int row = start; row <= end; ++row) {
            double value = model()->index(row, column, parent).data(Qt::DisplayRole).toDouble();
            barChart->addData(getKey(row), value);
        }

        updateLabels();

        barChart->rescaleAxes(true);
    }

    m_CustomPlot->replot();
}

void BarChartView::columnsInserted(const QModelIndex &parent, int start, int end)
{
    if(parent.isValid()) {
        return;
    }

    static const QColor colors[] = {
                                        Qt::red,
                                        Qt::green,
                                        Qt::blue,
                                        Qt::cyan,
                                        Qt::magenta,
                                        Qt::yellow,
                                        Qt::gray,
                                        Qt::darkRed,
                                        Qt::darkGreen,
                                        Qt::darkBlue,
                                        Qt::darkCyan,
                                        Qt::darkMagenta,
                                        Qt::darkYellow,
                                        Qt::darkGray
                                                            };

    for(int column = start; column <= end; ++column) {
        QColor color = colors[ m_BarCharts.count() % (sizeof(colors)/sizeof(colors[0]) ) ];

        QColor foregroundColor = color.darker();
        QVariant foregroundRole = model()->headerData(column, Qt::Horizontal, Qt::ForegroundRole);
        if(foregroundRole.isValid() && foregroundRole.type() == QVariant::Color) {
            foregroundColor = model()->headerData(column, Qt::Horizontal, Qt::ForegroundRole).value<QColor>();
        }

        QColor backgroundColor = color.lighter();
        QVariant backgroundRole = model()->headerData(column, Qt::Horizontal, Qt::BackgroundRole);
        if(backgroundRole.isValid() && backgroundRole.type() == QVariant::Color) {
            backgroundColor = model()->headerData(column, Qt::Horizontal, Qt::BackgroundRole).value<QColor>();
        }

        QPen pen = QPen(foregroundColor);
        pen.setWidthF((qreal)1.5);

        QBrush brush = QBrush(backgroundColor);

        QCPBars *barChart = new QCPBars(m_KeyAxis, m_ValueAxis);
        m_CustomPlot->addPlottable(barChart);

        if(m_BarCharts.count() > 0) {
            barChart->moveAbove(m_BarCharts.last());
        }

        m_BarCharts.append(barChart);

        barChart->setName(model()->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString());
        barChart->setPen(pen);
        barChart->setBrush(brush);
        barChart->rescaleAxes(true);
    }

    m_CustomPlot->replot();
}

void BarChartView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    if(parent.isValid()) {
        return;
    }

    /* We could probably get away with just removing the range of data, but there is the possibility that the
       data in the model isn't sorted by row, or otherwise linked to row number, so it's safer to manually remove
       each datapoint from each graph manually */

    for(int column = 0; column < model()->columnCount(); ++column) {
        for(int row = start; row <= end; ++row) {
            m_BarCharts.at(column)->removeData(getKey(row));
        }
    }

    updateLabels();


    m_CustomPlot->replot();
}

void BarChartView::columnsRemoved(const QModelIndex &parent, int start, int end)
{
    if(parent.isValid()) {
        return;
    }

    for(int column = start; column <= end; ++column) {
        m_CustomPlot->removePlottable(m_BarCharts.takeAt(column));
    }

    m_CustomPlot->replot();
}


void BarChartView::headerDataChanged(Qt::Orientation orientation, int start, int end)
{
    if(orientation == Qt::Horizontal) {
        for(int column = start; column < end; ++column) {
            m_BarCharts.at(column)->setName(model()->headerData(column, Qt::Horizontal).toString());
        }
    } else {
        //TODO: refresh the keys for all rows in the range!

        /* If the key data changes, we don't have the old data to compare to, which means we'd have to cache it in
           memory as we build the rows.  When it changes, we could remove the data  using the cached key and
           reinsert it using the new key */
    }

    m_CustomPlot->replot();
}



/*!
   \internal
   \brief BarChartView::visualRect

    Returns the rectangle on the viewport occupied by the item at index.

    If your item is displayed in several areas then visualRect should return the primary area that contains index
    and not the complete area that index might encompasses, touch or cause drawing.

   \param index
   \return the rectangle on the viewport occupied by the item at index.
 */
QRect BarChartView::visualRect(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QRect();
}


/*!
   \internal
   \brief BarChartView::scrollTo

   Scrolls the view if necessary to ensure that the item at index is visible. The view will try to position the item
   according to the given hint.

   \param index
   \param hint
 */
void BarChartView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    Q_UNUSED(index)
    Q_UNUSED(hint)
}

/*!
   \internal
   \brief BarChartView::indexAt

    Returns the model index of the item at the viewport coordinates point.

    In the base class this is a pure virtual function.

   \param point
   \return the model index of the item at the viewport coordinates point.
 */
QModelIndex BarChartView::indexAt(const QPoint &point) const
{
    Q_UNUSED(point)

    qDebug() << Q_FUNC_INFO << point;

    return(QModelIndex());
}

/*!
   \internal
   \brief BarChartView::moveCursor
   \param cursorAction
   \param modifiers
   \return
 */
QModelIndex BarChartView::moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    return(QModelIndex());
}

/*!
   \internal
   \brief BarChartView::horizontalOffset
   \return
 */
int BarChartView::horizontalOffset() const
{
    return 0;
}

/*!
   \internal
   \brief BarChartView::verticalOffset
   \return
 */
int BarChartView::verticalOffset() const
{
    return 0;
}

/*!
   \internal
   \brief BarChartView::isIndexHidden
   \param index
   \return
 */
bool BarChartView::isIndexHidden(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return false;
}

/*!
   \internal
   \brief BarChartView::setSelection
   \param rect
   \param command
 */
void BarChartView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    Q_UNUSED(rect)
    Q_UNUSED(command)
}

/*!
   \internal
   \brief BarChartView::visualRegionForSelection
   \param selection
   \return
 */
QRegion BarChartView::visualRegionForSelection(const QItemSelection &selection) const
{
    Q_UNUSED(selection)
    return(QRegion());
}

/*!
   \internal
   \brief BarChartView::dataChanged
   \param topLeft
   \param bottomRight
 */
void BarChartView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);

    for(int column = topLeft.column(); column <= bottomRight.column(); ++column) {
        QCPBars *barChart = m_BarCharts.at(column);

        for(int row = topLeft.row(); row <= bottomRight.row(); ++row) {
            double value = model()->index(row, column).data().toDouble();
            barChart->addData(getKey(row), value);
        }

        barChart->rescaleAxes(true);
    }

    updateLabels();

    m_CustomPlot->replot();
}


void BarChartView::updateLabels()
{
    bool isStrings = false;
    QVector<qreal> ticks;
    QVector<QString> labels;

    for(int row = 0; row < model()->rowCount(); ++row) {
        QVariant header = model()->headerData(row, Qt::Vertical);
        labels << header.toString();
        ticks << (qreal)row;

        // Check that these are user-supplied strings
        if(!isStrings && header.type() == QVariant::String) {
            isStrings = true;
        }
    }


    /*! \internal
        \note Many tools output the variable length string identifier as the last column in the CLI, in order to keep the
              column widths the same.  If they do a straight conversion to a model, many things will break. We try to
              accomodate them here. */

    if(!isStrings) {
        // If the user didn't supply explicit header info, check the last column
        labels.clear();
        ticks.clear();

        for(int row = 0; row < model()->rowCount(); ++row) {
            QModelIndex index = model()->index(row, model()->columnCount() - 1);

            QVariant header = index.data();

            QString headerLabel = header.toString();
            if(headerLabel.length() > 8) {
                headerLabel = headerLabel.split(' ').first();

                if(headerLabel.length() > 16) {
                    headerLabel = headerLabel.left(16);
                }
            }

            labels << headerLabel;

            ticks << (qreal)row;

            // Check that these are user-supplied strings
            if(!isStrings && header.type() == QVariant::String) {
                isStrings = true;
            }
        }
    }

    if(isStrings) {
        m_KeyAxis->setAutoTicks(false);
        m_KeyAxis->setAutoTickLabels(false);
        m_KeyAxis->setTickVector(ticks);
        m_KeyAxis->setTickVectorLabels(labels);

        if(m_KeyAxis == m_CustomPlot->xAxis) {
            m_KeyAxis->setTickLabelRotation(60);
        }
        m_KeyAxis->setSubTickCount(0);

    } else {
        m_KeyAxis->setAutoTicks(true);
        m_KeyAxis->setAutoTickLabels(true);

    }
}


} // namespace BarChartView
} // namespace Plugins
