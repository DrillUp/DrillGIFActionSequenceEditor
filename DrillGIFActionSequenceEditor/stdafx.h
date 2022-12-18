#include <QtWidgets>
#include <QCoreApplication>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QString>
#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#pragma execution_character_set("utf-8")

// > 资源路径
#define QRC_IconSrcPath QString(":/DrillGIFActionSequenceEditor/Resources/icons")

// > 智能指针
#define CLASS_PTR(XXX) class XXX; typedef QSharedPointer<XXX> XXX##Ptr;
