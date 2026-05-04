import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {
    id: root

    background: Item {
        Rectangle {
            anchors.fill: parent
            color: ThemeManager.colors.welcomeBgColor
        }

        BackgroundDecor {
            anchors.fill: parent
        }
    }

    Flickable {
        anchors.fill: parent
        contentWidth: width
        contentHeight: contentColumn.implicitHeight + contentColumn.y + 36
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: contentColumn
            x: 18
            y: 20
            width: parent.width - 36
            spacing: 18

            Item {
                width: parent.width
                height: 140

                Image {
                    source: ThemeManager.icons.logo || ""
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: parent.width
                radius: 28
                color: "#1f1826"
                border.width: 2
                border.color: "#181818"
                implicitHeight: heroLayout.implicitHeight + 28

                ColumnLayout {
                    id: heroLayout
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 8

                    Text {
                        Layout.fillWidth: true
                        text: "Mobile board browser"
                        font.pixelSize: 28
                        font.bold: true
                        color: ThemeManager.colors.textColor
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "Choose an account, open a synced board and keep the same ImagoRef mood on phone. Editing remains on desktop."
                        font.pixelSize: 14
                        color: "#d8c7e0"
                    }

                    Rectangle {
                        Layout.topMargin: 6
                        radius: 18
                        color: "#2c2335"
                        border.width: 1
                        border.color: "#5b456e"
                        implicitHeight: syncText.implicitHeight + 18
                        Layout.fillWidth: true

                        Text {
                            id: syncText
                            anchors.fill: parent
                            anchors.margins: 9
                            wrapMode: Text.WordWrap
                            text: AppState.syncStatus
                            font.pixelSize: 12
                            color: ThemeManager.colors.welcomeAccentYellow
                        }
                    }
                }
            }

            Column {
                width: parent.width
                spacing: 10

                Text {
                    text: "Account"
                    font.pixelSize: 22
                    font.bold: true
                    color: ThemeManager.colors.welcomeTextDark
                }

                ListView {
                    width: parent.width
                    height: 64
                    orientation: ListView.Horizontal
                    spacing: 10
                    model: AppState.accounts
                    clip: true

                    delegate: AccountChip {
                        account: modelData
                        selected: modelData.id === AppState.selectedAccountId
                        onClicked: AppState.selectAccount(modelData.id)
                    }
                }
            }

            Column {
                width: parent.width
                spacing: 10

                RowLayout {
                    width: parent.width

                    Text {
                        text: "Boards"
                        font.pixelSize: 22
                        font.bold: true
                        color: ThemeManager.colors.welcomeTextDark
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "Sync"
                        onClicked: AppState.refreshBoards()

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 13
                            font.bold: true
                            color: "#141414"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            radius: 18
                            color: ThemeManager.colors.welcomeAccentYellow
                            border.width: 1
                            border.color: "#181818"
                        }
                    }
                }

                Repeater {
                    model: AppState.boards

                    delegate: BoardCard {
                        width: contentColumn.width
                        board: modelData
                        selected: modelData.id === AppState.selectedBoardId
                        onClicked: AppState.selectBoard(modelData.id)
                    }
                }
            }

            Rectangle {
                width: parent.width
                radius: 28
                color: "#fff0dd"
                border.width: 2
                border.color: "#181818"
                implicitHeight: footLayout.implicitHeight + 22

                RowLayout {
                    id: footLayout
                    anchors.fill: parent
                    anchors.margins: 11
                    spacing: 12

                    Image {
                        source: ThemeManager.icons.character || ""
                        Layout.preferredWidth: 84
                        Layout.preferredHeight: 84
                        fillMode: Image.PreserveAspectFit
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        Text {
                            Layout.fillWidth: true
                            text: "Selected board"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#5b4d57"
                        }

                        Text {
                            Layout.fillWidth: true
                            text: AppState.selectedBoardName
                            wrapMode: Text.WordWrap
                            font.pixelSize: 20
                            font.bold: true
                            color: ThemeManager.colors.welcomeTextDark
                        }

                        Text {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: "This mobile shell is focused on browsing synced boards and choosing the active account. Creation and editing stay on desktop."
                            font.pixelSize: 12
                            color: "#5b4d57"
                        }
                    }
                }
            }
        }
    }
}
