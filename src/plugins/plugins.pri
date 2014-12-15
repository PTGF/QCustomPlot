# This file is part of the QCustomPlot Views project
# Copyright (C) 2013-2014 Argo Navis Technologies, LLC
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

!isEmpty(PLUGINS_PRI_INCLUDED):error("plugins/Plugins.pri already included")
PLUGINS_PRI_INCLUDED = 1

include(../PTGF.pri)
include(../QCustomPlot.pri)

TEMPLATE      = lib
CONFIG       += plugin plugin_with_soname
CONFIG       += depend_includepath

win32:target.path = /
else:{
  exists( /usr/lib64 ) {
       target.path  = /lib64
  } else {
       target.path  = /lib
  }
}

INSTALLS     += target
