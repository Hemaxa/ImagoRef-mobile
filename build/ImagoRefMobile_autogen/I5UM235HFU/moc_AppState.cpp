/****************************************************************************
** Meta object code from reading C++ file 'AppState.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/managers/AppState.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppState.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN8AppStateE_t {};
} // unnamed namespace

template <> constexpr inline auto AppState::qt_create_metaobjectdata<qt_meta_tag_ZN8AppStateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppState",
        "accountsChanged",
        "",
        "boardsChanged",
        "selectedAccountChanged",
        "selectedBoardChanged",
        "syncStatusChanged",
        "selectAccount",
        "accountId",
        "selectBoard",
        "boardId",
        "refreshBoards",
        "accounts",
        "QVariantList",
        "boards",
        "selectedAccountId",
        "selectedAccountName",
        "selectedBoardId",
        "selectedBoardName",
        "syncStatus"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'accountsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'boardsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'selectedAccountChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'selectedBoardChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'syncStatusChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectAccount'
        QtMocHelpers::MethodData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'selectBoard'
        QtMocHelpers::MethodData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Method 'refreshBoards'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'accounts'
        QtMocHelpers::PropertyData<QVariantList>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'boards'
        QtMocHelpers::PropertyData<QVariantList>(14, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'selectedAccountId'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'selectedAccountName'
        QtMocHelpers::PropertyData<QString>(16, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'selectedBoardId'
        QtMocHelpers::PropertyData<QString>(17, QMetaType::QString, QMC::DefaultPropertyFlags, 3),
        // property 'selectedBoardName'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags, 3),
        // property 'syncStatus'
        QtMocHelpers::PropertyData<QString>(19, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AppState, qt_meta_tag_ZN8AppStateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppState::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8AppStateE_t>.metaTypes,
    nullptr
} };

void AppState::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppState *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->accountsChanged(); break;
        case 1: _t->boardsChanged(); break;
        case 2: _t->selectedAccountChanged(); break;
        case 3: _t->selectedBoardChanged(); break;
        case 4: _t->syncStatusChanged(); break;
        case 5: _t->selectAccount((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->selectBoard((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->refreshBoards(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::accountsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::boardsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::selectedAccountChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::selectedBoardChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::syncStatusChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->accounts(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->boards(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->selectedAccountId(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->selectedAccountName(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->selectedBoardId(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->selectedBoardName(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->syncStatus(); break;
        default: break;
        }
    }
}

const QMetaObject *AppState::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppState::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AppState::accountsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AppState::boardsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AppState::selectedAccountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AppState::selectedBoardChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AppState::syncStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
