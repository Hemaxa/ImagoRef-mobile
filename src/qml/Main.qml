import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "pages"

ApplicationWindow {
    id: root

    visible: true
    width: 430
    height: 932
    minimumWidth: 360
    minimumHeight: 640
    title: "ImagoRef Mobile"
    color: "transparent"

    background: Rectangle {
        color: ThemeManager.colors.welcomeBgColor
    }

    HomePage {
        anchors.fill: parent
    }
}
