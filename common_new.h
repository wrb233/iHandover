#pragma once

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include <QTextCodec>
#include <QSettings>
#include <QFile>
#include <QWidget>

#include "CpsWindowPos_Export.h"
#include "oms/oms_database.h"
#include "oms/qt_application.h"
#include "oms/data_all.h" 
#include "oms/database.h"
#include "oms/database_defs.h"
#include "oms/object.h"
#include "oms/oms_time.h"
#include "oms/object_type.h"
#include "oms/object_attribute.h"
#include "oms/choice_literals.h" 
#include "ormdatabase/ORM_database.h"
#include "ormdatabase/ORM_interface.h"
#include "orm/ormlib.h"
#include "userms/UserMSInterface.h"
#include <tolerance/basic_fault_tolerance.h>
#include "operationlog/libOpLogInterface.h"
#include "logger.h"
#include "ModelTranslate.h"


//#include "OTypeAndATypeInit.h"

using namespace log4cplus;
using namespace log4cplus::helpers;
