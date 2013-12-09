/*!
   \file PlotViewPlugin.cpp
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

#include "PlotViewPlugin.h"

#include <PluginManager/PluginManager.h>

#include "PlotView.h"

namespace Plugins {
namespace PlotView {

/*! \namespace Plugins::PlotView
    \brief Contains the PlotViewPlugin.
 */

/*! \class PlotViewPlugin
    \version 0.4.dev
    \brief

    \todo Document this more explicitly.
 */

/*!
   \fn PlotViewPlugin::PlotViewPlugin()
   \brief Constructor.
 */
PlotViewPlugin::PlotViewPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "PlotView";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}


/*!
 * \brief PlotViewPlugin::viewName
 * \return
 */
QString PlotViewPlugin::viewName()
{
    return tr("Plot View");
}

/*!
 * \brief PlotViewPlugin::viewHandles
 * \param model
 * \return
 */
bool PlotViewPlugin::viewHandles(QAbstractItemModel *model)
{
    Q_UNUSED(model)

    if(model->columnCount() == 0 || model->rowCount() == 0) {
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
 * \brief PlotViewPlugin::viewWidget
 * \param model
 * \return
 */
QAbstractItemView *PlotViewPlugin::viewWidget(QAbstractItemModel *model)
{
    /* Check to see if we should even try to handle this model */
    if(!viewHandles(model)) {
        return NULL;
    }

    PlotView *view = new PlotView();
    view->setModel(model);
    return view;
}



/*!
   \fn PlotViewPlugin::~PlotViewPlugin()
   \brief Destructor.
 */
PlotViewPlugin::~PlotViewPlugin()
{
}

/*!
   \fn PlotViewPlugin::initialize()
   \brief
   \returns true if successful
 */
bool PlotViewPlugin::initialize(QStringList &args, QString *err)
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
   \fn PlotViewPlugin::shutdown()
   \brief
 */
void PlotViewPlugin::shutdown()
{
}

/*!
   \fn PlotViewPlugin::name()
   \brief
   \returns name of plugin
 */
QString PlotViewPlugin::name()
{
    return m_Name;
}

/*!
   \fn PlotViewPlugin::version()
   \brief
   \returns version of plugin
 */
QString PlotViewPlugin::version()
{
    return m_Version;
}

/*!
   \fn PlotViewPlugin::dependencies()
   \brief
   \returns list of dependecies
 */
QList<Core::PluginManager::Dependency> PlotViewPlugin::dependencies()
{
    return m_Dependencies;
}


} // namespace PlotView
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::PlotView::PlotViewPlugin)
#endif
