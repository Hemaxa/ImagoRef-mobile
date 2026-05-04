import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    required property var account
    required property bool selected

    signal clicked()

    width: 156
    height: 58

    Rectangle {
        anchors.fill: parent
        radius: 29
        color: root.selected ? ThemeManager.colors.controlBackground : "#fff0dd"
        border.width: 2
        border.color: root.selected ? account.accent : "#181818"
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        Rectangle {
            Layout.preferredWidth: 42
            Layout.preferredHeight: 42
            radius: 21
            color: account.accent
            border.width: 1
            border.color: "#181818"

            Text {
                anchors.centerIn: parent
                text: account.initials
                font.pixelSize: 17
                font.bold: true
                color: "#141414"
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 1

            Text {
                Layout.fillWidth: true
                text: account.name
                color: root.selected ? ThemeManager.colors.textColor : ThemeManager.colors.welcomeTextDark
                font.pixelSize: 14
                font.bold: true
                elide: Text.ElideRight
            }

            Text {
                Layout.fillWidth: true
                text: account.handle
                color: root.selected ? "#c9bbd5" : "#5b4d57"
                font.pixelSize: 11
                elide: Text.ElideRight
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
