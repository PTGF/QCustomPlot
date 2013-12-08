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

include(../../PTGF.pri)

QT       += testlib
TEMPLATE  = app

CONFIG(debug, debug|release) {
  TARGET = $${APPLICATION_TARGET}ManualTestsD
} else {
  TARGET = $${APPLICATION_TARGET}ManualTests
}

SOURCES += \
    manual.cpp
HEADERS +=
FORMS   +=

LIBS    += -L$$quote($${BUILD_PATH}/core/lib/$${DIR_POSTFIX}) -lCore$${LIB_POSTFIX}

win32:target.path = /
else:target.path  = /bin

INSTALLS += target
