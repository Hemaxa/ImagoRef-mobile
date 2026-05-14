import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string cardBoardId: ""
    property string cardTitle: ""
    property string cardSubtitle: ""
    property string cardUpdatedLabel: ""
    property string cardPreviewSource: ""
    property var cardPreviewItems: []
    property real cardPreviewCanvasWidth: 0
    property real cardPreviewCanvasHeight: 0
    property real cardPreviewOriginX: 0
    property real cardPreviewOriginY: 0
    property int cardItemCount: 0
    property bool selected: false

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

            Image {
                anchors.fill: parent
                source: ThemeManager.icons.frame || ""
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {
                id: previewViewport
                anchors.fill: parent
                anchors.margins: 12
                radius: 10
                clip: true
                color: root.selected ? "#474056" : "#3c3a3a"

                property real contentWidth: root.cardPreviewCanvasWidth > 0 ? root.cardPreviewCanvasWidth : width
                property real contentHeight: root.cardPreviewCanvasHeight > 0 ? root.cardPreviewCanvasHeight : height
                property real scaleFactor: Math.min(width / Math.max(contentWidth, 1), height / Math.max(contentHeight, 1))

                Item {
                    id: previewScene
                    visible: root.cardPreviewItems.length > 0
                    width: previewViewport.contentWidth * previewViewport.scaleFactor
                    height: previewViewport.contentHeight * previewViewport.scaleFactor
                    anchors.centerIn: parent

                    Repeater {
                        model: root.cardPreviewItems

                        delegate: Item {
                            required property var modelData

                            x: (modelData.x - root.cardPreviewOriginX) * previewViewport.scaleFactor
                            y: (modelData.y - root.cardPreviewOriginY) * previewViewport.scaleFactor
                            width: modelData.width * previewViewport.scaleFactor
                            height: modelData.height * previewViewport.scaleFactor
                            rotation: modelData.rotation
                            transformOrigin: Item.Center
                            opacity: modelData.opacity
                            z: modelData.z

                            Image {
                                anchors.fill: parent
                                source: modelData.source
                                fillMode: Image.PreserveAspectFit
                                asynchronous: true
                                smooth: true
                            }
                        }
                    }
                }

                Image {
                    id: previewImage
                    anchors.fill: parent
                    source: root.cardPreviewSource
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    smooth: true
                    visible: root.cardPreviewItems.length === 0 && status === Image.Ready
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Text {
                Layout.fillWidth: true
                text: root.cardTitle
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                elide: Text.ElideRight
                font.pixelSize: 18
                font.bold: true
                color: root.selected ? ThemeManager.colors.textColor : ThemeManager.colors.welcomeTextDark
            }

            Text {
                Layout.fillWidth: true
                text: root.cardSubtitle
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
                        text: "Облако"
                        font.pixelSize: 11
                        font.bold: true
                        color: root.selected ? ThemeManager.colors.welcomeAccentYellow : "#141414"
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: root.cardUpdatedLabel
                    font.pixelSize: 11
                    color: root.selected ? "#c9bbd5" : "#5b4d57"
                    elide: Text.ElideRight
                }
            }

            Text {
                Layout.fillWidth: true
                text: root.cardItemCount > 0 ? root.cardItemCount + " изображений" : "Готово к просмотру"
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
