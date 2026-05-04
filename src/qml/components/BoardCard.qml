import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    required property var board
    required property bool selected

    signal clicked()

    implicitHeight: 148

    Rectangle {
        anchors.fill: parent
        radius: 26
        color: root.selected ? ThemeManager.colors.controlBackground : "#fff5e8"
        border.width: 2
        border.color: root.selected ? ThemeManager.colors.welcomeAccentYellow : "#181818"
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 14

        Item {
            Layout.preferredWidth: 112
            Layout.preferredHeight: 112

            Rectangle {
                anchors.fill: parent
                anchors.margins: 10
                radius: 18

                gradient: Gradient {
                    GradientStop { position: 0.0; color: root.board.accentA }
                    GradientStop { position: 1.0; color: root.board.accentB }
                }
            }

            Image {
                anchors.fill: parent
                source: ThemeManager.icons.frame || ""
                fillMode: Image.PreserveAspectFit
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Text {
                Layout.fillWidth: true
                text: board.name
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                elide: Text.ElideRight
                font.pixelSize: 18
                font.bold: true
                color: root.selected ? ThemeManager.colors.textColor : ThemeManager.colors.welcomeTextDark
            }

            Text {
                Layout.fillWidth: true
                text: board.subtitle
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                elide: Text.ElideRight
                font.pixelSize: 12
                color: root.selected ? "#d7cade" : "#5b4d57"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Rectangle {
                    radius: 11
                    color: root.selected ? "#332a3d" : "#ffe8c3"
                    border.width: 1
                    border.color: root.selected ? "#66547a" : "#181818"
                    implicitWidth: statusText.implicitWidth + 18
                    implicitHeight: 22

                    Text {
                        id: statusText
                        anchors.centerIn: parent
                        text: board.state
                        font.pixelSize: 11
                        font.bold: true
                        color: root.selected ? ThemeManager.colors.welcomeAccentYellow : "#141414"
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: board.updatedLabel
                    font.pixelSize: 11
                    color: root.selected ? "#c9bbd5" : "#5b4d57"
                    elide: Text.ElideRight
                }
            }

            Text {
                Layout.fillWidth: true
                text: board.collaborators + " collaborators"
                font.pixelSize: 11
                font.bold: true
                color: root.selected ? "#fff0dd" : "#141414"
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
