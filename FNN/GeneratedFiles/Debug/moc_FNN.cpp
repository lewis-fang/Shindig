/****************************************************************************
** Meta object code from reading C++ file 'FNN.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FNN.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FNN.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FNN_t {
    QByteArrayData data[19];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FNN_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FNN_t qt_meta_stringdata_FNN = {
    {
QT_MOC_LITERAL(0, 0, 3), // "FNN"
QT_MOC_LITERAL(1, 4, 11), // "ImpotsDatas"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 8), // "ImpotWts"
QT_MOC_LITERAL(4, 26, 12), // "SetInputSize"
QT_MOC_LITERAL(5, 39, 6), // "Launch"
QT_MOC_LITERAL(6, 46, 7), // "ViewOut"
QT_MOC_LITERAL(7, 54, 9), // "ViewiNPUT"
QT_MOC_LITERAL(8, 64, 1), // "i"
QT_MOC_LITERAL(9, 66, 8), // "AddLayer"
QT_MOC_LITERAL(10, 75, 13), // "LauchCNNModel"
QT_MOC_LITERAL(11, 89, 11), // "uiSetFCPara"
QT_MOC_LITERAL(12, 101, 2), // "fc"
QT_MOC_LITERAL(13, 104, 7), // "popBack"
QT_MOC_LITERAL(14, 112, 15), // "ChangeTrainMode"
QT_MOC_LITERAL(15, 128, 12), // "LaunchTraing"
QT_MOC_LITERAL(16, 141, 10), // "ImportIdlO"
QT_MOC_LITERAL(17, 152, 16), // "ImportCifarTrain"
QT_MOC_LITERAL(18, 169, 15) // "ImportCifarTest"

    },
    "FNN\0ImpotsDatas\0\0ImpotWts\0SetInputSize\0"
    "Launch\0ViewOut\0ViewiNPUT\0i\0AddLayer\0"
    "LauchCNNModel\0uiSetFCPara\0fc\0popBack\0"
    "ChangeTrainMode\0LaunchTraing\0ImportIdlO\0"
    "ImportCifarTrain\0ImportCifarTest"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FNN[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x0a /* Public */,
       3,    0,   90,    2, 0x0a /* Public */,
       4,    0,   91,    2, 0x0a /* Public */,
       5,    0,   92,    2, 0x0a /* Public */,
       6,    1,   93,    2, 0x0a /* Public */,
       7,    1,   96,    2, 0x0a /* Public */,
       9,    0,   99,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    1,  101,    2, 0x0a /* Public */,
      13,    0,  104,    2, 0x0a /* Public */,
      14,    1,  105,    2, 0x0a /* Public */,
      15,    0,  108,    2, 0x0a /* Public */,
      16,    0,  109,    2, 0x0a /* Public */,
      17,    0,  110,    2, 0x0a /* Public */,
      18,    0,  111,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FNN::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FNN *_t = static_cast<FNN *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ImpotsDatas(); break;
        case 1: _t->ImpotWts(); break;
        case 2: _t->SetInputSize(); break;
        case 3: _t->Launch(); break;
        case 4: _t->ViewOut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ViewiNPUT((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->AddLayer(); break;
        case 7: _t->LauchCNNModel(); break;
        case 8: _t->uiSetFCPara((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->popBack(); break;
        case 10: _t->ChangeTrainMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->LaunchTraing(); break;
        case 12: _t->ImportIdlO(); break;
        case 13: _t->ImportCifarTrain(); break;
        case 14: _t->ImportCifarTest(); break;
        default: ;
        }
    }
}

const QMetaObject FNN::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FNN.data,
      qt_meta_data_FNN,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FNN::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FNN::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FNN.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FNN::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
