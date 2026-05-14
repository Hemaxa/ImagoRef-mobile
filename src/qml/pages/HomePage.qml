import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {
    id: root

    signal boardRequested(string boardId, string boardTitle)

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
        contentHeight: contentColumn.implicitHeight + contentColumn.y + 28
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: contentColumn
            x: 16
            y: 16
            width: parent.width - 32
            spacing: 14

            Item {
                width: parent.width
                height: 92

                Image {
                    anchors.fill: parent
                    source: ThemeManager.icons.logo || ""
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: parent.width
                radius: 24
                color: "#1f1826"
                border.width: 2
                border.color: "#181818"
                implicitHeight: heroLayout.implicitHeight + 24

                ColumnLayout {
                    id: heroLayout
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        Layout.fillWidth: true
                        text: "Облачные доски"
                        font.pixelSize: 24
                        font.bold: true
                        color: ThemeManager.colors.textColor
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "Открывайте те же референс-доски, что синхронизируются с настольной версией ImagoRef. Редактирование по-прежнему доступно на компьютере."
                        font.pixelSize: 13
                        color: "#d8c7e0"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 16
                        color: "#2c2335"
                        border.width: 1
                        border.color: "#5b456e"
                        implicitHeight: statusText.implicitHeight + 18

                        Text {
                            id: statusText
                            anchors.fill: parent
                            anchors.margins: 9
                            wrapMode: Text.WordWrap
                            text: AppState.syncStatus
                            font.pixelSize: 12
                            color: ThemeManager.colors.welcomeAccentYellow
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        visible: AppState.errorMessage !== ""
                        text: AppState.errorMessage
                        wrapMode: Text.WordWrap
                        font.pixelSize: 12
                        color: "#ffd7cc"
                    }
                }
            }

            Rectangle {
                width: parent.width
                radius: 24
                color: "#fff0dd"
                border.width: 2
                border.color: "#181818"
                implicitHeight: authLayout.implicitHeight + 22

                ColumnLayout {
                    id: authLayout
                    anchors.fill: parent
                    anchors.margins: 11
                    spacing: 10

                    RowLayout {
                        Layout.fillWidth: true

                        CircularAvatar {
                            Layout.preferredWidth: 44
                            Layout.preferredHeight: 44
                            borderColor: "#181818"
                            textColor: "#141414"
                            fallbackFontPixelSize: 18
                            fallbackText: AppState.loggedIn
                                ? (AppState.userDisplayName.length > 0 ? AppState.userDisplayName.charAt(0).toUpperCase() : "?")
                                : "?"
                            source: AppState.userAvatarUrl
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2

                            Text {
                                Layout.fillWidth: true
                                text: AppState.loggedIn ? AppState.userDisplayName : "Войти"
                                font.pixelSize: 20
                                font.bold: true
                                color: ThemeManager.colors.welcomeTextDark
                                elide: Text.ElideRight
                            }

                            Text {
                                Layout.fillWidth: true
                                text: AppState.loggedIn ? AppState.userEmail : "Используйте тот же аккаунт, что и на ПК"
                                font.pixelSize: 12
                                color: "#5b4d57"
                                elide: Text.ElideRight
                            }
                        }

                        Button {
                            visible: AppState.loggedIn
                            text: "Выйти"
                            onClicked: AppState.logout()

                            contentItem: Text {
                                text: parent.text
                                font.pixelSize: 12
                                font.bold: true
                                color: "#141414"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            background: Rectangle {
                                radius: 16
                                color: "#FFE135"
                                border.width: 1
                                border.color: "#181818"
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        visible: !AppState.loggedIn
                        spacing: 8

                        TextField {
                            id: emailField
                            Layout.fillWidth: true
                            placeholderText: "Email"
                            inputMethodHints: Qt.ImhEmailCharactersOnly
                            color: ThemeManager.colors.textColor

                            background: Rectangle {
                                color: ThemeManager.colors.controlBackground
                                border.color: emailField.activeFocus ? ThemeManager.colors.accentColor : "#181818"
                                border.width: 1
                                radius: 14
                            }
                        }

                        TextField {
                            id: passwordField
                            Layout.fillWidth: true
                            placeholderText: "Пароль"
                            echoMode: TextInput.Password
                            color: ThemeManager.colors.textColor
                            onAccepted: AppState.login(emailField.text, passwordField.text)

                            background: Rectangle {
                                color: ThemeManager.colors.controlBackground
                                border.color: passwordField.activeFocus ? ThemeManager.colors.accentColor : "#181818"
                                border.width: 1
                                radius: 14
                            }
                        }

                        Button {
                            id: loginButton
                            Layout.fillWidth: true
                            text: AppState.authenticating ? "Входим..." : "Войти"
                            enabled: !AppState.authenticating
                            onClicked: AppState.login(emailField.text, passwordField.text)

                            contentItem: Text {
                                text: parent.text
                                font.pixelSize: 14
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
                }
            }

            Column {
                width: parent.width
                spacing: 10
                visible: AppState.loggedIn

                RowLayout {
                    width: parent.width

                    Text {
                        text: "Ваши доски"
                        font.pixelSize: 22
                        font.bold: true
                        color: ThemeManager.colors.welcomeTextDark
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        text: AppState.loadingBoards ? "Загрузка..." : "Обновить"
                        enabled: !AppState.loadingBoards
                        onClicked: AppState.loadBoards()

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 12
                            font.bold: true
                            color: "#141414"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            radius: 16
                            color: "#FFE135"
                            border.width: 1
                            border.color: "#181818"
                        }
                    }
                }

                Repeater {
                    model: AppState.boardsModel

                    delegate: BoardCard {
                        width: contentColumn.width
                        cardBoardId: boardId
                        cardTitle: title
                        cardSubtitle: subtitle
                        cardUpdatedLabel: updatedLabel
                        cardPreviewSource: previewSource
                        cardPreviewItems: previewItems
                        cardPreviewCanvasWidth: previewCanvasWidth
                        cardPreviewCanvasHeight: previewCanvasHeight
                        cardPreviewOriginX: previewOriginX
                        cardPreviewOriginY: previewOriginY
                        cardItemCount: itemCount
                        selected: boardId === AppState.selectedBoardId
                        onClicked: root.boardRequested(boardId, title)
                    }
                }

                Rectangle {
                    width: parent.width
                    visible: AppState.boardsModel.count === 0 && !AppState.loadingBoards
                    radius: 20
                    color: "#fff5e8"
                    border.width: 2
                    border.color: "#181818"
                    implicitHeight: emptyText.implicitHeight + 22

                    Text {
                        id: emptyText
                        anchors.fill: parent
                        anchors.margins: 11
                        wrapMode: Text.WordWrap
                        text: "Пока нет облачных досок. Сначала создайте или синхронизируйте доску в настольной версии, затем обновите список здесь."
                        font.pixelSize: 13
                        color: "#5b4d57"
                    }
                }
            }
        }
    }
}
