/****************************************************************************
** Meta object code from reading C++ file 'parametermodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../racecar_firmware/parametermodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parametermodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSParameterModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSParameterModelENDCLASS = QtMocHelpers::stringData(
    "ParameterModel",
    "parametersChanged",
    "",
    "ParameterTypeDef",
    "parameters",
    "set_kp",
    "text",
    "set_ki",
    "set_kd",
    "set_publish_frequency",
    "set_pid_frequency",
    "set_steering_esc_pwm_frequency",
    "set_steering_offset",
    "set_steering_ratio",
    "set_steering_max",
    "set_steering_min",
    "set_esc_offset",
    "set_esc_max",
    "set_esc_min",
    "set_brake_pwm_frequency"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSParameterModelENDCLASS_t {
    uint offsetsAndSizes[40];
    char stringdata0[15];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[11];
    char stringdata5[7];
    char stringdata6[5];
    char stringdata7[7];
    char stringdata8[7];
    char stringdata9[22];
    char stringdata10[18];
    char stringdata11[31];
    char stringdata12[20];
    char stringdata13[19];
    char stringdata14[17];
    char stringdata15[17];
    char stringdata16[15];
    char stringdata17[12];
    char stringdata18[12];
    char stringdata19[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSParameterModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSParameterModelENDCLASS_t qt_meta_stringdata_CLASSParameterModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "ParameterModel"
        QT_MOC_LITERAL(15, 17),  // "parametersChanged"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 16),  // "ParameterTypeDef"
        QT_MOC_LITERAL(51, 10),  // "parameters"
        QT_MOC_LITERAL(62, 6),  // "set_kp"
        QT_MOC_LITERAL(69, 4),  // "text"
        QT_MOC_LITERAL(74, 6),  // "set_ki"
        QT_MOC_LITERAL(81, 6),  // "set_kd"
        QT_MOC_LITERAL(88, 21),  // "set_publish_frequency"
        QT_MOC_LITERAL(110, 17),  // "set_pid_frequency"
        QT_MOC_LITERAL(128, 30),  // "set_steering_esc_pwm_frequency"
        QT_MOC_LITERAL(159, 19),  // "set_steering_offset"
        QT_MOC_LITERAL(179, 18),  // "set_steering_ratio"
        QT_MOC_LITERAL(198, 16),  // "set_steering_max"
        QT_MOC_LITERAL(215, 16),  // "set_steering_min"
        QT_MOC_LITERAL(232, 14),  // "set_esc_offset"
        QT_MOC_LITERAL(247, 11),  // "set_esc_max"
        QT_MOC_LITERAL(259, 11),  // "set_esc_min"
        QT_MOC_LITERAL(271, 23)   // "set_brake_pwm_frequency"
    },
    "ParameterModel",
    "parametersChanged",
    "",
    "ParameterTypeDef",
    "parameters",
    "set_kp",
    "text",
    "set_ki",
    "set_kd",
    "set_publish_frequency",
    "set_pid_frequency",
    "set_steering_esc_pwm_frequency",
    "set_steering_offset",
    "set_steering_ratio",
    "set_steering_max",
    "set_steering_min",
    "set_esc_offset",
    "set_esc_max",
    "set_esc_min",
    "set_brake_pwm_frequency"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSParameterModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       1,  149, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  104,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,  107,    2, 0x0a,    4 /* Public */,
       7,    1,  110,    2, 0x0a,    6 /* Public */,
       8,    1,  113,    2, 0x0a,    8 /* Public */,
       9,    1,  116,    2, 0x0a,   10 /* Public */,
      10,    1,  119,    2, 0x0a,   12 /* Public */,
      11,    1,  122,    2, 0x0a,   14 /* Public */,
      12,    1,  125,    2, 0x0a,   16 /* Public */,
      13,    1,  128,    2, 0x0a,   18 /* Public */,
      14,    1,  131,    2, 0x0a,   20 /* Public */,
      15,    1,  134,    2, 0x0a,   22 /* Public */,
      16,    1,  137,    2, 0x0a,   24 /* Public */,
      17,    1,  140,    2, 0x0a,   26 /* Public */,
      18,    1,  143,    2, 0x0a,   28 /* Public */,
      19,    1,  146,    2, 0x0a,   30 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,

 // properties: name, type, flags
       4, 0x80000000 | 3, 0x0001510b, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject ParameterModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSParameterModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSParameterModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSParameterModelENDCLASS_t,
        // property 'parameters'
        QtPrivate::TypeAndForceComplete<ParameterTypeDef, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ParameterModel, std::true_type>,
        // method 'parametersChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ParameterTypeDef &, std::false_type>,
        // method 'set_kp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_ki'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_kd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_publish_frequency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_pid_frequency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_steering_esc_pwm_frequency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_steering_offset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_steering_ratio'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_steering_max'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_steering_min'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_esc_offset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_esc_max'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_esc_min'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'set_brake_pwm_frequency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ParameterModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ParameterModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->parametersChanged((*reinterpret_cast< std::add_pointer_t<ParameterTypeDef>>(_a[1]))); break;
        case 1: _t->set_kp((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->set_ki((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->set_kd((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->set_publish_frequency((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->set_pid_frequency((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->set_steering_esc_pwm_frequency((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->set_steering_offset((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->set_steering_ratio((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->set_steering_max((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->set_steering_min((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->set_esc_offset((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->set_esc_max((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->set_esc_min((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->set_brake_pwm_frequency((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< ParameterTypeDef >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ParameterModel::*)(const ParameterTypeDef & );
            if (_t _q_method = &ParameterModel::parametersChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ParameterTypeDef >(); break;
        }
    }  else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ParameterModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< ParameterTypeDef*>(_v) = _t->parameters(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ParameterModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setParameters(*reinterpret_cast< ParameterTypeDef*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *ParameterModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParameterModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSParameterModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ParameterModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ParameterModel::parametersChanged(const ParameterTypeDef & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
