/*!
   \file BarChartViewPlugin.cpp
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

#include "BarChartViewPlugin.h"

#include <PluginManager/PluginManager.h>

#include "BarChartView.h"

namespace Plugins {
namespace BarChartView {

/*! \namespace Plugins::BarChartView
    \brief Contains the BarChartViewPlugin.
 */

/*! \class BarChartViewPlugin
    \version 0.3.dev
    \brief

    \todo Document this more explicitly.
 */

/*!
   \fn BarChartViewPlugin::BarChartViewPlugin()
   \brief Constructor.
 */
BarChartViewPlugin::BarChartViewPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "BarChartView";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}


/*!
 * \brief BarChartViewPlugin::viewName
 * \return
 */
QString BarChartViewPlugin::viewName()
{
    return tr("Bar Chart View");
}

/*!
 * \brief BarChartViewPlugin::viewHandles
 * \param model
 * \return
 */
bool BarChartViewPlugin::viewHandles(QAbstractItemModel *model)
{
    Q_UNUSED(model)

    if(model->columnCount() <= 0 || model->rowCount() <= 0) {
        return false;
    }

    bool okay;
    for(int column = 0; column < model->columnCount(); ++column) {
        for(int row = 0; row < model->rowCount(); ++row) {
            model->index(row, column).data().toDouble(&okay);
            if(!okay) {
                if(column != (model->columnCount()-1)) {  // If it's the last column, ignore it
                    return false;
                }
            }
        }
    }

    return true;
}

/*!
 * \brief BarChartViewPlugin::viewWidget
 * \param model
 * \return
 */
QAbstractItemView *BarChartViewPlugin::viewWidget(QAbstractItemModel *model)
{
    /* Check to see if we should even try to handle this model */
    if(!viewHandles(model)) {
        return NULL;
    }

    BarChartView *view = new BarChartView();
    view->setModel(model);
    return view;
}



/*!
   \fn BarChartViewPlugin::~BarChartViewPlugin()
   \brief Destructor.
 */
BarChartViewPlugin::~BarChartViewPlugin()
{
}

/*!
   \fn BarChartViewPlugin::initialize()
   \brief
   \returns true if successful
 */
bool BarChartViewPlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {

        Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
        pluginManager.addObject(this);

    } catch(...) {
        return false;
    }

    return true;
}

/*!
   \fn BarChartViewPlugin::shutdown()
   \brief
 */
void BarChartViewPlugin::shutdown()
{
}

/*!
   \fn BarChartViewPlugin::name()
   \brief
   \returns name of plugin
 */
QString BarChartViewPlugin::name()
{
    return m_Name;
}

/*!
   \fn BarChartViewPlugin::version()
   \brief
   \returns version of plugin
 */
QString BarChartViewPlugin::version()
{
    return m_Version;
}

/*!
   \fn BarChartViewPlugin::dependencies()
   \brief
   \returns list of dependecies
 */
QList<Core::PluginManager::Dependency> BarChartViewPlugin::dependencies()
{
    return m_Dependencies;
}


} // namespace BarChartView
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::BarChartView::BarChartViewPlugin)
#endif
