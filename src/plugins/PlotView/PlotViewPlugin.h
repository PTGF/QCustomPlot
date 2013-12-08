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

#ifndef PLUGINS_PLOTVIEW_PLOTVIEWPLUGIN_H
#define PLUGINS_PLOTVIEW_PLOTVIEWPLUGIN_H

#include <QObject>

#include <PluginManager/IPlugin.h>
#include <ViewManager/IViewFactory.h>

namespace Plugins {
namespace PlotView {

class PlotView;

class PlotViewPlugin :
        public QObject,
        public Core::PluginManager::IPlugin,
        public Core::ViewManager::IViewFactory
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.krellinst.ptgf.PlotViewPlugin")
#endif

    Q_INTERFACES(Core::PluginManager::IPlugin)
    Q_INTERFACES(Core::ViewManager::IViewFactory)

public:
    PlotViewPlugin(QObject *parent = 0);

    /* IViewFactory Interface */
    QString viewName();
    bool viewHandles(QAbstractItemModel *model);
    QAbstractItemView *viewWidget(QAbstractItemModel *model);


    /* IPlugin Interface */
    ~PlotViewPlugin();
    bool initialize(QStringList &args, QString *err);
    void shutdown();
    QString name();
    QString version();
    QList<Core::PluginManager::Dependency> dependencies();

protected:
    QString m_Name;
    QString m_Version;
    QList<Core::PluginManager::Dependency> m_Dependencies;

};

} // namespace PlotView
} // namespace Plugins

#endif // PLUGINS_PLOTVIEW_PLOTVIEWPLUGIN_H
